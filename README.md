# CS-4373 Final Project

By Aaron Phung and Andrew Roark

## Building

Use `make` to build this project. The makefile will create the executables `det` and `tsp`. Using the command `make clean` will remove these executables.

## Matrix Determinant

The executable `det` requires either a single argument, the size of the matrix, or two arguments, the input file path and the size of the matrix. If a file path is not provided, the file is assumed to be in the current working directory, named `"m{n}x{n}.bin"`, with n being padded to 4 digits with zeros.

To calculate the determinant of the matrix, the provided matrix is triangularized with Gaussian elimination, which transforms the matrix while preserving its determinant. Once triangularized, the determinant can simply be calculated as the product of the diagonal entries of the matrix. Due to properties of logarithms, the log absolute determinant can be calculated as the trace of the log absolute values of the diagonal entries of the matrix.

To parallelize Gaussian elimination, OpenMP is used to parallelize the inner pair of loops that add a multiple of the ith row to all rows below it, to zero all of their entries in the ith column. This is accomplished by first calculating the necessary factor for each row j, and then using the parallel for directive with a collapse clause to parallelize the inner two loops that add the multiple of row i to row j.

## Traveling Salesperson Problem

The executable `tsp` requires two arguments: the number of threads to create and the adjacency matrix csv file path.

The algorithm used to find the tour uses a randomized greedy-weighted search. Each city keeps a copy of its outgoing paths, sorted by length. To select a path, a random index is selected, with an approximately geometric distribution, so lower indices are exponentially more likely than higher ones. (In practice, this means only the shortest 17 edges have a chance of being selected, and the first edge in the list is selected ~75% of the time). This algorithm has the advantage of being able to search every path, at least in theory, it will never get stuck in a local optimum since it creates a new path each iteration, and it is trivially parallelizable, only needing to share the length of the best path.

Parallelization is trivially accomplished using PThreads. The provided number of threads is created, and each thread runs the previously mentioned algorithm. When a thread finds a new best path, it acquires a lock to update the best path length, and copy its path into the shared best path. The original thread that created these threads does a 60-second timed wait on a condition variable. After this wait, it sets a flag that tells the created threads to return, and then, it joins those threads. After this, the best path is returned.