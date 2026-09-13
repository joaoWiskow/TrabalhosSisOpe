#include "union_find.h"
#include <stdio.h>
#include <stdlib.h>

UnionFind *uf_create(int n) {
    UnionFind *uf;
    int i;

    if (n <= 0) {
        return NULL;
    }

    uf = (UnionFind *)malloc(sizeof(UnionFind));
    if (uf == NULL) {
        perror("Erro ao alocar memoria para UnionFind");
        return NULL;
    }

    uf->n = n;
    uf->parent = (int *)malloc(n * sizeof(int));
    uf->rank = (int *)malloc(n * sizeof(int));

    if (uf->parent == NULL || uf->rank == NULL) {
        perror("Erro ao alocar vetores para UnionFind");
        if (uf->parent != NULL) free(uf->parent);
        if (uf->rank != NULL) free(uf->rank);
        free(uf);
        return NULL;
    }

    for (i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }

    if (pthread_mutex_init(&uf->lock, NULL) != 0) {
        perror("Erro ao inicializar mutex do UnionFind");
        free(uf->parent);
        free(uf->rank);
        free(uf);
        return NULL;
    }

    return uf;
}

void uf_destroy(UnionFind *uf) {
    if (uf == NULL) {
        return;
    }
    pthread_mutex_destroy(&uf->lock);
    if (uf->parent != NULL) free(uf->parent);
    if (uf->rank != NULL) free(uf->rank);
    free(uf);
}

/* Find com compressao de caminho iterativo (sem recursao para evitar estouro de pilha) */
int uf_find(UnionFind *uf, int i) {
    int root = i;
    int curr, next;

    while (root != uf->parent[root]) {
        root = uf->parent[root];
    }

    /* Compressao de caminho */
    curr = i;
    while (curr != root) {
        next = uf->parent[curr];
        uf->parent[curr] = root;
        curr = next;
    }

    return root;
}

/* Union por rank */
void uf_union(UnionFind *uf, int i, int j) {
    int root_i = uf_find(uf, i);
    int root_j = uf_find(uf, j);

    if (root_i != root_j) {
        if (uf->rank[root_i] < uf->rank[root_j]) {
            uf->parent[root_i] = root_j;
        } else if (uf->rank[root_i] > uf->rank[root_j]) {
            uf->parent[root_j] = root_i;
        } else {
            uf->parent[root_j] = root_i;
            uf->rank[root_i]++;
        }
    }
}

int uf_find_threadsafe(UnionFind *uf, int i) {
    int root;
    pthread_mutex_lock(&uf->lock);
    root = uf_find(uf, i);
    pthread_mutex_unlock(&uf->lock);
    return root;
}

void uf_union_threadsafe(UnionFind *uf, int i, int j) {
    pthread_mutex_lock(&uf->lock);
    uf_union(uf, i, j);
    pthread_mutex_unlock(&uf->lock);
}
