/*This program applies a simple edge smoothing operation to images at specified intensity.
 * It imports an "initial.pgm" and outputs "final.pgm".*/ 

#include <stdio.h>
#include <stdlib.h>
#define MAX 400

void import (int *white, int *m, int *n, int M[MAX][MAX]);
int avg (int M[MAX][MAX], int i, int j, int m, int n);
void export (int white, int m, int n, int M[MAX][MAX]);

int main() {
	int foo[MAX][MAX], bar[MAX][MAX], white, m, n, i, j, opquant, opcount;
	
	import(&white, &m, &n, foo);
	printf("Number of operations: ");
	scanf("%d", &opquant);
	
	for (opcount = 0; opcount < opquant; opcount++) {
		for (j=0; j<n; j++) for (i=0; i<m; i++) bar[j][i] = avg(foo, i, j, m, n);
		for (j=0; j<n; j++) for (i=0; i<m; i++) foo[j][i] = avg(bar, i, j, m, n);
	}	

	export(white, m, n, foo); 

	return 0;	
}

void import (int *white, int *m, int *n, int M[MAX][MAX]) {
	FILE *fp;
	char P2[2];
	int i, j;
	fp = fopen("initial.pgm", "r");
	
	fscanf(fp, "%c", &P2[0]); fscanf(fp, "%c", &P2[1]);
	fscanf(fp, "%d %d %d", m, n, white);
	for (j=0; j<(*n); j++) for (i=0; i<(*m); i++) fscanf(fp, "%d", &M[j][i]);

	fclose(fp);
}

int avg(int M[MAX][MAX], int x, int y, int m, int n) {
	int i, j, sample=0, sum=0;

	for (j = -1; j < 2; j++) {
		for (i = -1; i < 2; i++) {
			if ((0<=x+i && x+i<m) && (0<=y+j && y+j<n)) {
				sample ++;
				sum += M[y+j][x+i];
			}
		}
	}

	return sum / sample;
}

void export(int white, int m, int n, int M[MAX][MAX]) {
	FILE *fp;
	int i, j;
	fp = fopen("final.pgm", "w");
	
	fprintf(fp, "P2\n%d %d\n%d\n", m, n, white);
	for (j=0; j<n; j++) for (i=0; i<m; i++) fprintf(fp, "%d ", M[j][i]);

	fclose(fp);
}

