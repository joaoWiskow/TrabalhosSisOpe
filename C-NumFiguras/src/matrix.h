#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

/* Estrutura para representar a matriz binaria em C89 */
typedef struct {
    int rows;
    int cols;
    int **data;
} Matrix;

Matrix *matrix_create(int rows, int cols);
void matrix_free(Matrix *mat);
Matrix *matrix_create_from_array(int rows, int cols, const int *flat_data);
Matrix *matrix_generate_random(int rows, int cols, double density, unsigned int seed);
void matrix_print(const Matrix *mat);

#endif /* MATRIX_H */
