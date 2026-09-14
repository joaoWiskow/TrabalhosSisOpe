# Makefile na raiz do repositório para facilitar a execução pelo professor ou colega

all:
	$(MAKE) -C C-NumFiguras all

test:
	$(MAKE) -C C-NumFiguras test

benchmark:
	$(MAKE) -C C-NumFiguras run-benchmark

clean:
	$(MAKE) -C C-NumFiguras clean

.PHONY: all test benchmark clean
