#ifndef PARAMETER_H
#define PARAMETER_H

#include "utils.h"

int maxValue(int** pixelmat, int mheight, int mwidth);
int minValue(int** pixelmat, int mheight, int mwidth);
float avgValue(int** pixelmat, int mheight, int mwidth);
float varValue(int** pixelmat, int mheight, int mwidth);
int* histogram(int** pixelmat, int mheight, int mwidth);

#endif