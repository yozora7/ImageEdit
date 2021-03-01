#ifndef ENHANCE_H
#define ENHANCE_H

#include "utils.h"
#include "parameter.h"
#include "fft.h"

// contrast enhancement
int** threshold(int** pixelmat, int mheight, int mwidth);
int** negative(int** pixelmat, int mheight, int mwidth);
int** logTran(int** pixelmat, int mheight, int mwidth);
int** histEqual(int** pixelmat, int mheight, int mwidth);
int** grayStretch(int** pixelmat, int mheight, int mwidth);
// low-pass filtering
int** midFit(int** pixelmat, int mheight, int mwidth);
int** avgFit(int** pixelmat, int mheight, int mwidth);
double** lpFilter(int size, double cutoff);
int** lpFit(int** pixelmat, int mheight, int mwidth);
double** butterworth(int size, int n, double cutoff);
int** butterworthLp(int** pixelmat, int mheight, int mwidth);
// high-pass filtering
int** sobel(int** pixelmat, int mheight, int mwidth);
int** laplace(int** pixelmat, int mheight, int mwidth);
double** hpFilter(int size, double cutoff);
int** hpFit(int** pixelmat, int mheight, int mwidth);
// image restoration
Complex** motionDegrad(int msize);
int** invertFit(int** pixelmat, int mheight, int mwidth);
int** wienerFit(int** pixelmat, int mheight, int mwidth);

#endif