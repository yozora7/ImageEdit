#include "utils.h"
#include "parameter.h"
#include "fft.h"

using namespace std;

// D(i,j): 频率域中点(i,j)与频率矩形中心的距离
double distance(int i, int j, int c_u, int c_v) {
    return sqrt((i - c_u) * (i - c_u) + (j - c_v) * (j - c_v));
}

// 取出图像中的8*8区域, 左上角坐标为(x,y)
int** getBlock(int** pixelmat, int x, int y) {
    int** result = new int*[8];
    for (int i = 0; i < 8; i++) {
        result[i] = new int[8];
        for (int j = 0; j < 8; j++) {
            result[i][j] = pixelmat[x + i][y + j];
        }
    }
    return result;
}

// 判断一个点是否在图像中
bool inImage(int** pixelmat, int mheight, int mwidth, int x, int y) {
    return (x >= 0 && x < mheight && y >= 0 && y < mwidth);
}

// 求像素点(i,j)的3*3邻域, 以数组形式存储
int* nearby(int** pixelmat, int i, int j) {
    int* near = new int[9];
    int index = 0;
    for (int p = -1; p <= 1; p++) {
        for (int q = -1; q <= 1; q++) {
            near[index++] = pixelmat[i + p][j + q];
        }
    }
    return near;
}

// 图像灰度值归一化
void normalize(int** pixelmat, int mheight, int mwidth) {
    // 寻找灰度最大/小值
    double maxv = 0, minv = 255;
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            if (maxv < pixelmat[i][j]) {
                maxv = pixelmat[i][j];
            }
            if (minv > pixelmat[i][j]) {
                minv = pixelmat[i][j];
            }
        }
    }
    // 归一化处理
    double range = maxv - minv;
    for (int i = 0; i < mheight; i++) {
        for (int j = 0; j < mwidth; j++) {
            pixelmat[i][j] = int(double(pixelmat[i][j] - minv) * 255 / range);
        }
    }
}

// 一维数组快速排序
void quickSort(int a[], int low, int high) {
    int temp;
    int i = low, j = high;
    if (low < high) {
        temp = a[low];
        while (i < j) {
            // 从右向左扫描, 找到大于temp的关键字
            while (j > i && a[j] >= temp) {
                j--;
            }
            if (i < j) {
                a[i] = a[j];  // 放在temp右边
                i++;          // 右移一位
            }
            // 从左向右扫描, 找到小于temp的关键字
            while (i < j && a[i] < temp) {
                i++;
            }
            if (i < j) {
                a[j] = a[i];  // 放在temp左边
                j--;          // 左移一位
            }
        }
        a[i] = temp;
        // 对temp左右分而治之
        quickSort(a, low, i - 1);
        quickSort(a, low + 1, high);
    }
    return;
}

// 二维double数组转置(mheight == mwidth)
void transpose(double** data, int msize) {
    double** temp = new double*[msize];
    for (int i = 0; i < msize; i++) {
        temp[i] = new double[msize];
    }
    // 矩阵转置
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            temp[j][i] = data[i][j];
        }
    }
    for (int i = 0; i < msize; i++) {
        memcpy(data[i], temp[i], sizeof(double) * msize);
        delete[] temp[i];
    }
    delete[] temp;
}