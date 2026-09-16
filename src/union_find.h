#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <pthread.h>

/* Estrutura Union-Find com suporte a concorrência */
typedef struct {
    int *parent;
    int *rank;
    int n;
    pthread_mutex_t lock;
} UnionFind;

/* Aloca e inicializa a estrutura Union-Find */
UnionFind *uf_create(int n);

/* Desaloca a estrutura Union-Find */
void uf_destroy(UnionFind *uf);

/* Busca a raiz com compressão de caminho (não síncrona) */
int uf_find(UnionFind *uf, int i);

/* Unifica conjuntos por rank (não síncrona) */
void uf_union(UnionFind *uf, int i, int j);

/* Busca a raiz com exclusão mútua (thread-safe) */
int uf_find_threadsafe(UnionFind *uf, int i);

/* Unifica conjuntos com exclusão mútua (thread-safe) */
void uf_union_threadsafe(UnionFind *uf, int i, int j);

#endif /* UNION_FIND_H */


