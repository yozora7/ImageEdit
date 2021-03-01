#include "noise_blur.h"

using namespace std;
//
// 图像加噪
//
// 生成随机噪声
int** randomNoise(int** pixelmat, int mheight, int mwidth) {
    // 生成正态分布随机数
    std::random_device randint;
    std::mt19937 randgen(randint());
    std::normal_distribution<> normdistrib(0, 15);
    // 遍历各像素点加噪
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mheight; j++) {
            double value = double(pixelmat[i][j]) + normdistrib(randgen);
            // 确保灰度值不越界
            if (value > 255) {
                pixelmat[i][j] = 255;
            }
            else if (value < 0) {
                pixelmat[i][j] = 0;
            }
            else {
                pixelmat[i][j] = int(value);
            }
        }
    }
    return pixelmat;
}

// 生成椒盐噪声
int** impulseNoise(int** pixelmat, int mheight, int mwidth) {
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            // 随机将像素点灰度值变为0或255, 或保留原值
            if (!(rand() % 20)) {
                if (rand() % 2) {
                    pixelmat[i][j] = 0;
                }
                else {
                    pixelmat[i][j] = 255;
                }
            }
        }
    }
    return pixelmat;
}
//
// 图像运动模糊
//
//运动模糊图像(空间域处理)
int** motionBlur(int** pixelmat, int mheight, int mwidth) {
    int T = 6;
    double** result = new double* [mheight];
    for (int i = 0; i < mheight; i++) {
        result[i] = new double[mwidth + T];
    }
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth + T; j++) {
            if (j < T) {
                result[i][j] = pixelmat[i][j];
            }
            else {
                double sum = 0;
                for (int t = 0; t < T; t++) {
                    if (j - t < mwidth) {
                        sum += pixelmat[i][j - t] / T;
                    }
                }
                result[i][j] = sum;
            }
        }
    }
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = (int)result[i][j + T];
        }
    }
    for (int i = 0; i < mheight; i++) {
        delete[] result[i];
    }
    delete[] result;
    return pixelmat;
}

