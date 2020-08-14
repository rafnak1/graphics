/* This program outputs a series of frames, each showing a step in the smoothing process
 * of an image. It is intended to be executed by "animate.py".*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 400

void import (int *white, int *m, int *n, int M[MAX][MAX]);
int avg (int M[MAX][MAX], int i, int j, int m, int n);
void export (int framecount, int white, int m, int n, int M[MAX][MAX]);
void tag (char filename[11], int framecount);

int main() {
	int foo[MAX][MAX], bar[MAX][MAX], white, m, n, i, j, numframes, numsteps, stepcount, framecount;
	
	import(&white, &m, &n, foo);
	printf("Number of frames and of operations in between: ");
	scanf("%d %d", &numframes, &numsteps);
	
	for (framecount = 0; framecount < numframes; framecount++) {
		for (stepcount = 0; stepcount < numsteps; stepcount++) {
			for (j=0; j<n; j++) for (i=0; i<m; i++) bar[j][i] = avg(foo, i, j, m, n);
			for (j=0; j<n; j++) for (i=0; i<m; i++) foo[j][i] = avg(bar, i, j, m, n);
		}	
		export(framecount, white, m, n, foo); 
	}

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

void export(int framecount, int white, int m, int n, int M[MAX][MAX]) {
	FILE *fp;
	int i, j;
	char filename[11] = "out000.pgm\0";
	tag(filename, framecount);
	fp = fopen(filename, "w");
	
	fprintf(fp, "P2\n%d %d\n%d\n", m, n, white);
	for (j=0; j<n; j++) for (i=0; i<m; i++) fprintf(fp, "%d ", M[j][i]);

	fclose(fp);
}

void tag (char filename[11], int framecount) {
	int i, pow10;
	pow10 = 100; framecount ++;
	for (i = 3; i < 6; i++) {
		filename[i] = '0' + framecount / pow10;
		framecount %= pow10;
		pow10 /= 10;
	}
}
