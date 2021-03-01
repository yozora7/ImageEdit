#include "transform.h"

using namespace std;

//
// 几何变换
//
// 将图像平移到显示区域的中心
int** centralize(int** framemat, int** pixelmat, int mheight, int mwidth) {
    // framemat为指向显示区域(画板)的二维数组指针
    int xpt = (FRAME_HEIGHT - mheight) / 2;
    int ypt = (FRAME_WIDTH - mwidth) / 2;
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            framemat[i + xpt][j + ypt] = pixelmat[i][j];
        }
    }
    return framemat;
}

// 旋转图像
int** rotate(int** framemat, int** pixelmat, int mheight, int mwidth) {
    int x0 = FRAME_HEIGHT / 2 - 1, y0 = FRAME_WIDTH / 2 - 1;    // 画板中心
    // 旋转角
    double theta = -PI / 4;
    for (int i = 0; i < FRAME_HEIGHT; i++) {
        for (int j = 0; j < FRAME_WIDTH; j++) {
            // 遍历画板, 寻找原图像对应坐标
            int srcx = (i - x0) * cos(theta) - (j - y0) * sin(theta);
            int srcy = (i - x0) * sin(theta) + (j - y0) * cos(theta);
            // 若(srcx,srcy)在图像中, 在画板对应位置赋予灰度值
            if (inImage(pixelmat, mheight, mwidth, srcx, srcy)) {
                framemat[i][j] = pixelmat[srcx][srcy];
            }
            else {
                framemat[i][j] = 255;
            }
        }
    }
    return framemat;
}

//
// 线性变换
//
// DFT变换, 返回处理后的图像, 注意缩放到0~255的整型 (返回傅里叶谱)
int** DFT(int** pixelmat, int mheight, int mwidth) {
    int msize = mheight * 2;
    int** paddle = new int* [msize];    // 图像全零填充
    for (int i = 0; i < msize; i++) {
        paddle[i] = new int[msize];
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
    int** spectra = new int* [mheight];         // 傅里叶谱
    double** temp = new double* [mheight];      // 临时空间
    double maxv = 0, minv = 255;                // 灰度最大/小值
    for (int i = 0; i < mheight; i++) {
        temp[i] = new double[mwidth];
        spectra[i] = new int[mwidth];
        memset(temp[i], 0, sizeof(double) * mwidth);
        memset(spectra[i], 0, sizeof(int) * mwidth);
    }
    // 裁剪DFT结果
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            // 裁剪保留中间部分
            temp[i][j] =
                sqrt(pow(dftmat[i + mheight / 2][j + mwidth / 2].real, 2) +
                    pow(dftmat[i + mheight / 2][j + mwidth / 2].imag, 2));
            // 寻找最大值
            if (temp[i][j] > maxv) {
                maxv = temp[i][j];
            }
            // 寻找最小值
            if (temp[i][j] < minv) {
                minv = temp[i][j];
            }
        }
    }
    // 归一化
    double range = maxv - minv;
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            spectra[i][j] = int((temp[i][j] - minv) * 255 / range);
        }
    }
    for (int i = 0; i < mheight; i++) {
        delete[] temp[i];
        delete[] dftmat[i];
        delete[] pixelmat[i];
        delete[] paddle[i];
        delete[] paddle[i + mheight];
    }
    delete[] temp;
    delete[] dftmat;
    delete[] pixelmat;
    delete[] paddle;
    // log变换
    int c = 45; // c为可调节系数
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            spectra[i][j] = int(c * log(1 + (double)spectra[i][j]));
            //防止灰度值溢出
            if (spectra[i][j] > 255) {
                spectra[i][j] = 255;
            }
        }
    }
    return spectra; // 返回傅里叶谱
}

// DCT变换
int** DCT(int** pixelmat, int mheight, int mwidth) {
    double minv = 255, maxv = 0;    // 灰度最大/小值
    // 遍历图像中的不重合8*8区域
    for (int i = 0; i < mheight / 8; i++) {
        for (int j = 0; j < mwidth / 8; j++) {
            double** result = DCTBlock(pixelmat, i * 8, j * 8); // 对8*8区域做DCT变换
            // 寻找灰度最大/小值
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (minv > result[x][y]) {
                        minv = result[x][y];
                    }
                    if (maxv < result[x][y]) {
                        maxv = result[x][y];
                    }
                }
            }
            // 归一化
            double range = maxv - minv;
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    pixelmat[i * 8 + x][j * 8 + y] =
                        int((result[x][y] - minv) / range * 255);
                }
            }
            for (int x = 0; x < 8; x++) {
                delete[] result[x];
            }
            delete[] result;
        }
    }
    return pixelmat;
}

// 对8*8区域做walsh变换 (kernel: 变换核)
double** walshBlock(int** pixelmat, int kernel[8][8], int x, int y) {
    int** block = getBlock(pixelmat, x, y); // 取出图像中的8*8区域, 左上角坐标为(x,y)
    double** result = new double* [8];
    for (int i = 0; i < 8; i++) {
        result[i] = new double[8];
        memset(result[i], 0, sizeof(double) * 8);
    }
    for (int u = 0; u < 8; u++) {
        for (int v = 0; v < 8; v++) {
            for (int i = 0; i < 8; i++) {
                result[u][v] += kernel[u][i] * block[i][v];
            }
        }
    }
    for (int u = 0; u < 8; u++) {
        for (int v = 0; v < 8; v++) {
            double sum = 0;
            for (int i = 0; i < 8; i++) {
                sum += result[u][i] * kernel[i][v];
            }
            result[u][v] = sum / 8;
        }
    }
    for (int i = 0; i < 8; i++) {
        delete[] block[i];
    }
    delete[] block;
    return result;
}

// walsh变换, 返回处理后的图像
int** walsh(int** pixelmat, int mheight, int mwidth) {
    double minv = 255, maxv = 0;    // 灰度最大/小值
    // walsh变换核
    int kernel[8][8] = {
        {1, 1, 1, 1, 1, 1, 1, 1},     {1, 1, 1, 1, -1, -1, -1, -1},
        {1, 1, -1, -1, -1, -1, 1, 1}, {1, 1, -1, -1, 1, 1, -1, -1},
        {1, -1, -1, 1, 1, -1, -1, 1}, {1, -1, -1, 1, -1, 1, 1, -1},
        {1, -1, 1, -1, -1, 1, -1, 1}, {1, -1, 1, -1, 1, -1, 1, -1} };
    // 遍历图像中的不重合8*8区域
    for (int i = 0; i < mheight / 8; i++) {
        for (int j = 0; j < mwidth / 8; j++) {
            double** result = walshBlock(pixelmat, kernel, i * 8, j * 8);   // 对8*8区域做walsh变换
            //寻找灰度最大/小值
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (minv > result[x][y]) {
                        minv = result[x][y];
                    }
                    if (maxv < result[x][y]) {
                        maxv = result[x][y];
                    }
                }
            }
            // 归一化
            double range = maxv - minv;
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    pixelmat[i * 8 + x][j * 8 + y] =
                        int((result[x][y] - minv) / range * 255);
                }
            }
            for (int x = 0; x < 8; x++) {
                delete[] result[x];
            }
            delete[] result;
        }
    }
    return pixelmat;
}

// 一维haar变换
void haar1D(double* data, int size) {
    double norm = 1.0 / sqrt(size); // 归一化常数
    int threshold = 64; // 控制haar变换的阶数, 即左上角保留原图的尺寸
    for (int i = 0; i < size; i++) {
        data[i] *= norm;
    }
    double* temp = new double[size];
    while (size > threshold) {
        size /= 2;
        for (int i = 0; i < size; i++) {
            temp[i] = (data[2 * i] + data[2 * i + 1]) / sqrt(2.0);
            temp[size + i] = (data[2 * i] - data[2 * i + 1]) / sqrt(2.0);
        }
        memcpy(data, temp, sizeof(double) * size * 2);
    }
    delete[] temp;
}

// 二维haar变换 (mheight == mwidth)
void haar2D(double** data, int mheight, int mwidth) {
    for (int i = 0; i < mheight; i++) {
        haar1D(data[i], mwidth);    // 对各行做变换
    }
    transpose(data, mheight);
    for (int i = 0; i < mwidth; i++) {
        haar1D(data[i], mheight);   // 转置后对各行做变换
    }
    transpose(data, mheight);   // 转置
}

// haar变换
int** haar(int** pixelmat, int mheight, int mwidth) {
    double minv = 255, maxv = 0;    // 灰度最大/小值
    // 将图像灰度值转为double类型, 暂存于result中
    double** result = new double* [mheight];
    for (int i = 0; i < mheight; i++) {
        result[i] = new double[mwidth];
        for (int j = 0; j < mwidth; j++) {
            result[i][j] = double(pixelmat[i][j]);
        }
    }
    haar2D(result, mheight, mwidth);    // 二维haar变换
    // 寻找灰度最大/小值
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            if (minv > result[i][j]) {
                minv = result[i][j];
            }
            if (maxv < result[i][j]) {
                maxv = result[i][j];
            }
        }
    }
    // 归一化
    double range = maxv - minv;
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = int((result[i][j] - minv) / range * 255);
        }
    }
    for (int i = 0; i < mheight; i++) {
        delete[] result[i];
    }
    delete[] result;
    return pixelmat;
}