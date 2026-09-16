#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

/* Estrutura para matriz binária 2D */
typedef struct {
    int rows;
    int cols;
    int **data;
} Matrix;

/* Aloca dinamicamente uma matriz de zeros */
Matrix *matrix_create(int rows, int cols);

/* Libera a memória da matriz */
void matrix_free(Matrix *mat);

/* Cria matriz a partir de vetor unidimensional (flat) */
Matrix *matrix_create_from_array(int rows, int cols, const int *flat_data);

/* Gera matriz binária aleatória por densidade */
Matrix *matrix_generate_random(int rows, int cols, double density, unsigned int seed);

/* Imprime o conteúdo da matriz no console */
void matrix_print(const Matrix *mat);

#endif /* MATRIX_H */


