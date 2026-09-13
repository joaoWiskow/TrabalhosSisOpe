public class App {

    record Coordenada(int id, int linha, int coluna) {}

    // 8 Direções de vizinhança (horizontal, vertical e 4 diagonais)
    private static final int[] DR = {-1, -1, -1,  0, 0,  1, 1, 1};
    private static final int[] DC = {-1,  0,  1, -1, 1, -1, 0, 1};

    // Método Sequencial de referência com Conectividade 8
    public static int contaObjetosSequencial(int[][] imagem) {
        int rows = imagem.length;
        int cols = imagem[0].length;
        UnionFind uf = new UnionFind(rows * cols);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (imagem[r][c] == 1) {
                    int id1 = r * cols + c;
                    for (int k = 0; k < 8; k++) {
                        int nr = r + DR[k];
                        int nc = c + DC[k];
                        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                            if (imagem[nr][nc] == 1) {
                                int id2 = nr * cols + nc;
                                uf.union(id1, id2);
                            }
                        }
                    }
                }
            }
        }

        int objetos = 0;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (imagem[r][c] == 1) {
                    int id = r * cols + c;
                    if (uf.find(id) == id) {
                        objetos++;
                    }
                }
            }
        }
        return objetos;
    }

    // Método Paralelo com Decomposição por Blocos e Consolidação de Fronteiras
    public static int contaObjetosParalelo(int[][] imagem, int gridRows, int gridCols) {
        int rows = imagem.length;
        int cols = imagem[0].length;
        UnionFind uf = new UnionFind(rows * cols);

        int numThreads = gridRows * gridCols;
        Thread[] threads = new Thread[numThreads];

        // Fase 1: Rotulagem Local em Paralelo por Bloco
        int t = 0;
        for (int gr = 0; gr < gridRows; gr++) {
            for (int gc = 0; gc < gridCols; gc++) {
                final int rStart = (gr * rows) / gridRows;
                final int rEnd = ((gr + 1) * rows) / gridRows;
                final int cStart = (gc * cols) / gridCols;
                final int cEnd = ((gc + 1) * cols) / gridCols;

                threads[t] = new Thread(() -> {
                    for (int r = rStart; r < rEnd; r++) {
                        for (int c = cStart; c < cEnd; c++) {
                            if (imagem[r][c] == 1) {
                                int id1 = r * cols + c;
                                for (int k = 0; k < 8; k++) {
                                    int nr = r + DR[k];
                                    int nc = c + DC[k];
                                    if (nr >= rStart && nr < rEnd && nc >= cStart && nc < cEnd) {
                                        if (imagem[nr][nc] == 1) {
                                            int id2 = nr * cols + nc;
                                            uf.union(id1, id2);
                                        }
                                    }
                                }
                            }
                        }
                    }
                });
                threads[t].start();
                t++;
            }
        }

        // Aguarda Fase 1
        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // Fase 2: Consolidação de Fronteiras (Stitching)
        // 1. Fronteiras Horizontais
        for (int gr = 0; gr < gridRows - 1; gr++) {
            int rBoundary = ((gr + 1) * rows) / gridRows - 1;
            int rNext = rBoundary + 1;
            if (rNext < rows) {
                for (int c = 0; c < cols; c++) {
                    if (imagem[rBoundary][c] == 1) {
                        int id1 = rBoundary * cols + c;
                        if (imagem[rNext][c] == 1) {
                            uf.union(id1, rNext * cols + c);
                        }
                        if (c - 1 >= 0 && imagem[rNext][c - 1] == 1) {
                            uf.union(id1, rNext * cols + (c - 1));
                        }
                        if (c + 1 < cols && imagem[rNext][c + 1] == 1) {
                            uf.union(id1, rNext * cols + (c + 1));
                        }
                    }
                }
            }
        }

        // 2. Fronteiras Verticais
        for (int gc = 0; gc < gridCols - 1; gc++) {
            int cBoundary = ((gc + 1) * cols) / gridCols - 1;
            int cNext = cBoundary + 1;
            if (cNext < cols) {
                for (int r = 0; r < rows; r++) {
                    if (imagem[r][cBoundary] == 1) {
                        int id1 = r * cols + cBoundary;
                        if (imagem[r][cNext] == 1) {
                            uf.union(id1, r * cols + cNext);
                        }
                        if (r - 1 >= 0 && imagem[r - 1][cNext] == 1) {
                            uf.union(id1, (r - 1) * cols + cNext);
                        }
                        if (r + 1 < rows && imagem[r + 1][cNext] == 1) {
                            uf.union(id1, (r + 1) * cols + cNext);
                        }
                    }
                }
            }
        }

        // Fase 3: Contagem Final de Raízes Únicas
        int objetos = 0;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (imagem[r][c] == 1) {
                    int id = r * cols + c;
                    if (uf.find(id) == id) {
                        objetos++;
                    }
                }
            }
        }
        return objetos;
    }

    public static void main(String[] args) {
        System.out.println("================================================================================");
        System.out.println("            CONTAGEM DE OBJETOS EM JAVA (SEQUENCIAL VS PARALELO)                ");
        System.out.println("================================================================================");

        int[][][] testes = {
            // Ex 1: 5x5 (esperado 3)
            {
                {1, 1, 0, 0, 0},
                {1, 1, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0},
                {1, 0, 0, 0, 0}
            },
            // Ex 2: 6x8 (esperado 4)
            {
                {0, 0, 0, 0, 0, 0, 1, 1},
                {0, 1, 1, 1, 1, 0, 1, 0},
                {0, 0, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 1},
                {1, 1, 0, 0, 0, 0, 1, 1}
            },
            // Ex 3: 8x8 (esperado 5)
            {
                {1, 1, 0, 0, 0, 0, 0, 0},
                {1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 1, 0},
                {0, 0, 0, 1, 1, 0, 1, 0},
                {0, 0, 0, 1, 1, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0, 0, 1},
                {0, 0, 1, 0, 0, 0, 1, 1}
            },
            // Ex 4: 9x12 (esperado 6)
            {
                {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                {0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0},
                {0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0},
                {0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0},
                {0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0},
                {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}
            },
            // Ex 5: 12x12 (esperado 7)
            {
                {1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1},
                {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1}
            }
        };

        int[] esperados = {3, 4, 5, 6, 7};
        int[][] grades = {{2, 2}, {2, 2}, {2, 2}, {3, 3}, {3, 3}};

        for (int i = 0; i < testes.length; i++) {
            int[][] mat = testes[i];
            int exp = esperados[i];
            int gR = grades[i][0];
            int gC = grades[i][1];

            int seq = contaObjetosSequencial(mat);
            int par = contaObjetosParalelo(mat, gR, gC);

            System.out.printf("Exemplo %d (%dx%d): Esperado=%d | Sequencial=%d | Paralelo=%d | %s%n",
                (i + 1), mat.length, mat[0].length, exp, seq, par,
                (seq == exp && par == exp) ? "[OK]" : "[FALHA]");
        }
        System.out.println("================================================================================");
    }
}