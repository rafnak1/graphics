/* Produces a LENxLENpx, zoomed snapshot of the graph of a function.
 * Format is ascii PGM. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LEN 1000

double function (double x);
int paint (int xp, int yp, double x0, double y0, double l, double r, double delta);
void export (int l, int M[LEN][LEN]);
double square (double a);

double function (double x) {
	return sin(x) + cos(2*x); /*This is the example function*/
}

int main() {
	int M[LEN][LEN], i, j;
	double delta, r, l, x0, y0;
	printf("Type the spacing between each test ordinate (step): ");
	scanf("%lf", &delta);
	printf("Type the radius of each intercept circle: ");
	scanf("%lf", &r);
	printf("Then the coordinate for centering the square window: ");
	scanf("%lf %lf", &x0, &y0);
	printf("Type the length of the square window: ");
	scanf("%lf", &l);

	for (i = 0; i < LEN; i++) {
		for (j = 0; j < LEN; j++) {
			M[i][j] = paint(j, i, x0, y0, l, r, delta);
		}
	}

	export (LEN, M);
	return 0;
}	

double square (double a) {
	return a * a;
}

int paint (int xp, int yp, double x0, double y0, double l, double r, double delta) {
	float t, x, y, pix;
	pix = l/(double)LEN;
	x = x0 - l/2.0 + pix/2 + xp*pix;
	y = y0 + l/2.0 - pix/2 - yp*pix;

	/*one of the testing points is on x = x0 - l/2*/
	for (t = x0-l/2; t < x0+l/2; t += delta) {
		if (square(x-t) + square(y-function(t)) < square(r)) return 1;
	}

	return 0;
}


void export (int l, int M[LEN][LEN]) {
	FILE *fp;
	int i, j;
	fp = fopen("out.pgm", "w");
	
	fprintf(fp, "P2\n%d %d\n1\n", l, l);
	for (i = 0; i < l; i++) for (j = 0; j < l; j++) fprintf(fp, "%d ", M[i][j]);

	fclose(fp);
}
