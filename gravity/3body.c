/* Simulation of three point-bodies affected only by each other's gravity.*/
/* Outputs "orbit.mp4". */
/* Only SI units are used.*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LEN 1000
#define G 6.67408e-11

/*Structural functions*/
void update (double m[3], double s[3][2], double v[3][2], double time_step);
void render (double radius, double s[3][2], int M[LEN][LEN], double l, double corner[2]);
void export (int count, int l, int M[LEN][LEN]);
/*Auxiliary functions*/
void acc_field(int body_i, double s[3][2], double m[3], double acc[2]);
void svmult (double scalar, double vector[2], double result[2]);
void vsum (double v[2], double u[2] , double w[2]);
double distance (double A[2], double B[2]);
double squared (double x);
void format(char filename[11], int count);

int main() {
	double m[3], s[3][2], v[3][2], time_step;
	double radius, shootcount, corner[2], l;
	int n, count, i, u, M[LEN][LEN], shootfreq;
	
	printf("Type the masses for each one of them: ");
	for (i = 0; i < 3; i++) scanf("%lf", &m[i]);
	printf("Type initial coordinates: ");
	for (i = 0; i < 3; i++) for (u = 0; u < 2; u++) scanf("%lf", &s[i][u]);
	printf("Type initial velocities: ");
	for (i = 0; i < 3; i++) for (u = 0; u < 2; u++) scanf("%lf", &v[i][u]);
	printf("Type length of simulation time step: ");
	scanf("%lf", &time_step);
	
	printf("Coordinate of upper left corner and window length: ");
	scanf("%lf %lf %lf", &corner[0], &corner[1], &l);
	printf("Render every ?? time steps: ");
	scanf("%d", &shootfreq);
	printf("How many time steps?: ");
	scanf("%d", &n);
	printf("Radius for each body: ");
	scanf("%lf", &radius);
	
	shootcount = 1;
	render (radius, s, M, l, corner);
	export (shootcount, LEN, M);
	shootcount ++;
	for (count = 2; count <= n; count++) {
		update (m, s, v, time_step);
		if (count % shootfreq == 0) {
			render (radius, s, M, l, corner);
			export (shootcount, LEN, M);
			shootcount ++;
		}
	}

	system("rm orbit.mp4");
	system("ffmpeg -framerate 10 -i out\%03d.pgm orbit.mp4");
	system("rm *pgm");

	return 0;
}

void update (double m[3], double s[3][2], double v[3][2], double dt) {
	int i, u;
	double acc[3][2], ds[3][2], dv[3][2];

	for (i = 0; i < 3; i++) acc_field(i, s, m, acc[i]);
	for (i = 0; i < 3; i++) {
		svmult (dt, v[i], ds[i]);
		vsum (s[i], ds[i] , s[i]);
	}
	for (i = 0; i < 3; i++) {
		svmult (dt, acc[i], dv[i]);	
		vsum (v[i], dv[i], v[i]);
	}
}

void render (double radius, double s[3][2], int M[LEN][LEN], double l, double corner[2]) {
	int i, j, body, axis;
	double x, y;

	for (i = 0; i < LEN; i++) for (j = 0; j < LEN; j++) M[i][j] = 0;

	for (i = 0; i < LEN; i++) {
		for (j = 0; j < LEN; j++) {
			x = corner[0] + (.5 + (double)j)*(l/LEN);
			y = corner[1] - (.5 + (double)i)*(l/LEN);
			for (body = 0; body < 3; body++) {
				if (squared(x-s[body][0]) + squared(y-s[body][1]) < squared(radius))
					M[i][j] = 1;
			}
		}
	}
}

void export (int count, int l, int M[LEN][LEN]) {
	FILE *fp;
	int i, j;
	char filename[11] = "out000.pgm\0";
	format (filename, count);
	fp = fopen (filename, "w");
	
	fprintf(fp, "P2\n%d %d\n1\n", l, l);
	for (i = 0; i < l; i++) for (j = 0; j < l; j++) fprintf(fp, "%d ", M[i][j]);

	fclose(fp);
}

void acc_field(int body_i, double s[3][2], double m[3], double acc[2]) {
	int i, u;
	double d, accmod;
	for (u = 0; u < 2; u++) acc[u] = 0;

	for (i = 0; i < 3; i++) {
		if (i == body_i) continue;
		d = distance (s[body_i], s[i]);
		accmod = G * m[i] / (d*d);
		for (u = 0; u < 2; u++) { 
			acc[u] += accmod * (s[i][u] - s[body_i][u]) / d;
		}
	}
}

void svmult (double scalar, double vector[2], double result[2]) {
	int u;
	for (u = 0; u < 2; u++) result[u] = scalar * vector[u];
}

void vsum (double v1[2], double v2[2] , double result[2]) {
	int u;
	for (u = 0; u < 2; u++) result[u] = v1[u] + v2[u];
}

double distance (double A[2], double B[2]) {
	int u;
	double sqrdist = 0;
	for (u = 0; u < 2; u++) sqrdist += (A[u]-B[u]) * (A[u]-B[u]);
	return sqrt (sqrdist);
}

double squared (double x) {
	return x * x;
}

void format(char filename[11], int count) {
	int i, power = 100;
	for (i = 3; i <= 5; i++) {
		filename[i] = '0' + count/power;
		count %= power;
		power /= 10;
	}
}
