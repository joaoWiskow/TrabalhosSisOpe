# Contagem Paralela de Objetos em uma Matriz Binária (ANSI C89 / Pthreads)

> **Disciplina:** Sistemas Operacionais - 2026/II  
> **Professor:** Prof. Filipo Novo Mór  
> **Instituição:** Pontifícia Universidade Católica do Rio Grande do Sul - Escola Politécnica  
> **Autores:** João Pedro Wiskow Marth, Rafael dos Reis, Guilherme Dentzien Silva e Lucas Goettert Lopes  

---

## 📌 Apresentação do Trabalho

Este projeto consiste na implementação e análise de algoritmos para contagem de componentes conexos (objetos) em matrizes binárias utilizando **conectividade 8**. Foram desenvolvidas duas abordagens funcionalmente equivalentes em **ANSI C (C89/C90)** com a biblioteca POSIX Threads (**Pthreads**):

1. **Versão Sequencial:** Servindo como referência de correção e base para medição de desempenho.
2. **Versão Paralela Concorrente:** Utilizando decomposição por blocos 2D e consolidação determinística de fronteiras (*stitching*) entre threads trabalhadoras.

---

## 🛠️ Arquitetura e Estratégia de Paralelização

### 1. Conectividade 8
Dois pixels de valor `1` pertencem ao mesmo objeto se forem adjacentes horizontalmente, verticalmente ou em qualquer uma das 4 diagonais.

### 2. Estrutura de Dados: Union-Find (Disjoint Set Union)
A solução utiliza um **Union-Find** com:
- **Compressão de Caminho (Path Compression):** Reduz a profundidade da árvore para garantir consultas $O(\alpha(N)) \approx O(1)$.
- **União por Rank (Union by Rank):** Mantém as árvores de componentes balanceadas.
- **Sincronização por Mutex (`pthread_mutex_t`):** Garante acesso seguro e atômico durante a fusão de rótulos entre bordas concorrentes.

### 3. Decomposição do Problema e Consolidação de Fronteiras
- **Decomposição Geométrica em Blocos 2D:** A matriz de tamanho $R \times C$ é dividida em uma grade de sub-blocos. Cada thread recebe um bloco único.
- **Fase 1 (Rotulagem Local - Sem Contenção):** Cada thread processa exclusivamente os elementos internos do seu bloco.
- **Fase 2 (Consolidação de Fronteiras / Stitching):** As fronteiras horizontais, verticais e quinas diagonais de 4 blocos são unificadas no Union-Find.
- **Fase 3 (Contagem Global):** Contagem em paralelo das raízes distintas pertencentes a células de primeiro plano (`1`).

---

## 🚀 Compilação e Execução

O projeto segue estritamente o padrão **ANSI C (C89/C90)** e compila em **Linux** e **macOS** sem nenhum erro ou aviso (*warning*).

### 1. Execução Direta (Recomendado)
```bash
make
```
> **Nota para o Professor:** O comando `make` compila o código em C89 estrito e executa automaticamente a validação das 5 matrizes obrigatórias e a avaliação de desempenho (benchmark).

### 2. Recompilação Limpa do Zero
```bash
make clean && make
```
> Apaga executáveis antigos (`make clean`) e recompila todo o projeto do zero, executando em seguida os testes e o benchmark.

---

### Outros Comandos do Makefile:

- **Executar Apenas a Suíte dos 5 Testes Obrigatórios:**
  ```bash
  make test
  ```

- **Executar Apenas a Avaliação de Desempenho (Benchmark):**
  ```bash
  make benchmark
  ```

- **Executar o Benchmark com Exportação para CSV:**
  ```bash
  make run-benchmark
  # Ou parametrizado: ./benchmark <linhas> <colunas> <densidade> <arquivo_csv>
  ```

- **Limpar Arquivos Compilados:**
  ```bash
  make clean
  ```

---

## 📁 Estrutura do Repositório

```text
.
├── README.md                 # Descrição geral, compilação e uso
├── RELATORIO_TECNICO.md      # Relatório técnico completo de 16 seções
├── Makefile                  # Script de compilação ANSI C89
├── src/
│   ├── matrix.h / matrix.c   # Manipulação e alocação de matrizes binárias
│   ├── union_find.h / .c     # Estrutura Union-Find thread-safe
│   ├── conta-objetos-sequencial.h / .c # Algoritmo sequencial de referência
│   └── conta-objetos-paralelo.h / .c   # Algoritmo paralelo com Pthreads e stitching
├── tests/
│   └── test_runner.c         # Suíte de teste das 5 matrizes obrigatórias
├── results/
│   ├── benchmark.c           # Medição de tempo e exportação em CSV
│   ├── medicoes.csv          # Dados brutos das medições
│   ├── grafico-tempo.png     # Gráfico de tempo de execução
│   ├── grafico-aceleracao.png# Gráfico de velocidade (Speedup)
│   └── grafico-eficiencia.png# Gráfico de eficiência paralela
└── slides/
    └── apresentacao.pdf      # Apresentação de slides em PDF (10 min)
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

## 📚 Bibliotecas, Referências, Ferramentas e Códigos Externos

- **Bibliotecas POSIX e Padrão ANSI C (C89):**
  - `<pthread.h>`: Interface POSIX para criação, coordenação e finalização de threads e mutexes.
  - `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<math.h>`: Gerenciamento de E/S, memória e matemática.
  - `<sys/time.h>`: Medição de tempo de alta precisão via `gettimeofday`.
- **Ferramentas:**
  - **GCC / Clang:** Compiladores C (`-std=c89 -Wall -Wextra -pedantic -pthread -O2`).
  - **GNU Make:** Gerenciador de compilação automatizada.
- **Códigos Externos e Referências:**
  - Algoritmo Union-Find (Tarjan, 1975).
