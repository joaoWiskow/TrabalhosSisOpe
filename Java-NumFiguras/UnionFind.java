// Estrutura Union-Find (Disjoint-Set Union) thread-safe
public class UnionFind {
    private int[] parent;
    private int[] rank;

    // Inicializa os conjuntos disjuntos
    public UnionFind(int n) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    // Busca o representante (raiz) com compressão de caminho
    public synchronized int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Compressão de caminho
        }
        return parent[x];
    }

    // Une dois conjuntos utilizando união por rank
    public synchronized void union(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) {
            return;
        }
        if (rank[rx] < rank[ry]) {
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {
            parent[ry] = rx;
        } else {
            parent[ry] = rx;
            rank[rx]++;
        }
    }

    // Verifica se dois elementos pertencem ao mesmo conjunto
    public synchronized boolean connected(int x, int y) {
        return find(x) == find(y);
    }
}