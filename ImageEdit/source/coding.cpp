#include "coding.h"

using namespace std;

// DCT块变换解码(8*8)
void DCTBlockDecoding(double** result) {
    double argu, argv;
    // 存储DCT变换的所有幅值
    vector<double> list;
    double temp[8][8];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            list.push_back(abs(result[i][j]));
        }
    }
    // 将幅值从小到大排序
    sort(list.begin(), list.end());
    // 截断幅值较小的50%
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (abs(result[i][j]) < list[31]) {
                result[i][j] = 0;
            }
            temp[i][j] = result[i][j];
        }
    }
    // 反变换
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            double sum = 0;
            for (int u = 0; u < 8; u++) {
                // 确定反变换系数
                if (u == 0) {
                    argu = sqrt(1.0 / 8);
                }
                else {
                    argu = sqrt(2.0 / 8);
                }
                for (int v = 0; v < 8; v++) {
                    // 确定反变换系数
                    if (v == 0) {
                        argv = sqrt(1.0 / 8);
                    }
                    else {
                        argv = sqrt(2.0 / 8);
                    }
                    // 反变换
                    sum += argu * argv * temp[u][v] *
                        cos((2 * i + 1) * u * PI / 16) *
                        cos((2 * j + 1) * v * PI / 16);
                }
            }
            result[i][j] = sum;
        }
    }
}

// DCT块变换编解码(用于验证DCT变换正确性)
int** DCTCoding(int** pixelmat, int mheight, int mwidth) {
    //灰度最大/小值
    double minv = 255, maxv = 0;
    //遍历图像中的不重合8*8区域
    for (int i = 0; i < mheight / 8; i++) {
        for (int j = 0; j < mwidth / 8; j++) {
            //对8*8区域做正变换
            double** result = DCTBlock(pixelmat, i * 8, j * 8);
            //截断后对8*8区域做反变换
            DCTBlockDecoding(result);
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
            //归一化
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