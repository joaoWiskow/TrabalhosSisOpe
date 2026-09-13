#ifndef CONTA_OBJETOS_PAR_H
#define CONTA_OBJETOS_PAR_H

#include "matrix.h"

typedef struct {
    int num_threads;
    int grid_rows;
    int grid_cols;
} ParallelConfig;

int conta_objetos_paralelo(const Matrix *mat, int num_threads);
int conta_objetos_paralelo_config(const Matrix *mat, const ParallelConfig *config);

#endif /* CONTA_OBJETOS_PAR_H */
