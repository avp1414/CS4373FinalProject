cc=gcc
flags=-Wall -O3
gedir=Gaussian\ Elimination
tspdir=Traveling\ Salesperson\ Problem

compile: det tsp

det: $(gedir)/*
	$(cc) $(flags) -o det $(gedir)/Gaussian_Elimination.c $(gedir)/main.c -fopenmp -lm

tsp: $(tspdir)/*
	$(cc) $(flags) -o tsp $(tspdir)/TSP.c $(tspdir)/utils.c $(tspdir)/main.c -lm -lpthread

.PHONY: clean

clean:
	rm -f det tsp