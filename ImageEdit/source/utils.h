#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <random>
#include <algorithm>

#define FRAME_HEIGHT 800
#define FRAME_WIDTH 800
#define PI 3.1415926

struct Complex {
	double real;
	double imag;
};

double distance(int i, int j, int c_u, int c_v);
int** getBlock(int** pixelmat, int x, int y);
bool inImage(int** pixelmat, int mheight, int mwidth, int x, int y);
int* nearby(int** pixelmat, int i, int j);
void normalize(int** pixelmat, int mheight, int mwidth);
void quickSort(int a[], int low, int high);
void transpose(double** data, int msize);

#endif
