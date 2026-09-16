#ifndef CONTA_OBJETOS_PARALELO_H
#define CONTA_OBJETOS_PARALELO_H

#include "matrix.h"

/* Configuração da grade de decomposição 2D */
typedef struct {
    int num_threads;
    int grid_rows;
    int grid_cols;
} ParallelConfig;

/* Contagem paralela de objetos determinando a grade automaticamente */
int conta_objetos_paralelo(const Matrix *mat, int num_threads);

/* Contagem paralela de objetos com configuração explícita de grade */
int conta_objetos_paralelo_config(const Matrix *mat, const ParallelConfig *config);

#endif /* CONTA_OBJETOS_PARALELO_H */


