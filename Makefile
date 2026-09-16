CC = gcc
CFLAGS = -std=c89 -Wall -Wextra -pedantic -pthread -O2
SRC_DIR = src
TEST_DIR = tests
RESULTS_DIR = results

SRCS = $(SRC_DIR)/matrix.c $(SRC_DIR)/union_find.c $(SRC_DIR)/conta-objetos-sequencial.c $(SRC_DIR)/conta-objetos-paralelo.c

all: test_runner benchmark_bin test benchmark

test_runner: $(SRCS) $(TEST_DIR)/test_runner.c
	$(CC) $(CFLAGS) $(SRCS) $(TEST_DIR)/test_runner.c -o test_runner -lm

benchmark_bin: $(SRCS) $(RESULTS_DIR)/benchmark.c
	$(CC) $(CFLAGS) $(SRCS) $(RESULTS_DIR)/benchmark.c -o benchmark -lm

benchmark: benchmark_bin
	./benchmark 1000 1000 0.30

run-benchmark: benchmark_bin
	./benchmark 5000 5000 0.30 results/medicoes.csv

test: test_runner
	./test_runner

clean:
	rm -f test_runner benchmark *.o

.PHONY: all test benchmark run-benchmark clean
