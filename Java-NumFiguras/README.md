# Versão em Java - Contagem Paralela de Objetos Binários

Este módulo contém a implementação sequencial e paralela em Java para contagem de objetos conexos em matrizes binárias com **conectividade 8**.

---

## 📌 O que Faltava / Foi Corrigido na Versão Anterior em Java

1. **Conectividade 8 Completa:**
   - A versão anterior verificava apenas vizinhos da direita (`col + 1`) e abaixo (`r + 1`), cobrindo apenas 4 direções.
   - Foi atualizada para verificar as 8 direções adjacentes (horizontal, vertical e as 4 diagonais), atendendo estritamente ao enunciado da disciplina.

2. **Paralelismo Real por Blocos com Threads:**
   - O código anterior dividia a matriz mas deixava o processamento paralelo em aberto.
   - Agora, foi implementada a divisão da matriz em sub-blocos 2D, atribuindo cada bloco a uma `Thread` Java independente para rotulagem local sem contenção.

3. **Consolidação de Fronteiras (Stitching):**
   - Implementada a Fase 2 de consolidação que une os rótulos das células `1` que se tocam nas bordas horizontais, verticais e encontros diagonais das quinas de 4 blocos.

4. **Sincronização do Union-Find:**
   - Adicionada sincronização (`synchronized`) na classe `UnionFind` para garantir acesso atômico e thread-safe aos métodos `find`, `union` e `connected` durante a fusão concorrente de bordas.

5. **Suíte de Testes Automatizada:**
   - Adicionada a execução automatizada dos 5 exemplos obrigatórios do enunciado no método `main`, validando se os resultados coincidem exatamente com os valores esperados (3, 4, 5, 6 e 7 objetos).

---

## 🚀 Como Executar

```bash
cd Java-NumFiguras
javac App.java UnionFind.java
java App
```
