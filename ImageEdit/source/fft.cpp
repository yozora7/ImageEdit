#include "fft.h"

using namespace std;

//
// 一维FFT/IFFT
//
// 一维实数信号FFT
void FFTReal(double* signal, double* realf, double* imagf, int size, int n) {
    // signal[]: 实信号； realf[], imagf[]: 频域实虚部； size: 信号长度； n:
    // 蝶形运算位移 size == 1 到达递归终点
    if (size == 1) {
        realf[0] = signal[0];
        imagf[0] = 0.0;
        return;
    }
    // 分而治之
    int mid = size / 2;
    FFTReal(signal, realf, imagf, mid, 2 * n);
    FFTReal(signal + n, realf + mid, imagf + mid, mid, 2 * n);
    // 蝶形运算
    for (int i = 0; i < mid; ++i) {
        double r1 = realf[i];
        double i1 = imagf[i];
        double r2 = realf[i + mid];
        double i2 = imagf[i + mid];
        double alpha = -2.0 * PI * (double)(i) / (double)(size);
        double r3 = cos(alpha);
        double i3 = sin(alpha);
        double r4 = r2 * r3 - i2 * i3;
        double i4 = r3 * i2 + r2 * i3;
        realf[i] = r1 + r4;
        imagf[i] = i1 + i4;
        realf[i + mid] = r1 - r4;
        imagf[i + mid] = i1 - i4;
    }
}

// 一维复数信号FFT
void FFTComplex(double* re_signal, double* im_signal, double* realf,
    double* imagf, int size, int n) {
    // re_signal[], im_signal[]: 信号实虚部； realf[], imagf[]: 频域实虚部；
    // size: 信号长度； n: 蝶形运算位移 size == 1 到达递归终点
    if (size == 1) {
        realf[0] = re_signal[0];
        imagf[0] = im_signal[0];
        return;
    }
    // 分而治之
    int mid = size / 2;
    FFTComplex(re_signal, im_signal, realf, imagf, mid, 2 * n);
    FFTComplex(re_signal + n, im_signal + n, realf + mid, imagf + mid, mid,
        2 * n);
    // 蝶形运算
    for (int k = 0; k < mid; ++k) {
        double r1 = realf[k];
        double i1 = imagf[k];
        double r2 = realf[k + mid];
        double i2 = imagf[k + mid];
        double alpha = -2.0 * PI * (double)(k) / (double)(size);
        double r3 = cos(alpha);
        double i3 = sin(alpha);
        double r4 = r2 * r3 - i2 * i3;
        double i4 = r3 * i2 + r2 * i3;
        realf[k] = r1 + r4;
        imagf[k] = i1 + i4;
        realf[k + mid] = r1 - r4;
        imagf[k + mid] = i1 - i4;
    }
}

//一维复数信号IFFT
void IFFT(double* re_signal, double* im_signal, double* realf, double* imagf,
    int size, int n) {
    // re_signal[], im_signal[]: 信号实虚部； realf[], imagf[]: 频域实虚部；
    // size: 信号长度； n: 蝶形运算位移 size == 1 到达递归终点
    if (size == 1) {
        realf[0] = re_signal[0];
        imagf[0] = im_signal[0];
        return;
    }
    int mid = size / 2;
    //分而治之
    IFFT(re_signal, im_signal, realf, imagf, mid, 2 * n);
    IFFT(re_signal + n, im_signal + n, realf + mid, imagf + mid, mid, 2 * n);
    //蝶形运算
    for (int i = 0; i < mid; ++i) {
        double r1 = realf[i];
        double i1 = imagf[i];
        double r2 = realf[i + mid];
        double i2 = imagf[i + mid];
        double alpha = 2.0 * PI * i / (double)(size);
        double r3 = cos(alpha);
        double i3 = sin(alpha);
        double r4 = r2 * r3 - i2 * i3;
        double i4 = r3 * i2 + r2 * i3;
        realf[i] = r1 + r4;
        imagf[i] = i1 + i4;
        realf[i + mid] = r1 - r4;
        imagf[i + mid] = i1 - i4;
    }
}

//
// 二维FFT/IFFT
//
// 二维FFT
Complex** FFT2D(int** pixelmat, int msize) {
    Complex** result = new Complex * [msize];   // 最终结果(复数)
    double** data = new double* [msize];        // 原始图像
    double** realf = new double* [msize];       // DFT(实部)
    double** imagf = new double* [msize];       // DFT(虚部)
    double** re_temp = new double* [msize];     // 临时数组(实部)
    double** im_temp = new double* [msize];     // 临时数组(虚部)
    for (int i = 0; i < msize; ++i) {
        result[i] = new Complex[msize];
        data[i] = new double[msize];
        realf[i] = new double[msize];
        imagf[i] = new double[msize];
        re_temp[i] = new double[msize];
        im_temp[i] = new double[msize];
    }
    // 将原图复制到data中处理
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            data[i][j] = pixelmat[i][j];
        }
    }
    // 对各行做FFT
    for (int u = 0; u < msize; ++u) {
        FFTReal(data[u], re_temp[u], im_temp[u], msize, 1);
    }
    // 行列变换后重复先前步骤
    double* _realf = new double[msize];    // DFT(实部)
    double* _imagf = new double[msize];    // DFT(虚部)
    double* _re_temp = new double[msize];  // 临时数组(实部)
    double* _im_temp = new double[msize];  // 临时数组(虚部)
    for (int j = 0; j < msize; ++j) {
        // 行列变换
        for (int i = 0; i < msize; ++i) {
            _re_temp[i] = re_temp[i][j];
            _im_temp[i] = im_temp[i][j];
        }
        // 对新的各行做FFT
        FFTComplex(_re_temp, _im_temp, _realf, _imagf, msize, 1);
        // FFT结果
        for (int i = 0; i < msize; ++i) {
            realf[i][j] = _realf[i];
            imagf[i][j] = _imagf[i];
        }
    }
    // 整理结果
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < msize; j++) {
            result[i][j].real = realf[i][j];
            result[i][j].imag = imagf[i][j];
        }
    }
    for (int i = 0; i < msize; ++i) {
        delete[] data[i];
        delete[] realf[i];
        delete[] imagf[i];
        delete[] re_temp[i];
        delete[] im_temp[i];
    }
    delete[] data;
    delete[] realf;
    delete[] imagf;
    delete[] re_temp;
    delete[] im_temp;
    delete[] _realf;
    delete[] _imagf;
    delete[] _re_temp;
    delete[] _im_temp;
    return result;
}

// 二维IFFT
int** IFFT2D(Complex** dftmat, int msize) {
    double norm = 1.0 / (msize * msize);    // 归一化常数
    int** result = new int* [msize];        // 最终结果
    double** data = new double* [msize];    // 原始图像
    double** realf = new double* [msize];   // DFT(实部)
    double** imagf = new double* [msize];   // DFT(虚部)
    double** re_temp = new double* [msize]; // 临时数组(实部)
    double** im_temp = new double* [msize]; // 临时数组(虚部)
    for (int u = 0; u < msize; ++u) {
        result[u] = new int[msize];
        data[u] = new double[msize];
        realf[u] = new double[msize];
        imagf[u] = new double[msize];
        re_temp[u] = new double[msize];
        im_temp[u] = new double[msize];
    }
    // 将DFT结果复制到realf[], imagf[]中处理
    for (int u = 0; u < msize; u++) {
        for (int v = 0; v < msize; v++) {
            realf[u][v] = dftmat[u][v].real;
            imagf[u][v] = dftmat[u][v].imag;
        }
    }
    // 对各行做IFFT
    for (int x = 0; x < msize; ++x) {
        IFFT(realf[x], imagf[x], re_temp[x], im_temp[x], msize, 1);
    }
    // 行列变换后重复先前步骤
    double* _re_save = new double[msize];  // 反变换实部
    double* _im_save = new double[msize];  // 反变换虚部
    double* _re_temp = new double[msize];  // 临时数组(实部)
    double* _im_temp = new double[msize];  // 临时数组(虚部)
    for (int j = 0; j < msize; ++j) {
        // 行列变换
        for (int x = 0; x < msize; ++x) {
            _re_temp[x] = re_temp[x][j];
            _im_temp[x] = im_temp[x][j];
        }
        // 对新的各行做IFFT
        IFFT(_re_temp, _im_temp, _re_save, _im_save, msize, 1);
        // 只保留实部
        for (int i = 0; i < msize; ++i) {
            result[i][j] = _re_save[i] * norm;
        }
    }
    for (int i = 0; i < msize; ++i) {
        delete[] data[i];
        delete[] realf[i];
        delete[] imagf[i];
        delete[] re_temp[i];
        delete[] im_temp[i];
    }
    delete[] data;
    delete[] realf;
    delete[] imagf;
    delete[] re_temp;
    delete[] im_temp;
    delete[] _re_temp;
    delete[] _im_temp;
    delete[] _re_save;
    delete[] _im_save;
    return result;
}

// 对8*8区域做DCT变换
double** DCTBlock(int** pixelmat, int x, int y) {
    int** block = getBlock(pixelmat, x, y); // 左上角坐标为(x,y)
    double argu, argv;  // DCT变换系数
    double** result = new double* [8];
    for (int i = 0; i < 8; i++) {
        result[i] = new double[8];
        memset(result[i], 0, sizeof(double) * 8);
    }
    // 正变换
    for (int u = 0; u < 8; u++) {
        if (u == 0) {
            argu = sqrt(1.0 / 8);
        }
        else {
            argu = sqrt(2.0 / 8);
        }
        for (int v = 0; v < 8; v++) {
            if (v == 0) {
                argv = sqrt(1.0 / 8);
            }
            else {
                argv = sqrt(2.0 / 8);
            }
            double sum = 0;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    sum += block[i][j] * cos((2 * i + 1) * u * PI / 16) *
                        cos((2 * j + 1) * v * PI / 16);
                }
            }
            result[u][v] = argu * argv * sum;
        }
    }
    for (int i = 0; i < 8; i++) {
        delete[] block[i];
    }
    delete[] block;
    return result;
}