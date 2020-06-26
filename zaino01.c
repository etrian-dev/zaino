// dynamic programming for backpack 0-1 problem
#include <stdio.h>
#include <stdlib.h>

// reads the lenght and max weight, then an array of values and corresponding weights
int read_instance(int **values, int *len, int **weights, int *maxW);
// prints the given instance of the problem
void print(int *values, const int len, int *weights, const int W);
// solves with pd the problem using a table (n+1) x (W + 1)
int solve_pd(int *values, const int len, int *weights, const int W);
// frees matr of i rows
void free_matr(int **m, const int rows);

int main(void) {
	// read the array of n objects and max weight
	int *vals = 0, *weights = 0, n, W;
	if(!read_instance(&vals, &n, &weights, &W)) {
		puts("Failed to read instance of the problem");
		exit(1);
	}
	else {
		// prints the given instance
		print(vals, n, weights, W);
		// solves with dynamic programming in pseudolinear time [theta(nW)]
		int res = solve_pd(vals, n, weights, W);
		if(res < 0) {
			puts("Error in solving the instance");
			free(vals);
			free(weights);
			return 2;
		}
		printf("(One) Optimal solution: %d\n", res);
		// free mem
		free(vals);
		free(weights);
		return 0;
	}
	return 1;
}

int read_instance(int **values, int *len, int **weights, int *maxW) {
	// read leght and max weight
	if(scanf("%d\n%d", len, maxW) != 2) {
		// at least one of the arguments were not read properly: error
		return 0;
	}
	else {
		// calloc arrays
		*values = (int*) calloc(*len, sizeof(int));
		*weights = (int*) calloc(*len, sizeof(int));
		if(*values && *weights) {
			for(int i = 0; i < *len; i++) {
				// verifies the correct formatting for data (ssv)
				if(scanf("%d %d", *values + i, *weights + i) != 2) {
					puts("Wrong data format. Quitting");
					free(*values);
					free(*weights);
					return 0;
				}
			}
			// all went well: arrays filled
			return 1;
		}
		else {
			puts("Failed alloc");
			return 0;
		}
	}
}
void print(int *values, const int len, int *weights, const int W) {
	int i;
	printf("%-10s\t%-10s\t%-10s\n", "OBJECT", "VALUES", "WEIGHTS");
	for(i = 0; i < len; i++) {
		printf("a_%-8d\t%-10d\t%-10d\n", i, values[i], weights[i]);
	}
	printf("Max weight : %d\n", W);
}
int solve_pd(int *values, const int len, int *weights, const int W) {
	// build a table (len+1) x (W+1)
	int i, j;
	int **matr = (int**) calloc(len + 1, sizeof(int*));
	if(matr) {
		for(i = 0; i <= len; i++) {
			matr[i] = calloc(W + 1, sizeof(int));
			if(!matr[i]) {
				puts("Failed calloc");
				return -1;
			}
		}
		// matr alloc'd
		// init base cases (empty backpack or zero max weight) for first row and col
		for(i = 0; i <= len; i++) {
			matr[i][0] = 0;
		}
		for(j = 1; j <= W; j++) {
			matr[0][j] = 0;
		}
		// useful variable to store the resulting value of adding object i to the set
		int add_i;
		// then fill the matrix: the 
		for(i = 1; i <= len; i++) {
			for(j = 1; j <= W; j++) {
				// the same weight without the current object is obvioulsy a solution
				matr[i][j] = matr[i-1][j];
				add_i = matr[i-1][j - weights[i-1]] + values[i-1];
				// if the sum a_i.weight + prev optimal weight with a_0..a_i-1 <= W 
				if(weights[i-1] <= j && add_i > matr[i][j]) {
					// then add v_i in the backpack with the optimal sol for j - w_i
					matr[i][j] = add_i;
				}
				// else no update needed
			}
		}
		// matrix filled: the cell matr[i][j] holds the optimal value, make a copy to free the matrix
		puts("Matrix filled:");
		for(i = 0; i <= len; i++) {
			for(j = 0; j <= W; j++) {
				printf("%4d\t", matr[i][j]);
			}
			putchar('\n');
		}
		int optimal = matr[len][W];
		// free matr
		free_matr(matr, i);
		// return result
		return optimal;
	}
	puts("Failed calloc");
	return -1;
}
void free_matr(int **m, const int rows) {
	for(int i = 0; i < rows; i++) {
		free(m[i]);
	}
	free(m);
}
