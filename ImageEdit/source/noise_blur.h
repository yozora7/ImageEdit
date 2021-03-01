#ifndef NOISE_BLUR_H
#define NOISE_BLUR_H

#include "utils.h"

// add noise
int** randomNoise(int** pixelmat, int mheight, int mwidth);
int** impulseNoise(int** pixelmat, int mheight, int mwidth);
// motion blur
int** motionBlur(int** pixelmat, int mheight, int mwidth);

#endif
