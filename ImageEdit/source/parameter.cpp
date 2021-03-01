#include "parameter.h"

using namespace std;

// 灰度最大值
int maxValue(int** pixelmat, int mheight, int mwidth) {
    int maxv = pixelmat[0][0];
    for (int i = 0; i < mheight; i++)
        for (int j = 0; j < mwidth; j++)
            if (pixelmat[i][j] > maxv)
                maxv = pixelmat[i][j];
    return maxv;
}

// 灰度最小值
int minValue(int** pixelmat, int mheight, int mwidth) {
    int minv = pixelmat[0][0];
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            if (pixelmat[i][j] < minv) {
                minv = pixelmat[i][j];
            }
        }
    }
    return minv;
}

// 灰度平均值
float avgValue(int** pixelmat, int mheight, int mwidth) {
    long long sum = 0;
    int count = mheight * mwidth;
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            sum += pixelmat[i][j];
        }
    }
    return (float)(sum) / count;
}

// 灰度方差
float varValue(int** pixelmat, int mheight, int mwidth) {
    long long sumquad = 0;
    int count = mheight * mwidth;
    float avg = avgValue(pixelmat, mheight, mwidth);
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            sumquad += pixelmat[i][j] * pixelmat[i][j];
        }
    }
    return (float)(sumquad) / count - avg * avg;
}

// 直方图
int* histogram(int** pixelmat, int mheight, int mwidth) {
    int* hist_array = new int[256];
    memset(hist_array, 0, sizeof(int) * 256);
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            hist_array[pixelmat[i][j]]++;
        }
    }
    return hist_array;
}