#include "../src/matrix.h"
#include "../src/conta-objetos-sequencial.h"
#include "../src/conta-objetos-paralelo.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

static double get_time_sec(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (tv.tv_usec / 1000000.0);
}

int main(int argc, char *argv[]) {
    int rows = 5000;
    int cols = 5000;
    double density = 0.30;
    unsigned int seed = 42;
    int repeticoes = 5;

    Matrix *mat;
    double start, end;
    double t_seq_total = 0.0, t_seq_avg;
    int res_seq = 0, res_par = 0;
    int rep;

    int thread_counts[4] = {1, 2, 4, 8};
    int num_tc = 4;
    int idx;
    FILE *csv_file = NULL;

    if (argc >= 3) {
        rows = atoi(argv[1]);
        cols = atoi(argv[2]);
    }
    if (argc >= 4) {
        density = atof(argv[3]);
    }
    if (argc >= 5) {
        csv_file = fopen(argv[4], "w");
        if (csv_file != NULL) {
            fprintf(csv_file, "matriz,linhas,colunas,versao,trabalhadores,repeticao,tempo_ms,objetos,resultado_correto\n");
        }
    }

    printf("================================================================================\n");
    printf("            AVALIACAO DE DESEMPENHO E ESCALABILIDADE (SPEEDUP)                  \n");
    printf("================================================================================\n");
    printf("Matriz de Teste: %d x %d | Densidade: %.2f | Repeticoes: %d\n", rows, cols, density, repeticoes);
    printf("Gerando matriz binaria aleatoria com semente %u...\n", seed);

    mat = matrix_generate_random(rows, cols, density, seed);
    if (mat == NULL) {
        fprintf(stderr, "Erro ao gerar matriz para benchmark.\n");
        return 1;
    }

    printf("Executando versao SEQUENCIAL (%d repeticoes)...\n", repeticoes);
    for (rep = 0; rep < repeticoes; rep++) {
        start = get_time_sec();
        res_seq = conta_objetos_sequencial(mat);
        end = get_time_sec();
        t_seq_total += (end - start);
        if (csv_file != NULL) {
            fprintf(csv_file, "matriz_5000x5000,%d,%d,sequencial,1,%d,%.3f,%d,true\n",
                    rows, cols, rep + 1, (end - start) * 1000.0, res_seq);
        }
    }
    t_seq_avg = t_seq_total / repeticoes;

    printf("Objetos detectados (Sequencial): %d\n", res_seq);
    printf("Tempo medio Sequencial: %.6f segundos (%.2f ms)\n\n", t_seq_avg, t_seq_avg * 1000.0);

    printf("--------------------------------------------------------------------------------\n");
    printf("Threads   Objetos   Tempo Paralelo (s)   Speedup (S = Tseq / Tpar)   Status     \n");
    printf("--------------------------------------------------------------------------------\n");

    for (idx = 0; idx < num_tc; idx++) {
        int nthreads = thread_counts[idx];
        double t_par_total = 0.0, t_par_avg, speedup;

        for (rep = 0; rep < repeticoes; rep++) {
            start = get_time_sec();
            res_par = conta_objetos_paralelo(mat, nthreads);
            end = get_time_sec();
            t_par_total += (end - start);
            if (csv_file != NULL) {
                fprintf(csv_file, "matriz_5000x5000,%d,%d,paralela,%d,%d,%.3f,%d,%s\n",
                        rows, cols, nthreads, rep + 1, (end - start) * 1000.0, res_par,
                        (res_par == res_seq) ? "true" : "false");
            }
        }
        t_par_avg = t_par_total / repeticoes;
        speedup = t_seq_avg / t_par_avg;

        printf("%-9d %-9d %-20.6f %-25.2fx %s\n",
               nthreads, res_par, t_par_avg, speedup,
               (res_par == res_seq) ? "[CORRETO]" : "[ERRO]");
    }

    printf("================================================================================\n");

    if (csv_file != NULL) {
        fclose(csv_file);
    }
    matrix_free(mat);
    return 0;
}
