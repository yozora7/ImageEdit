#include "enhance.h"

using namespace std;

//
// 图像对比度增强
//
// 将灰度图转化为二值图像
int** threshold(int** pixelmat, int mheight, int mwidth) {
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = pixelmat[i][j] > 128 ? 255 : 0;
        }
    }
    return pixelmat;
}

// 负片
int** negative(int** pixelmat, int mheight, int mwidth) {
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = 255 - pixelmat[i][j];
        }
    }
    return pixelmat;
}

// log变换
int** logTran(int** pixelmat, int mheight, int mwidth) {
    // c为可调节系数
    int c = 45;
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = int(c * log(1 + (double)pixelmat[i][j]));
            if (pixelmat[i][j] > 255) {
                pixelmat[i][j] = 255;   // 防止溢出
            }
        }
    }
    return pixelmat;
}

// 直方图均衡
int** histEqual(int** pixelmat, int mheight, int mwidth) {
    
    int* hist_array = histogram(pixelmat, mheight, mwidth); // 直方图数组
    if (hist_array == NULL) {
        return NULL;
    }
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            int sum = 0;
            for (int k = 0; k < pixelmat[i][j]; k++) {
                sum += hist_array[k];
            }
            pixelmat[i][j] = int(sum / double(mheight * mwidth) * 255);
        }
    }
    return pixelmat;
}

// 灰度拉伸
int** grayStretch(int** pixelmat, int mheight, int mwidth) {
    int maxv = maxValue(pixelmat, mheight, mwidth);
    int minv = minValue(pixelmat, mheight, mwidth);
    int range = maxv - minv;
    if (range == 0) {
        return pixelmat;    // 极端情况(全黑图)
    }
    else {
        for (int i = 0; i < mheight; i++) {
            for (int j = 0; j < mwidth; j++) {
                pixelmat[i][j] =
                    pixelmat[i][j] * 255 / range - 255 * minv / range;
            }
        }
    }
    return pixelmat;
}
//
// 图像平滑
//
// 中值滤波
int** midFit(int** pixelmat, int mheight, int mwidth) {
    int** temp = new int* [mheight];
    for (int i = 0; i < mheight; i++) {
        temp[i] = new int[mwidth];
        memcpy(temp[i], pixelmat[i], sizeof(int) * mwidth);
    }
    // 遍历像素
    for (int i = 1; i < mheight - 1; i++) {
        for (int j = 1; j < mwidth - 1; j++) {
            int* near = nearby(temp, i, j);  // 获得像素3*3邻域
            quickSort(near, 0, 8);           // 对邻域灰度值进行排序
            pixelmat[i][j] = near[4];        // 取中值
            delete[] near;
        }
    }
    for (int i = 0; i < mheight; i++) {
        delete[] temp[i];
    }
    delete[] temp;
    return pixelmat;
}

// 均值滤波
int** avgFit(int** pixelmat, int mheight, int mwidth) {
    int** temp = new int* [mheight];
    for (int i = 0; i < mheight; i++) {
        temp[i] = new int[mwidth];
        memcpy(temp[i], pixelmat[i], sizeof(int) * mwidth);
    }
    // 遍历像素
    for (int i = 1; i < mheight - 1; i++) {
        for (int j = 1; j < mwidth - 1; j++) {
            int sum = 0;
            int* near = nearby(temp, i, j); // 获得像素3*3邻域
            for (int k = 0; k < 9; k++) {
                sum += near[k];             //邻域求和
            }
            pixelmat[i][j] = sum / 9;       //取均值
            delete[] near;
        }
    }
    for (int i = 0; i < mheight; i++) {
        delete[] temp[i];
    }
    delete[] temp;
    return pixelmat;
}

// 理想低通滤波器(频域)
double** lpFilter(int size, double cutoff) {
    double** filter = new double* [size];
    for (int i = 0; i < size; i++) {
        filter[i] = new double[size];
    }
    int c_u = size / 2;    // 中心坐标
    int c_v = size / 2;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {     
            double dist = distance(i, j, c_u, c_v); // D(i,j)
            if (dist <= cutoff) {
                filter[i][j] = 1;
            }
            else {
                filter[i][j] = 0;   // 滤去高频分量
            }
        }
    }
    //返回滤波器
    return filter;
}

// 理想低通滤波
int** lpFit(int** pixelmat, int mheight, int mwidth) {
    int msize = mheight * 2;
    double** filter = lpFilter(msize, 120);
    int** paddle = new int* [msize];
    for (int i = 0; i < msize; i++) {
        paddle[i] = new int[msize]; // 全零填充
    }
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            if (i < mheight && j < mwidth) {
                paddle[i][j] = pixelmat[i][j] * pow(-1, i + j); //将图像移动到变换中心
            }
            else {
                paddle[i][j] = 0;   // 其余部分全零填充
            }
        }
    }
    Complex** dftmat = FFT2D(paddle, msize);    // DFT-2D
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            dftmat[i][j].real *= filter[i][j];  // 频域与滤波器相乘
            dftmat[i][j].imag *= filter[i][j];
        }
    }
    for (int i = 0; i < msize; i++) {
        delete[] filter[i];
    }
    for (int i = 0; i < msize; i++) {
        delete[] paddle[i];
    }
    delete[] filter;
    delete[] paddle;
    paddle = IFFT2D(dftmat, msize); // IFFT-2D
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = paddle[i][j] * pow(-1, i + j); // 去掉填充部分
        }
    }
    normalize(pixelmat, mheight, mwidth);       // 灰度值归一化
    for (int i = 0; i < msize; i++) {
        delete[] paddle[i];
        delete[] dftmat[i];
    }
    delete[] paddle;
    delete[] dftmat;
    return pixelmat;
}

// butterworth低通滤波器
double** butterworth(int size, int n, double cutoff) {
    double** filter = new double* [size];
    for (int i = 0; i < size; i++) {
        filter[i] = new double[size];
    }
    int c_u = size / 2;    // 中心坐标
    int c_v = size / 2;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double dist = distance(i, j, c_u, c_v);   // D(i,j)
            filter[i][j] = 1.0 / (1 + pow(dist / cutoff, 2 * n));   // 滤去高频分量
        }
    }
    //返回滤波器
    return filter;
}

// butterworth低通滤波
int** butterworthLp(int** pixelmat, int mheight, int mwidth) {
    int msize = mheight * 2;
    double** filter = butterworth(msize, 10, 60);
    int** paddle = new int* [msize];
    for (int i = 0; i < msize; i++) {
        paddle[i] = new int[msize];     // 图像全零填充
    }
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            
            if (i < mheight && j < mwidth) {
                paddle[i][j] = pixelmat[i][j] * pow(-1, i + j); // 将图像移动到变换中心
            }
            else {
                paddle[i][j] = 0;    // 其余部分全零填充
            }
        }
    }
    Complex** dftmat = FFT2D(paddle, msize);    // DFT-2D
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            dftmat[i][j].real *= filter[i][j];  // 频域与滤波器相乘
            dftmat[i][j].imag *= filter[i][j];
        }
    }
    for (int i = 0; i < msize; i++) {
        delete[] filter[i];
    }
    for (int i = 0; i < msize; i++) {
        delete[] paddle[i];
    }
    delete[] filter;
    delete[] paddle;
    paddle = IFFT2D(dftmat, msize); // IFFT-2D
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = paddle[i][j];  // 去掉填充部分
        }
    }
    for (int i = 0; i < msize; i++) {
        delete[] paddle[i];
        delete[] dftmat[i];
    }
    delete[] paddle;
    delete[] dftmat;
    return pixelmat;
}
//
// 图像锐化
//
// sobel算子
int** sobel(int** pixelmat, int mheight, int mwidth) {
    // sobel算子
    int mask[2][9] = { {-1, 0, 1, -2, 0, 2, -1, 0, 1},
                      {-1, -2, -1, 0, 0, 0, 1, 2, 1} };
    int** temp = new int* [mheight];
    for (int i = 0; i < mheight; i++) {
        temp[i] = new int[mwidth];
        memcpy(temp[i], pixelmat[i], sizeof(int) * mwidth);
    }
    // sobel锐化
    for (int i = 1; i < mheight - 1; i++) {
        for (int j = 1; j < mwidth - 1; j++) {
            int* near = nearby(temp, i, j); // 获取像素3*3邻域
            // 空域滤波
            int gx = 0, gy = 0;
            for (int k = 0; k < 9; k++) {
                gx += near[k] * mask[0][k];
                gy += near[k] * mask[1][k];
            }
            pixelmat[i][j] = (int)sqrt(gx * gx + gy * gy);
            // 防止灰度越界
            if (pixelmat[i][j] > 255) {
                pixelmat[i][j] = 255;
            }
            if (pixelmat[i][j] < 0) {
                pixelmat[i][j] = 0;
            }
            delete[] near;
        }
    }
    delete[] temp;
    return pixelmat;
}

// laplace锐化
int** laplace(int** pixelmat, int mheight, int mwidth) {
    int mask[9] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };    // laplace算子
    int** temp = new int* [mheight];
    for (int i = 0; i < mheight; i++) {
        temp[i] = new int[mwidth];
        memcpy(temp[i], pixelmat[i], sizeof(int) * mwidth);
    }
    for (int i = 1; i < mheight - 1; i++) {
        for (int j = 1; j < mwidth - 1; j++) {
            int* near = nearby(temp, i, j); // 获取像素3*3邻域
            // 空域滤波
            for (int k = 0; k < 9; k++) {
                pixelmat[i][j] += near[k] * mask[k];
                // 防止灰度越界
                if (pixelmat[i][j] > 255) {
                    pixelmat[i][j] = 255;
                }
                if (pixelmat[i][j] < 0) {
                    pixelmat[i][j] = 0;
                }
            }
            delete[] near;
        }
    }
    delete[] temp;
    return pixelmat;
}

// 理想高通滤波器(频域)
double** hpFilter(int size, double cutoff) {
    double** filter = new double* [size];
    for (int i = 0; i < size; i++) {
        filter[i] = new double[size];
    }
    int c_u = size / 2; // 中心坐标
    int c_v = size / 2;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double dist = distance(i, j, c_u, c_v); // D(i,j)
            if (dist <= cutoff) {
                filter[i][j] = 0;    // 滤去低频分量
            }
            else {
                filter[i][j] = 1;
            }
        }
    }
    return filter;
}

// 理想高通滤波
int** hpFit(int** pixelmat, int mheight, int mwidth) {
    int msize = mheight * 2;
    double** filter = hpFilter(msize, 40);
    int** paddle = new int* [msize];
    for (int i = 0; i < msize; i++) {
        paddle[i] = new int[msize]; // 全零填充
    }
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            if (i < mheight && j < mwidth) {
                paddle[i][j] = pixelmat[i][j] * pow(-1, i + j); // 将图像移动到变换中心
            }
            else {
                paddle[i][j] = 0;   // 其余部分全零填充
            }
        }
    }
    Complex** dftmat = FFT2D(paddle, msize);    // DFT-2D
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            dftmat[i][j].real *= filter[i][j];  // 频域与滤波器相乘
            dftmat[i][j].imag *= filter[i][j];
        }
    }
    for (int i = 0; i < mheight; i++) {
        delete[] filter[i];
    }
    for (int i = 0; i < msize; i++) {
        delete[] paddle[i];
    }
    delete[] filter;
    delete[] paddle;
    paddle = IFFT2D(dftmat, msize); // IFFT-2D
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = paddle[i][j] * pow(-1, i + j); // 去掉填充部分
        }
    }    
    normalize(pixelmat, mheight, mwidth);   // 灰度值归一化
    for (int i = 0; i < msize; i++) {
        delete[] paddle[i];
        delete[] dftmat[i];
    }
    delete[] paddle;
    delete[] dftmat;
    return pixelmat;
}
//
// 图像复原
//
//估计图像退化函数H(u,v)
Complex** motionDegrad(int msize) {
    double a = 0, b = -0.012, T = 1;
    Complex** H = new Complex * [msize];
    for (int u = 0; u < msize; u++) {
        H[u] = new Complex[msize];
        for (int v = 0; v < msize; v++) {
            double n = PI * (u * a + v * b);
            if (n == 0) {
                H[u][v].real = T;
                H[u][v].imag = 0;
                continue;
            }
            H[u][v].real = T * sin(n) * cos(n) / n;
            H[u][v].imag = -T * sin(n) * sin(n) / n;
        }
    }
    return H;
}

// 逆滤波复原
int** invertFit(int** pixelmat, int mheight, int mwidth) {
    int msize = mheight;
    // 频域逆滤波
    Complex** F = new Complex * [msize];
    Complex** H = motionDegrad(msize);
    Complex** G = FFT2D(pixelmat, msize);
    for (int u = 0; u < msize; u++) {
        F[u] = new Complex[msize];
        for (int v = 0; v < msize; v++) {
            double temp = pow(H[u][v].real, 2) + pow(H[u][v].imag, 2);
            if (sqrt(temp) < 0.5) {
                temp = 0.5;
                H[u][v].real = sqrt(0.5);
                H[u][v].imag = 0;
            }
            F[u][v].real =
                (G[u][v].real * H[u][v].real + G[u][v].imag * H[u][v].imag) /
                temp;
            F[u][v].imag =
                (-G[u][v].real * H[u][v].imag + G[u][v].imag * H[u][v].real) /
                temp;
        }
    }
    for (int i = 0; i < msize; i++) {
        delete[] pixelmat[i];
    }
    delete[] pixelmat;
    pixelmat = IFFT2D(F, msize);    // IFFT-2D
    pixelmat = butterworthLp(pixelmat, mheight, mwidth);    // butterworth滤波
    // 防止灰度值越界
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            if (pixelmat[i][j] < 0) {
                pixelmat[i][j] = 0;
            }
            if (pixelmat[i][j] > 255) {
                pixelmat[i][j] = 255;
            }
        }
    }
    for (int i = 0; i < mheight; i++) {
        delete[] F[i];
        delete[] H[i];
        delete[] G[i];
    }
    delete[] F;
    delete[] H;
    delete[] G;
    return pixelmat;
}

// 维纳滤波
int** wienerFit(int** pixelmat, int mheight, int mwidth) {
    int msize = mheight * 2;
    int** paddle = new int* [msize];
    for (int i = 0; i < msize; i++) {
        paddle[i] = new int[msize]; // 图像全零填充
    }
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            if (i < mheight && j < mwidth) {
                paddle[i][j] = pixelmat[i][j] * pow(-1, i + j); // 将图像移动到变换中心
            }
            else {
                paddle[i][j] = 0;   // 其余部分全零填充
            }
        }
    }
    // 频域维纳滤波
    Complex** F = new Complex * [msize];
    Complex** H = motionDegrad(msize);
    Complex** G = FFT2D(paddle, msize);
    double K = 0.05;
    for (int u = 0; u < msize; u++) {
        F[u] = new Complex[msize];
        for (int v = 0; v < msize; v++) {
            double temp = pow(H[u][v].real, 2) + pow(H[u][v].imag, 2);
            F[u][v].real =
                (G[u][v].real * H[u][v].real + G[u][v].imag * H[u][v].imag) /
                (temp + K);
            F[u][v].imag =
                (-G[u][v].real * H[u][v].imag + G[u][v].imag * H[u][v].real) /
                (temp + K);
        }
    }
    for (int i = 0; i < msize; i++) {
        delete[] paddle[i];
    }
    delete[] paddle;
    paddle = IFFT2D(F, msize);  // IFFT-2D
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = paddle[i][j] * pow(-1, i + j); // 去掉填充部分
        }
    }
    normalize(pixelmat, mheight, mwidth);   // 灰度值归一化
    for (int i = 0; i < msize; i++) {
        delete[] F[i];
        delete[] H[i];
        delete[] G[i];
    }
    delete[] F;
    delete[] H;
    delete[] G;
    return pixelmat;
}