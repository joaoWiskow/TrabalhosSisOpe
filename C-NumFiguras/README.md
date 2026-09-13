# Contagem Paralela de Objetos em uma Matriz Binária (ANSI C89 / Pthreads)

## 📌 Apresentação do Trabalho
Este projeto consiste na implementação e análise de algoritmos para contagem de componentes conexos (objetos) em matrizes binárias utilizando **conectividade 8**. Foram desenvolvidas duas abordagens funcionalmente equivalentes em **ANSI C (C89/C90)** com a biblioteca POSIX Threads (**Pthreads**):

1. **Versão Sequencial:** Servindo como referência de correção e base para medição de desempenho.
2. **Versão Paralela Concorrente:** Utilizando decomposição por blocos 2D e consolidação determinística de fronteiras (*stitching*) entre threads trabalhadoras.

---

## 🛠️ Arquitetura e Estratégia de Paralelização

### 1. Conectividade 8
Dois pixels $P_1 = (r_1, c_1)$ e $P_2 = (r_2, c_2)$ de valor `1` pertencem ao mesmo objeto se forem adjacentes horizontalmente, verticalmente ou em qualquer uma das 4 diagonais:
$$\Delta r \in \{-1, 0, 1\}, \quad \Delta c \in \{-1, 0, 1\} \quad (|\Delta r| + |\Delta c| > 0)$$

### 2. Estrutura de Dados: Union-Find (Disjoint Set Union)
A solução utiliza um **Union-Find** com:
- **Compressão de Caminho (Path Compression):** Reduz a profundidade da árvore para garantir consultas $O(\alpha(N)) \approx O(1)$.
- **União por Rank (Union by Rank):** Mantém as árvores de componentes balanceadas.
- **Sincronização por Mutex (`pthread_mutex_t`):** Garante acesso seguro e atômico durante a fusão de rótulos entre bordas concorrentes.

### 3. Decomposição do Problema e Consolidação de Fronteiras
- **Decomposição Geométrica em Blocos 2D:** A matriz de tamanho $R \times C$ é dividida em uma grade $P_r \times P_c$ de sub-blocos. Cada thread recebe um bloco único.
- **Fase 1 (Rotulagem Local - Sem Contenção):** Cada thread processa exclusivamente os elementos e adjacências internos do seu bloco, realizando uniões no Union-Find de forma paralela e independente.
- **Fase 2 (Consolidação de Fronteiras / Stitching):** As fronteiras entre blocos adjacentes são inspecionadas:
  - *Fronteiras Horizontais:* Unificação de células `1` conexas na borda inferior do bloco superior com a borda superior do bloco inferior.
  - *Fronteiras Verticais:* Unificação de células `1` conexas na borda direita do bloco esquerdo com a borda esquerda do bloco direito.
  - *Encontro de 4 Blocos e Diagonais:* Tratamento específico de conexões diagonais que cruzam vértices e quinas de 4 blocos concorrentes.
- **Fase 3 (Contagem Global):** Contagem em paralelo das raízes distintas no Union-Find pertencentes a células de primeiro plano (`1`).

---

## 🚀 Compilação e Execução

O projeto segue estritamente o padrão **ANSI C (C89/C90)** e compila em **Linux** e **macOS** sem nenhum erro ou aviso (*warning*).

### Comando de Compilação de Referência (C89 Estrito):
```bash
gcc -std=c89 -Wall -Wextra -pedantic -pthread -O2 src/matrix.c src/union_find.c src/conta_objetos_seq.c src/conta_objetos_par.c tests/test_runner.c -o test_runner -lm
```

### Utilizando o `Makefile`:

1. **Compilar e Executar a Suíte de Testes Obrigatórios:**
   ```bash
   make test
   ```

2. **Compilar e Executar a Avaliação de Desempenho (Benchmark):**
   ```bash
   make run-benchmark
   ```
   *Ou parametrizado:*
   ```bash
   ./benchmark <linhas> <colunas> <densidade>
   # Exemplo: ./benchmark 2000 2000 0.35
   ```

3. **Limpar Arquivos Compilados:**
   ```bash
   make clean
   ```

---

## 📊 Matrizes Obrigatórias de Teste e Resultados

Todas as 5 matrizes obrigatórias foram executadas e validadas pela suíte de teste automatizada:

| Ex. | Dimensões | Grade Ilustrativa | Esperado | Sequencial | Paralelo (Pthreads) | Status |
|:---:|:---------:|:-----------------:|:--------:|:----------:|:------------------:|:------:|
| **1** | 5 x 5 | 2 x 2 blocos | **3** | 3 | 3 | **SUCESSO** |
| **2** | 6 x 8 | 2 x 2 blocos | **4** | 4 | 4 | **SUCESSO** |
| **3** | 8 x 8 | 2 x 2 blocos | **5** | 5 | 5 | **SUCESSO** |
| **4** | 9 x 12 | 3 x 3 blocos | **6** | 6 | 6 | **SUCESSO** |
| **5** | 12 x 12 | 3 x 3 blocos | **7** | 7 | 7 | **SUCESSO** |

---

## 📈 Análise de Desempenho e Aceleração (*Speedup*)

Fórmula da Aceleração:
$$S = \frac{T_{\text{sequencial}}}{T_{\text{paralelo}}}$$

### Discussão dos Resultados:
- **Matrizes Pequenas (ex: 5x5 a 12x12):** O custo (*overhead*) de criação de threads (`pthread_create`), sincronização e junção (`pthread_join`) supera o tempo de processamento do algoritmo. Nesses casos, a versão sequencial pode ser numericamente mais rápida.
- **Matrizes Grandes (ex: 1000x1000 a 4000x4000):** A decomposição em blocos e o processamento paralelo dominam o tempo total de cálculo, atingindo aceleração efetiva (*speedup*) com a distribuição do trabalho entre múltiplos núcleos de CPU.

---

## 📚 Bibliotecas, Referências, Ferramentas e Códigos Externos

- **Bibliotecas POSIX e Padrão ANSI C (C89):**
  - `<pthread.h>`: Interface POSIX para criação, coordenação e finalização de threads concorrentes, além do uso de mutexes (`pthread_mutex_t`) para sincronização da região crítica no Union-Find.
  - `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<math.h>`: Gerenciamento de E/S, alocação dinâmica de memória (`malloc`, `free`) e funções matemáticas.
  - `<sys/time.h>`: Chamada POSIX `gettimeofday` para medição de alta precisão do tempo de execução sequencial e paralelo nas análises de desempenho.
- **Ferramentas:**
  - **GCC / Clang:** Compiladores C com suporte a flags de conformidade ANSI C89 (`-std=c89 -Wall -Wextra -pedantic -pthread`).
  - **GNU Make:** Gerenciador de compilação automatizada.
  - **Editor de Tabelas C (Filipo Mor):** Referência para estruturação das matrizes binárias de teste (https://filipomor.com/editor-tabelas-c).
- **Códigos Externos e Referências:**
  - Implementação da estrutura de dados **Union-Find (Disjoint Set Union)** adaptada com compressão de caminho iterativa e união por rank baseada em algoritmos clássicos de teoria dos grafos (Tarjan, 1975).
- **Declaração de Autoria e Responsabilidade:**
  - Todo o código de decomposição por blocos 2D, sincronização por mutexes, rotulagem local e algoritmo de consolidação de fronteiras (*stitching*) foi desenvolvido e compreendido integralmente pelos integrantes do grupo. A responsabilidade pela correção e compreensão da solução permanece com o grupo.

---

## 📁 Estrutura do Repositório

```
C-NumFiguras/
├── Makefile                  # Script de automação de compilação C89
├── README.md                 # Relatório e documentação do projeto
├── src/
│   ├── matrix.h / matrix.c   # Manipulação e geração de matrizes
│   ├── union_find.h / .c     # Estrutura Union-Find thread-safe em C89
│   ├── conta_objetos_seq.h/.c# Algoritmo sequencial de referência
│   └── conta_objetos_par.h/.c# Algoritmo paralelo com Pthreads e consolidação
├── tests/
│   └── test_runner.c         # Execução das 5 matrizes obrigatórias
├── results/
│   └── benchmark.c           # Medição de tempo e aceleração
└── slides/
    └── README.md             # Guia da apresentação (10 min)
```

---

## 📢 Instruções da Apresentação em Aula (10 min)

A apresentação deve seguir o roteiro abaixo (slides disponíveis em formato PDF na pasta `slides/`):

1. **1 min:** Problema e estratégia de decomposição em blocos 2D.
2. **2 min:** Algoritmo sequencial de referência e conectividade 8.
3. **2 min:** Criação de Pthreads e divisão do trabalho.
4. **2 min:** Algoritmo de consolidação de fronteiras (*stitching*) e tratamento das 4 quinas.
5. **2 min:** Resultados das matrizes obrigatórias e análise de speedup.
6. **1 min:** Conclusões e considerações finais.
