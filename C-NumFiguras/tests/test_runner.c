#include "../src/matrix.h"
#include "../src/conta-objetos-sequencial.h"
#include "../src/conta-objetos-paralelo.h"
#include <stdio.h>
#include <stdlib.h>

/* Matriz 1: 5 x 5 (Esperado: 3 objetos, 2x2 blocos) */
static const int ex1_data[5 * 5] = {
    1, 1, 0, 0, 0,
    1, 1, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 1, 0,
    1, 0, 0, 0, 0
};

/* Matriz 2: 6 x 8 (Esperado: 4 objetos, 2x2 blocos) */
static const int ex2_data[6 * 8] = {
    0, 0, 0, 0, 0, 0, 1, 1,
    0, 1, 1, 1, 1, 0, 1, 0,
    0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 1, 1
};

/* Matriz 3: 8 x 8 (Esperado: 5 objetos, 2x2 blocos) */
static const int ex3_data[8 * 8] = {
    1, 1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 1, 1, 0, 1, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 1,
    0, 0, 1, 0, 0, 0, 1, 1
};

/* Matriz 4: 9 x 12 (Esperado: 6 objetos, 3x3 blocos) */
static const int ex4_data[9 * 12] = {
    0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,
    0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1
};

/* Matriz 5: 12 x 12 (Esperado: 7 objetos, 3x3 blocos) */
static const int ex5_data[12 * 12] = {
    1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
    0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1
};

typedef struct {
    int id;
    int rows;
    int cols;
    int esperado;
    const int *data;
    ParallelConfig pcfg;
} TestCase;

/* Executor dos testes de validação */
int main(void) {
    TestCase tests[5];
    int i;
    int falhas = 0;

    tests[0].id = 1; tests[0].rows = 5;  tests[0].cols = 5;  tests[0].esperado = 3; tests[0].data = ex1_data; tests[0].pcfg.grid_rows = 2; tests[0].pcfg.grid_cols = 2; tests[0].pcfg.num_threads = 4;
    tests[1].id = 2; tests[1].rows = 6;  tests[1].cols = 8;  tests[1].esperado = 4; tests[1].data = ex2_data; tests[1].pcfg.grid_rows = 2; tests[1].pcfg.grid_cols = 2; tests[1].pcfg.num_threads = 4;
    tests[2].id = 3; tests[2].rows = 8;  tests[2].cols = 8;  tests[2].esperado = 5; tests[2].data = ex3_data; tests[2].pcfg.grid_rows = 2; tests[2].pcfg.grid_cols = 2; tests[2].pcfg.num_threads = 4;
    tests[3].id = 4; tests[3].rows = 9;  tests[3].cols = 12; tests[3].esperado = 6; tests[3].data = ex4_data; tests[3].pcfg.grid_rows = 3; tests[3].pcfg.grid_cols = 3; tests[3].pcfg.num_threads = 9;
    tests[4].id = 5; tests[4].rows = 12; tests[4].cols = 12; tests[4].esperado = 7; tests[4].data = ex5_data; tests[4].pcfg.grid_rows = 3; tests[4].pcfg.grid_cols = 3; tests[4].pcfg.num_threads = 9;

    printf("================================================================================\n");
    printf("        VALIDACAO DAS MATRIZES OBRIGATORIAS (SEQUENCIAL VS PARALELO PTHREADS)   \n");
    printf("================================================================================\n");
    printf("Ex.  Dimensoes   Esperado   Sequencial   Paralelo   Status\n");
    printf("--------------------------------------------------------------------------------\n");

    for (i = 0; i < 5; i++) {
        Matrix *mat = matrix_create_from_array(tests[i].rows, tests[i].cols, tests[i].data);
        int res_seq, res_par;
        char dim_str[32];

        if (mat == NULL) {
            printf("Erro ao criar matriz para Exemplo %d\n", tests[i].id);
            falhas++;
            continue;
        }

        res_seq = conta_objetos_sequencial(mat);
        res_par = conta_objetos_paralelo_config(mat, &tests[i].pcfg);

        sprintf(dim_str, "%d x %d", tests[i].rows, tests[i].cols);

        if (res_seq == tests[i].esperado && res_par == tests[i].esperado) {
            printf("%-4d %-11s %-10d %-12d %-10d [SUCESSO]\n",
                   tests[i].id, dim_str, tests[i].esperado, res_seq, res_par);
        } else {
            printf("%-4d %-11s %-10d %-12d %-10d [FALHA]\n",
                   tests[i].id, dim_str, tests[i].esperado, res_seq, res_par);
            falhas++;
        }

        matrix_free(mat);
    }

    printf("--------------------------------------------------------------------------------\n");
    if (falhas == 0) {
        printf("RESULTADO FINAL: TODOS OS 5 TESTES OBRIGATORIOS PASSARAM COM SUCESSO!\n");
    } else {
        printf("RESULTADO FINAL: %d TESTES FALHARAM.\n", falhas);
    }
    printf("================================================================================\n");

    return falhas;
}
