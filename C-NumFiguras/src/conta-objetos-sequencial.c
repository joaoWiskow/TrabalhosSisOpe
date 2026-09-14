#include "conta-objetos-sequencial.h"
#include "union_find.h"
#include <stdio.h>
#include <stdlib.h>

/* Algoritmo sequencial de contagem de objetos em 2 passagens */
int conta_objetos_sequencial(const Matrix *mat) {
    int rows, cols, total_cells;
    UnionFind *uf;
    int r, c, k;
    int count = 0;

    /* 8 direcoes de vizinhanca: horizontal, vertical e 4 diagonais */
    static const int dr[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
    static const int dc[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

    if (mat == NULL || mat->rows <= 0 || mat->cols <= 0) {
        return 0;
    }

    rows = mat->rows;
    cols = mat->cols;
    total_cells = rows * cols;

    uf = uf_create(total_cells);
    if (uf == NULL) {
        return -1;
    }

    /* Passagem 1: Unir todas as células 1 adjacentes em 8 direções */
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            if (mat->data[r][c] == 1) {
                int id1 = r * cols + c;
                for (k = 0; k < 8; k++) {
                    int nr = r + dr[k];
                    int nc = c + dc[k];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        if (mat->data[nr][nc] == 1) {
                            int id2 = nr * cols + nc;
                            uf_union(uf, id1, id2);
                        }
                    }
                }
            }
        }
    }

    /* Passagem 2: Contar raizes unicas para celulas de primeiro plano (1) */
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            if (mat->data[r][c] == 1) {
                int id = r * cols + c;
                if (uf_find(uf, id) == id) {
                    count++;
                }
            }
        }
    }

    uf_destroy(uf);
    return count;
}
