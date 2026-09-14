#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

/* Aloca uma nova matriz dinamicamente */
Matrix *matrix_create(int rows, int cols) {
    Matrix *mat;
    int i;

    if (rows <= 0 || cols <= 0) {
        return NULL;
    }

    mat = (Matrix *)malloc(sizeof(Matrix));
    if (mat == NULL) {
        perror("Erro ao alocar memoria para Matrix");
        return NULL;
    }

    mat->rows = rows;
    mat->cols = cols;
    mat->data = (int **)malloc(rows * sizeof(int *));
    if (mat->data == NULL) {
        perror("Erro ao alocar memoria para linhas da matriz");
        free(mat);
        return NULL;
    }

    for (i = 0; i < rows; i++) {
        mat->data[i] = (int *)calloc(cols, sizeof(int));
        if (mat->data[i] == NULL) {
            int j;
            perror("Erro ao alocar memoria para colunas da matriz");
            for (j = 0; j < i; j++) {
                free(mat->data[j]);
            }
            free(mat->data);
            free(mat);
            return NULL;
        }
    }

    return mat;
}

/* Desaloca a memória reservada para a matriz */
void matrix_free(Matrix *mat) {
    int i;
    if (mat == NULL) {
        return;
    }
    if (mat->data != NULL) {
        for (i = 0; i < mat->rows; i++) {
            if (mat->data[i] != NULL) {
                free(mat->data[i]);
            }
        }
        free(mat->data);
    }
    free(mat);
}

/* Converte um array unidimensional em uma matriz binária 2D */
Matrix *matrix_create_from_array(int rows, int cols, const int *flat_data) {
    Matrix *mat;
    int i, j;

    mat = matrix_create(rows, cols);
    if (mat == NULL) {
        return NULL;
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            mat->data[i][j] = flat_data[i * cols + j];
        }
    }

    return mat;
}

/* Gera matriz binária aleatória com base em uma densidade */
Matrix *matrix_generate_random(int rows, int cols, double density, unsigned int seed) {
    Matrix *mat;
    int i, j;

    mat = matrix_create(rows, cols);
    if (mat == NULL) {
        return NULL;
    }

    srand(seed);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            double r = (double)rand() / (double)RAND_MAX;
            mat->data[i][j] = (r < density) ? 1 : 0;
        }
    }

    return mat;
}

/* Exibe os valores da matriz no console */
void matrix_print(const Matrix *mat) {
    int i, j;
    if (mat == NULL) {
        printf("(Matriz nula)\n");
        return;
    }

    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
