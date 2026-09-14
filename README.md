# TrabalhosSisOpe - Contagem Paralela de Objetos em Matriz Binária

Este repositório contém a solução do trabalho prático da disciplina de **Sistemas Operacionais (PUCRS)** sobre contagem paralela de objetos conexos em matrizes binárias com **conectividade 8**, comparando implementações sequencial e paralela.

---

## 📂 Estrutura do Repositório

```
TrabalhosSisOpe/
├── Makefile                  # Makefile raiz (compila e executa tudo com apenas 'make')
├── README.md                 # Visão geral do repositório
├── C-NumFiguras/             # IMPLEMENTAÇÃO OBRIGATÓRIA EM ANSI C (C89/C90) COM PTHREADS
│   ├── Makefile              # Compilação em C89 estrito (-std=c89 -Wall -Wextra -pedantic -pthread)
│   ├── README.md             # Documentação técnica completa e fundamentação teórica
│   ├── src/                  # Código-fonte da matriz, Union-Find e algoritmos (Seq / Par)
│   ├── tests/                # Testes automatizados das 5 matrizes obrigatórias
│   ├── results/              # Medição de desempenho e aceleração (Speedup)
│   └── slides/               # Roteiro e apresentação (PDF)
└── Java-NumFiguras/          # IMPLEMENTAÇÃO EM JAVA (SEQUENCIAL E THREADS)
    ├── App.java              # Algoritmo de contagem sequencial e paralelo
    └── UnionFind.java        # Estrutura Union-Find sincronizada
```

---

## 🚀 Como Compilar e Executar

### 1. Execução Direta (Recomendado)

Basta estar na raiz do repositório e executar:
```bash
make
```
> **Nota:** Este comando irá compilar automaticamente o código C no padrão ANSI C89 e executar a suíte de testes validando as 5 matrizes obrigatórias do enunciado.

---

### 2. Opções Adicionais do Makefile (Projeto em C)

Na raiz ou dentro da pasta `C-NumFiguras/`:

- **Executar as 5 Matrizes Obrigatórias:**
  ```bash
  make test
  ```

- **Executar a Medição de Desempenho (Benchmark em Matrizes Grandes):**
  ```bash
  make benchmark
  ```

- **Executar o Benchmark com Tamanho Personalizado:**
  ```bash
  cd C-NumFiguras
  ./benchmark <linhas> <colunas> <densidade>
  # Exemplo de matriz 2000x2000 com 35% de densidade:
  ./benchmark 2000 2000 0.35
  ```

- **Limpar Arquivos Compilados:**
  ```bash
  make clean
  ```

---

### 3. Projeto Auxiliar em Java

Acesse a pasta `Java-NumFiguras`:
```bash
cd Java-NumFiguras
javac App.java UnionFind.java
java App
```

---

## 🎯 Resumo da Solução e Requisitos Atendidos

1. **Linguagem e Padrão:** Desenvolvido em **ANSI C (C89/C90)** utilizando **POSIX Threads (Pthreads)** e sincronização via mutexes.
2. **Conectividade 8:** Varredura em 8 direções (horizontal, vertical e 4 diagonais).
3. **Decomposição e Consolidação de Fronteiras:** Matriz particionada em grade de blocos 2D. Fase 1 de rotulagem local paralela e Fase 2 de consolidação (*stitching*) nas bordas e quinas de 4 blocos para unificar componentes que cruzam regiões.
4. **Validação:** Todas as 5 matrizes obrigatórias (5x5, 6x8, 8x8, 9x12, 12x12) foram validadas com resultados idênticos aos esperados (3, 4, 5, 6 e 7 objetos).
5. **Desempenho:** Avaliação de *Speedup* ($S = T_{seq} / T_{par}$) em matrizes binárias grandes.

---

## 📚 Bibliotecas, Referências, Ferramentas e Códigos Externos

- **Bibliotecas POSIX e Standard C (ANSI C89):**
  - `<pthread.h>`: Chamadas da API POSIX para criação (`pthread_create`), aguardo (`pthread_join`) e sincronização por mutex (`pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`) para controle da região crítica no Union-Find.
  - `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<math.h>`: Funções de entrada/saída, manipulação de memória dinâmica (`malloc`, `free`) e rotinas matemáticas.
  - `<sys/time.h>`: Função POSIX `gettimeofday` para medição de alta precisão de tempo e desempenho.
- **Ferramentas:**
  - **GCC / Clang:** Compiladores C configurados com flags restritivas ANSI C89 (`-std=c89 -Wall -Wextra -pedantic -pthread`).
  - **GNU Make:** Automação de compilação.
  - **Editor de Tabelas C (Filipo Mor):** Ferramenta de apoio para montagem e geração de matrizes de teste (https://filipomor.com/editor-tabelas-c).
- **Códigos Externos e Referências:**
  - Conceito e algoritmos clássicos da estrutura **Union-Find (Disjoint Set Union)** com compressão de caminho e união por rank (Tarjan, 1975).
- **Declaração de Autoria e Responsabilidade:**
  - Toda a lógica de particionamento de matrizes, sincronização de threads, rotulagem local e algoritmo de consolidação de fronteiras (*stitching*) foi implementada e compreendida integralmente pelos autores. A responsabilidade pela correção e compreensão da solução permanece com o grupo.
