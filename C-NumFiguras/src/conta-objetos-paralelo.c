#include "conta-objetos-paralelo.h"
#include "union_find.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

/* Estrutura de dados passada para cada thread trabalhadora */
typedef struct {
    int thread_id;
    int grid_r;
    int grid_c;
    int grid_rows;
    int grid_cols;
    int r_start;
    int r_end;
    int c_start;
    int c_end;
    const Matrix *mat;
    UnionFind *uf;
    int local_count;
} ThreadData;

/* 8 direcoes de vizinhanca */
static const int dr[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
static const int dc[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

/* Função executada pelas threads na Fase 1: Rotulagem Local */
static void *thread_local_labeling(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    const Matrix *mat = data->mat;
    UnionFind *uf = data->uf;
    int cols = mat->cols;
    int r, c, k;

    for (r = data->r_start; r < data->r_end; r++) {
        for (c = data->c_start; c < data->c_end; c++) {
            if (mat->data[r][c] == 1) {
                int id1 = r * cols + c;
                for (k = 0; k < 8; k++) {
                    int nr = r + dr[k];
                    int nc = c + dc[k];
                    /* Verifica se o vizinho esta dentro do bloco local da thread */
                    if (nr >= data->r_start && nr < data->r_end &&
                        nc >= data->c_start && nc < data->c_end) {
                        if (mat->data[nr][nc] == 1) {
                            int id2 = nr * cols + nc;
                            uf_union(uf, id1, id2);
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

/* Consolidação de Fronteiras entre blocos adjacentes (Fase 2) */
static void consolidar_fronteiras(const Matrix *mat, UnionFind *uf, int grid_rows, int grid_cols) {
    int rows = mat->rows;
    int cols = mat->cols;
    int gr, gc, r, c;

    /* 1. Consolidação de Fronteiras Horizontais (entre linhas de blocos gr e gr+1) */
    for (gr = 0; gr < grid_rows - 1; gr++) {
        int r_boundary = ((gr + 1) * rows) / grid_rows - 1;
        int r_next = r_boundary + 1;

        if (r_next < rows) {
            for (c = 0; c < cols; c++) {
                if (mat->data[r_boundary][c] == 1) {
                    int id1 = r_boundary * cols + c;

                    /* Vizinho diretamente abaixo */
                    if (mat->data[r_next][c] == 1) {
                        uf_union_threadsafe(uf, id1, r_next * cols + c);
                    }
                    /* Vizinho diagonal abaixo-esquerda */
                    if (c - 1 >= 0 && mat->data[r_next][c - 1] == 1) {
                        uf_union_threadsafe(uf, id1, r_next * cols + (c - 1));
                    }
                    /* Vizinho diagonal abaixo-direita */
                    if (c + 1 < cols && mat->data[r_next][c + 1] == 1) {
                        uf_union_threadsafe(uf, id1, r_next * cols + (c + 1));
                    }
                }
            }
        }
    }

    /* 2. Consolidação de Fronteiras Verticais (entre colunas de blocos gc e gc+1) */
    for (gc = 0; gc < grid_cols - 1; gc++) {
        int c_boundary = ((gc + 1) * cols) / grid_cols - 1;
        int c_next = c_boundary + 1;

        if (c_next < cols) {
            for (r = 0; r < rows; r++) {
                if (mat->data[r][c_boundary] == 1) {
                    int id1 = r * cols + c_boundary;

                    /* Vizinho diretamente a direita */
                    if (mat->data[r][c_next] == 1) {
                        uf_union_threadsafe(uf, id1, r * cols + c_next);
                    }
                    /* Vizinho diagonal acima-direita */
                    if (r - 1 >= 0 && mat->data[r - 1][c_next] == 1) {
                        uf_union_threadsafe(uf, id1, (r - 1) * cols + c_next);
                    }
                    /* Vizinho diagonal abaixo-direita */
                    if (r + 1 < rows && mat->data[r + 1][c_next] == 1) {
                        uf_union_threadsafe(uf, id1, (r + 1) * cols + c_next);
                    }
                }
            }
        }
    }
}

/* Função executada pelas threads na Fase 3: Contagem Local de Raízes */
static void *thread_local_counting(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    const Matrix *mat = data->mat;
    UnionFind *uf = data->uf;
    int cols = mat->cols;
    int r, c;
    int count = 0;

    for (r = data->r_start; r < data->r_end; r++) {
        for (c = data->c_start; c < data->c_end; c++) {
            if (mat->data[r][c] == 1) {
                int id = r * cols + c;
                if (uf_find_threadsafe(uf, id) == id) {
                    count++;
                }
            }
        }
    }

    data->local_count = count;
    return NULL;
}

int conta_objetos_paralelo_config(const Matrix *mat, const ParallelConfig *config) {
    int rows, cols, total_cells;
    int grid_rows, grid_cols, num_threads;
    UnionFind *uf;
    pthread_t *threads;
    ThreadData *tdata;
    int i, t, total_objects = 0;

    if (mat == NULL || mat->rows <= 0 || mat->cols <= 0) {
        return 0;
    }

    rows = mat->rows;
    cols = mat->cols;
    total_cells = rows * cols;

    grid_rows = (config != NULL && config->grid_rows > 0) ? config->grid_rows : 1;
    grid_cols = (config != NULL && config->grid_cols > 0) ? config->grid_cols : 1;
    num_threads = grid_rows * grid_cols;

    uf = uf_create(total_cells);
    if (uf == NULL) {
        return -1;
    }

    threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    tdata = (ThreadData *)malloc(num_threads * sizeof(ThreadData));

    if (threads == NULL || tdata == NULL) {
        perror("Erro ao alocar threads para contagem paralela");
        if (threads != NULL) free(threads);
        if (tdata != NULL) free(tdata);
        uf_destroy(uf);
        return -1;
    }

    /* Inicialização do particionamento por blocos 2D */
    t = 0;
    for (i = 0; i < grid_rows; i++) {
        int j;
        for (j = 0; j < grid_cols; j++) {
            tdata[t].thread_id = t;
            tdata[t].grid_r = i;
            tdata[t].grid_c = j;
            tdata[t].grid_rows = grid_rows;
            tdata[t].grid_cols = grid_cols;
            tdata[t].r_start = (i * rows) / grid_rows;
            tdata[t].r_end = ((i + 1) * rows) / grid_rows;
            tdata[t].c_start = (j * cols) / grid_cols;
            tdata[t].c_end = ((j + 1) * cols) / grid_cols;
            tdata[t].mat = mat;
            tdata[t].uf = uf;
            tdata[t].local_count = 0;
            t++;
        }
    }

    /* Fase 1: Criação e execução das threads para rotulagem local */
    for (t = 0; t < num_threads; t++) {
        if (pthread_create(&threads[t], NULL, thread_local_labeling, &tdata[t]) != 0) {
            perror("Erro ao criar thread na Fase 1");
            free(threads);
            free(tdata);
            uf_destroy(uf);
            return -1;
        }
    }

    /* Aguarda a conclusão da Fase 1 */
    for (t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    /* Fase 2: Consolidação de Fronteiras (Stitching) */
    consolidar_fronteiras(mat, uf, grid_rows, grid_cols);

    /* Fase 3: Contagem local em paralelo das raízes do Union-Find */
    for (t = 0; t < num_threads; t++) {
        if (pthread_create(&threads[t], NULL, thread_local_counting, &tdata[t]) != 0) {
            perror("Erro ao criar thread na Fase 3");
            free(threads);
            free(tdata);
            uf_destroy(uf);
            return -1;
        }
    }

    /* Aguarda a conclusão da Fase 3 e consolida o total */
    for (t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
        total_objects += tdata[t].local_count;
    }

    free(threads);
    free(tdata);
    uf_destroy(uf);

    return total_objects;
}

int conta_objetos_paralelo(const Matrix *mat, int num_threads) {
    ParallelConfig config;

    if (num_threads <= 1) {
        config.num_threads = 1;
        config.grid_rows = 1;
        config.grid_cols = 1;
    } else if (num_threads == 2) {
        config.num_threads = 2;
        config.grid_rows = 2;
        config.grid_cols = 1;
    } else if (num_threads == 4) {
        config.num_threads = 4;
        config.grid_rows = 2;
        config.grid_cols = 2;
    } else if (num_threads == 9) {
        config.num_threads = 9;
        config.grid_rows = 3;
        config.grid_cols = 3;
    } else {
        /* Decomposição generica aproximando uma grade quadrada */
        int g_r = (int)sqrt((double)num_threads);
        if (g_r < 1) g_r = 1;
        config.grid_rows = g_r;
        config.grid_cols = num_threads / g_r;
        config.num_threads = config.grid_rows * config.grid_cols;
    }

    return conta_objetos_paralelo_config(mat, &config);
}
