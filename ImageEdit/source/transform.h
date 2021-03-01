#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "utils.h"
#include "fft.h"

// geometric transform
int** centralize(int** framemat, int** pixelmat, int mheight, int mwidth);
int** rotate(int** framemat, int** pixelmat, int mheight, int mwidth);
// linear transform
int** DFT(int** pixelmat, int mheight, int mwidth);
int** DCT(int** pixelmat, int mheight, int mwidth);
double** walshBlock(int** pixelmat, int kernel[8][8], int x, int y);
int** walsh(int** pixelmat, int mheight, int mwidth);
void haar1D(double* data, int size);
void haar2D(double** data, int mheight, int mwidth);
int** haar(int** pixelmat, int mheight, int mwidth);

#endif