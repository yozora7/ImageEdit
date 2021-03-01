#ifndef FFT_H
#define FFT_H

#include "utils.h"

// FFT - 1D
void FFTReal(double* signal, double* realf, double* imagf, int size, int n);
void FFTComplex(double* re_signal, double* im_signal, double* realf, double* imagf, int size, int n);
void IFFT(double* re_signal, double* im_signal, double* realf, double* imagf, int size, int n);
// FFT - 2D
Complex** FFT2D(int** pixelmat, int msize);
int** IFFT2D(Complex** dftmat, int msize);
double** DCTBlock(int** pixelmat, int x, int y);

#endif
