#ifndef CODING_H
#define CODING_H

#include "utils.h"
#include "fft.h"

void DCTBlockDecoding(double** result);
int** DCTCoding(int** pixelmat, int mheight, int mwidth);

#endif
