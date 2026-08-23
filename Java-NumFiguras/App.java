
public class App {

    record Coordenada(int id, int linha, int coluna) {

    }

    // Primeira Seção -> Verificação das coordenadas onde é igual a 1
    public static Coordenada[] buscaCoor(int imagem[][], int rows, int colms) {
        // Conta o total de '1's para instanciar o array com o tamanho exato (evita nulls)
        int totalUns = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < colms; j++) {
                if (imagem[i][j] == 1) {
                    totalUns++;
                }
            }
        }

        Coordenada coor[] = new Coordenada[totalUns];
        int count = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < colms; j++) {
                if (imagem[i][j] == 1) {
                    coor[count] = new Coordenada(count, i, j);
                    count++;
                }
            }
        }
        return coor;
    }

    // Segunda Seção -> Criar uma ÚNICA instância do UnionFind para o total de coordenadas
    public static UnionFind criaUnionFind(Coordenada coor[]) {
        return new UnionFind(coor.length);
    }

    // Terceira Seção -> Identificar ligações e realizar os unions em O(1) por vizinho
    public static void findPeers(int[][] imagem, Coordenada[] coor, UnionFind uf, int rows, int colms) {
        // Mapeia (linha, coluna) para o ID correspondente para consulta rápida O(1)
        int[][] idGrid = new int[rows][colms];
        for (Coordenada c : coor) {
            idGrid[c.linha()][c.coluna()] = c.id();
        }

        for (Coordenada c : coor) {
            int r = c.linha();
            int col = c.coluna();

            // Checa Vizinho da DIREIRA
            if (col + 1 < colms && imagem[r][col + 1] == 1) {
                int idVizinho = idGrid[r][col + 1];
                uf.union(c.id(), idVizinho); // Executa o union de fato
            }

            // Checa Vizinho de BAIXO
            if (r + 1 < rows && imagem[r + 1][col] == 1) {
                int idVizinho = idGrid[r + 1][col];
                uf.union(c.id(), idVizinho); // Executa o union de fato
            }
        }
    }

    //Quarta Seção -> Union dirá a quantidade de figuras
    public static int contaFiguras(Coordenada[] coor, UnionFind uf) {
        int figuras = 0;
        for (Coordenada c : coor) {
            // Se o pai do elemento é ele mesmo, ele é a raiz de uma figura
            if (uf.find(c.id()) == c.id()) {
                figuras++;
            }
        }
        return figuras;
    }

    public static void main(String[] args) {
        int[][] imagem = {
            {1, 1, 0, 0},
            {1, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 1}
        };

        // Execute as 3 seções em ordem:
        Coordenada coor[] = buscaCoor(imagem, 4, 4);
        UnionFind uf = criaUnionFind(coor);
        findPeers(imagem, coor, uf, 4, 4);

        System.out.println("Union-Find executado e adjacências conectadas!");
        System.out.println(contaFiguras(coor, uf));

    }

}
