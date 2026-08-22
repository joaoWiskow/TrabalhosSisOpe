public class UnionFind {
    private int[] parent;
    private int[] rank;

    public UnionFind(int n) {
        parent = new int[n];
        rank = new int[n];
//Cria dois vetores: parent, que guarda o pai de cada elemento, e rank, que representa aproximadamente a altura da �rvore.
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }//Cada elemento come�a sendo pai de si mesmo. E uma arvore isolada de profundidade 0
    }
    public int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Compress�o de caminho
        }
        return parent[x];
    }//Verifica se x � a raiz da sua �rvore. Se n�o for, procura recursivamente a raiz e faz x apontar diretamente para ela.
    public void union(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) {
            return;
        }// Ja concatenados
        if (rank[rx] < rank[ry]) {//A �rvore cuja raiz � rx possui rank menor que a �rvore cuja raiz � ry. rank n�o estava dizendo que ry era pai de rx. Ele estava sendo usado para decidir quem deveria virar pai.
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {//contrario
            parent[ry] = rx;
        } else {
            parent[ry] = rx;//incrementa o rank de rx pois esta no nivel errado
            rank[rx]++;
        }
    }

    public boolean connected(int x, int y) {
        return find(x) == find(y);
     } 
    }//verifica conexao
    //parent → quem é meu pai?
    // find → qual é minha raiz?
    // rank → qual árvore é mais alta?
    // union → junta duas árvores escolhendo a melhor raiz.
    // connected → as duas têm a mesma raiz?

    // Se fornece:
    // os vértices A e B estão ligados por uma aresta
    // faz:
    // union(A, B);
    // Descobrir pontos com 1
    // Bota-los em um algoritmo que percebe se ha vizinhos com 1 tambem
    // Liga-los com union-find