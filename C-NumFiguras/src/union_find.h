#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <pthread.h>

/* Estrutura Union-Find (Disjoint Set Union) em C89 com suporte a concorrência */
typedef struct {
    int *parent;
    int *rank;
    int n;
    pthread_mutex_t lock;
} UnionFind;

UnionFind *uf_create(int n);
void uf_destroy(UnionFind *uf);
int uf_find(UnionFind *uf, int i);
void uf_union(UnionFind *uf, int i, int j);
int uf_find_threadsafe(UnionFind *uf, int i);
void uf_union_threadsafe(UnionFind *uf, int i, int j);

#endif /* UNION_FIND_H */
