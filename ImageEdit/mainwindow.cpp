#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "source\parameter.h"
#include "source\enhance.h"
#include "source\transform.h"
#include "source\noise_blur.h"
#include "source\coding.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    winimg = new QImage();

    geometricflag = true;

    QMenu* ParameterMenu = new QMenu(this);
    QAction* MaxValueBtn = ParameterMenu->addAction(QStringLiteral("最大值"));
    QAction* MinValueBtn = ParameterMenu->addAction(QStringLiteral("最小值"));
    QAction* AvgValueBtn = ParameterMenu->addAction(QStringLiteral("平均值"));
    QAction* VarValueBtn = ParameterMenu->addAction(QStringLiteral("方差"));
    QAction* HistogramBtn =
        ParameterMenu->addAction(QStringLiteral("直方图"));

    QMenu* EnhanceMenu = new QMenu(this);
    QMenu* ContrastMenu = EnhanceMenu->addMenu(QStringLiteral("对比度增强"));
    QAction* ThresholdBtn =
        ContrastMenu->addAction(QStringLiteral("二值化(示例)"));
    QAction* NegativeBtn =
        ContrastMenu->addAction(QStringLiteral("图像负片"));
    QAction* LogTranBtn =
        ContrastMenu->addAction(QStringLiteral("log变换"));
    QAction* HistEqualBtn =
        ContrastMenu->addAction(QStringLiteral("直方图均衡"));
    QAction* GraystretchBtn =
        ContrastMenu->addAction(QStringLiteral("灰度拉伸"));
    QMenu* SmoothMenu = EnhanceMenu->addMenu(QStringLiteral("图像平滑"));
    QAction* MidFitBtn = SmoothMenu->addAction(QStringLiteral("中值滤波"));
    QAction* AvgFitBtn = SmoothMenu->addAction(QStringLiteral("均值滤波"));
    QAction* LpFitBtn = SmoothMenu->addAction(QStringLiteral("理想低通滤波"));
    QMenu* SharpenMenu = EnhanceMenu->addMenu(QStringLiteral("图像锐化"));
    QAction* SobelBtn = SharpenMenu->addAction(QStringLiteral("Sobel锐化"));
    QAction* LaplaceBtn = SharpenMenu->addAction(QStringLiteral("Laplace锐化"));
    QAction* HpBtn = SharpenMenu->addAction(QStringLiteral("理想高通滤波"));
    QMenu* RestoreMenu = EnhanceMenu->addMenu(QStringLiteral("图像复原"));
    QAction* InvertFitBtn =
        RestoreMenu->addAction(QStringLiteral("逆滤波复原"));
    QAction* WienerFitBtn =
        RestoreMenu->addAction(QStringLiteral("维纳滤波复原"));

    QMenu* TranMenu = new QMenu(this);
    QMenu* GeometryMenu = TranMenu->addMenu(QStringLiteral("几何变换"));
    QAction* CenterBtn =
        GeometryMenu->addAction(QStringLiteral("平移至中心"));
    QAction* RotateBtn = GeometryMenu->addAction(QStringLiteral("旋转"));
    QMenu* LinearMenu = TranMenu->addMenu(QStringLiteral("线性变换"));
    QAction* DFTBtn = LinearMenu->addAction(QStringLiteral("DFT"));
    QAction* DCTBtn = LinearMenu->addAction(QStringLiteral("DCT"));
    QAction* HaarBtn = LinearMenu->addAction(QStringLiteral("HAAR"));
    QAction* WalshBtn = LinearMenu->addAction(QStringLiteral("WALSH"));

    QMenu* NoiseBlurMenu = new QMenu(this);
    QAction* RandomNoiseBtn =
        NoiseBlurMenu->addAction(QStringLiteral("生成随机噪声"));
    QAction* ImpulseNoiseBtn =
        NoiseBlurMenu->addAction(QStringLiteral("生成椒盐噪声"));
    QAction* MotionBlurBtn =
        NoiseBlurMenu->addAction(QStringLiteral("运动模糊图像"));

    QMenu* codingmenu = new QMenu(this);
    QAction* DCTCodingBtn =
        codingmenu->addAction(QStringLiteral("DCT块变换编解码"));

    ui->ParameterBtn->setMenu(ParameterMenu);
    ui->EnhanceBtn->setMenu(EnhanceMenu);
    ui->TransformBtn->setMenu(TranMenu);
    ui->NoiseBlurBtn->setMenu(NoiseBlurMenu);
    ui->CodingBtn->setMenu(codingmenu);

    connect(ui->OpenImage, SIGNAL(triggered()), this,
        SLOT(on_slotOpenImage_triggered()));
    connect(ui->SaveImage, SIGNAL(triggered()), this,
        SLOT(on_slotSaveImage_triggered()));
    connect(ui->FreshImage, SIGNAL(triggered()), this,
        SLOT(on_slotFreshImage_triggered()));
    connect(ui->ActionExit, SIGNAL(triggered()), this,
        SLOT(on_slotExit_triggered()));
    connect(ui->Actionhomepage, SIGNAL(triggered()), this,
        SLOT(on_slotHomepage_triggered()));
    connect(ui->ActionDemo, SIGNAL(triggered()), this,
        SLOT(on_slotDemo_triggered()));
    connect(ui->ActionInfo, SIGNAL(triggered()), this,
        SLOT(on_slotInfo_triggered()));

    connect(MaxValueBtn, SIGNAL(triggered()), this,
        SLOT(on_slotMaxValueBtn_triggered()));
    connect(MinValueBtn, SIGNAL(triggered()), this,
        SLOT(on_slotMinValueBtn_triggered()));
    connect(AvgValueBtn, SIGNAL(triggered()), this,
        SLOT(on_slotAvgValueBtn_triggered()));
    connect(VarValueBtn, SIGNAL(triggered()), this,
        SLOT(on_slotVarValueBtn_triggered()));

    connect(HistogramBtn, SIGNAL(triggered()), this,
        SLOT(on_slotHistogramBtn_triggered()));
    connect(ThresholdBtn, SIGNAL(triggered()), this,
        SLOT(on_slotThresholdBtn_triggered()));
    connect(NegativeBtn, SIGNAL(triggered()), this,
        SLOT(on_slotNegativeBtn_triggered()));
    connect(LogTranBtn, SIGNAL(triggered()), this,
        SLOT(on_slotLogTranBtn_triggered()));
    connect(HistEqualBtn, SIGNAL(triggered()), this,
        SLOT(on_slotHistEqualBtn_triggered()));
    connect(GraystretchBtn, SIGNAL(triggered()), this,
        SLOT(on_slotGrayStretchBtn_triggered()));
    connect(MidFitBtn, SIGNAL(triggered()), this,
        SLOT(on_slotMidFitBtn_triggered()));
    connect(AvgFitBtn, SIGNAL(triggered()), this,
        SLOT(on_slotAvgFitBtn_triggered()));
    connect(LpFitBtn, SIGNAL(triggered()), this,
        SLOT(on_slotLpFitBtn_triggered()));
    connect(SobelBtn, SIGNAL(triggered()), this,
        SLOT(on_slotSobelBtn_triggered()));
    connect(LaplaceBtn, SIGNAL(triggered()), this,
        SLOT(on_slotLaplaceBtn_triggered()));
    connect(HpBtn, SIGNAL(triggered()), this,
        SLOT(on_slotHpFitBtn_triggered()));
    connect(InvertFitBtn, SIGNAL(triggered()), this,
        SLOT(on_slotInvertFitBtn_triggered()));
    connect(MotionBlurBtn, SIGNAL(triggered()), this,
        SLOT(on_slotMotionBlurBtn_triggered()));
    connect(WienerFitBtn, SIGNAL(triggered()), this,
        SLOT(on_slotWienerFitBtn_triggered()));

    connect(CenterBtn, SIGNAL(triggered()), this,
        SLOT(on_slotCenterBtn_triggered()));
    connect(RotateBtn, SIGNAL(triggered()), this,
        SLOT(on_slotRotateBtn_triggered()));

    connect(DFTBtn, SIGNAL(triggered()), this, SLOT(on_slotDFTBtn_triggered()));
    connect(DCTBtn, SIGNAL(triggered()), this, SLOT(on_slotDCTBtn_triggered()));
    connect(WalshBtn, SIGNAL(triggered()), this,
        SLOT(on_slotWalshBtn_triggered()));
    connect(HaarBtn, SIGNAL(triggered()), this,
        SLOT(on_slotHaarBtn_triggered()));

    connect(RandomNoiseBtn, SIGNAL(triggered()), this,
        SLOT(on_slotRandomNoiseBtn_triggered()));
    connect(ImpulseNoiseBtn, SIGNAL(triggered()), this,
        SLOT(on_slotImpulseNoiseBtn_triggered()));


    connect(DCTCodingBtn, SIGNAL(triggered()), this,
        SLOT(on_slotDCTCodingBtn_triggered()));
}

MainWindow::~MainWindow() { delete ui; }

bool MainWindow::on_slotOpenImage_triggered() {
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open File Name"), ".", tr("Image File(*.bmp *.jpg)"));
    if (filename != "") {
        if (winimg->load(filename)) {
            if (winimg->isNull()) {
                int ret = QMessageBox::warning(this, QStringLiteral("错误"),
                    QStringLiteral("无法打开图片"),
                    QMessageBox::Abort);
                if (ret == QMessageBox::Abort) return false;
            }
            QGraphicsScene* scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(*winimg));
            ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            ui->graphicsView->setScene(scene);
            ui->graphicsView->show();

            geometricflag = true;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void MainWindow::on_slotSaveImage_triggered() {
    QString filename = QFileDialog::getSaveFileName(
        this, tr("Save File"), tr("filename"), tr("*.bmp"));
    if (filename != "") {
        winimg->save(filename + ".bmp");
    }
}

void MainWindow::on_slotFreshImage_triggered() {
    delete[] winimg;
    winimg = new QImage(256, 256, QImage::Format_Indexed8);
    winimg->fill(255);
    winimgshow();
}

void MainWindow::on_slotExit_triggered() { qApp->quit(); }

void MainWindow::on_slotInfo_triggered() {
    QMessageBox msg(QMessageBox::NoIcon, QStringLiteral("About"),
        QStringLiteral("ImageEdit\n"));
    msg.setIconPixmap(QPixmap("ImageEdit.ico"));
    msg.exec();
}

bool MainWindow::on_slotMaxValueBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    int ans = maxValue(winimgpixelmat, winimgheight, winimgwidth);
    QMessageBox::about(this, QStringLiteral(" "),
        QStringLiteral("最大值：%1").arg(ans));
    return true;
}

bool MainWindow::on_slotMinValueBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    int ans = minValue(winimgpixelmat, winimgheight, winimgwidth);
    QMessageBox::about(this, QStringLiteral(" "),
        QStringLiteral("最小值：%1").arg(ans));
    return true;
}

bool MainWindow::on_slotAvgValueBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    float ans = avgValue(winimgpixelmat, winimgheight, winimgwidth);
    QMessageBox::about(this, QStringLiteral(" "),
        QStringLiteral("平均值：%1").arg(ans));
    return true;
}

bool MainWindow::on_slotVarValueBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    float ans = varValue(winimgpixelmat, winimgheight, winimgwidth);
    QMessageBox::about(this, QStringLiteral(" "),
        QStringLiteral("方差：%1").arg(ans));
    return true;
}

bool MainWindow::on_slotHistogramBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    // int* histarr = new int[256];
    int* histarr = histogram(winimgpixelmat, winimgheight, winimgwidth);
    if (histarr == NULL) {
        return false;
    }
    int histmax = 0;
    for (int i = 0; i < 256; i++) {
        if (histmax < histarr[i]) {
            histmax = histarr[i];
        }
    }
    float magratio = 512.0 / float(histmax);

    winimgheight = 512;
    winimgwidth = 512;
    winimgpixelmat = new int* [winimgheight];
    for (int i = 0; i < winimgheight; i++) {
        winimgpixelmat[i] = new int[winimgwidth];
        for (int j = 0; j < winimgwidth; j++) {
            winimgpixelmat[i][j] = 255;
        }
    }

    for (int j = 0; j < 256; j++) {
        for (int i = 512 - 1; i > 512 - histarr[j] * magratio - 1 && i >= 0;
            i--) {
            winimgpixelmat[i][j * 2] = 0;
            winimgpixelmat[i][j * 2 + 1] = 0;
        }
    }
    delete[] histarr;
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotThresholdBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = threshold(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotNegativeBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = negative(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotLogTranBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = logTran(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotHistEqualBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = histEqual(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotGrayStretchBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = grayStretch(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotMidFitBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = midFit(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotAvgFitBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = avgFit(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotLpFitBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = lpFit(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotSobelBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = sobel(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotLaplaceBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = laplace(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotHpFitBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = hpFit(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotCenterBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    if (!geometricflag) {
        int ret = QMessageBox::warning(
            this, QStringLiteral("错误"),
            QStringLiteral("请重新打开新的图片以重置显示框"), QMessageBox::Abort);
        if (ret == QMessageBox::Abort) return false;
    }
    initframepixelmat();
    framepixelmat =
        centralize(framepixelmat, winimgpixelmat, winimgheight, winimgwidth);
    if (framepixelmat == NULL) {
        return false;
    }
    geometricflag = false;
    framepixelmattoshow();
    return true;
}

bool MainWindow::on_slotRotateBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    if (!geometricflag) {
        int ret = QMessageBox::warning(
            this, QStringLiteral("错误"),
            QStringLiteral("请重新打开新的图片重置显示框"), QMessageBox::Abort);
        if (ret == QMessageBox::Abort) return false;
    }
    initframepixelmat();
    framepixelmat =
        rotate(framepixelmat, winimgpixelmat, winimgheight, winimgwidth);
    if (framepixelmat == NULL) {
        return false;
    }
    geometricflag = false;
    framepixelmattoshow();
    return true;
}

bool MainWindow::on_slotDFTBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = DFT(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotDCTBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = DCT(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotWalshBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = walsh(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotHaarBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = haar(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotRandomNoiseBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = randomNoise(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotImpulseNoiseBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = impulseNoise(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotInvertFitBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = invertFit(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotMotionBlurBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = motionBlur(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotWienerFitBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = wienerFit(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    return true;
}

bool MainWindow::on_slotDCTCodingBtn_triggered() {
    if (winimgtopixelmat() == false) {
        return false;
    }
    winimgpixelmat = DCTCoding(winimgpixelmat, winimgheight, winimgwidth);
    if (winimgpixelmatshow() == false) {
        return false;
    }
    QMessageBox::about(this, QStringLiteral(" "),
        QStringLiteral("编码成功并解码"));
    return true;
}

bool MainWindow::winimgtopixelmat() {
    if (winimg->isNull()) {
        return false;
    }
    uchar* winimgpt;
    winimgheight = winimg->height();
    winimgwidth = winimg->width();
    winimgpixelmat = new int* [winimgheight];
    for (int i = 0; i < winimgheight; i++) {
        winimgpixelmat[i] = new int[winimgwidth];
        winimgpt = winimg->scanLine(i);
        for (int j = 0; j < winimgwidth; j++) {
            winimgpixelmat[i][j] = int(winimgpt[j]);
        }
    }
    return true;
}

void MainWindow::pixelmattowinimg() {
    winimg = new QImage(winimgheight, winimgwidth, QImage::Format_Indexed8);
    int* dataptr;
    uchar* imgptr;
    for (int i = 0; i < winimgheight; i++) {
        dataptr = winimgpixelmat[i];
        imgptr = winimg->scanLine(i);
        for (int j = 0; j < winimgwidth; j++) {
            imgptr[j] = uchar(dataptr[j]);
        }
    }
}

void MainWindow::initframepixelmat() {
    framepixelmat = new int* [FRAME_WIDTH];
    for (int i = 0; i < FRAME_WIDTH; i++) {
        framepixelmat[i] = new int[FRAME_WIDTH];
        for (int j = 0; j < FRAME_WIDTH; j++) {
            framepixelmat[i][j] = 255;
        }
    }
}

void MainWindow::framepixelmattoshow() {
    int* dataptr;
    uchar* winimgpt;
    winimg = new QImage(FRAME_WIDTH, FRAME_WIDTH, QImage::Format_Indexed8);
    for (int i = 0; i < FRAME_WIDTH; i++) {
        dataptr = framepixelmat[i];
        winimgpt = winimg->scanLine(i);
        for (int j = 0; j < FRAME_WIDTH; j++) {
            winimgpt[j] = uchar(dataptr[j]);
        }
    }
    winimgshow();
}

bool MainWindow::winimgshow() {
    if (winimg->isNull()) {
        return false;
    }
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(*winimg));
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

    return true;
}

bool MainWindow::winimgpixelmatshow() {
    if (winimgpixelmat == NULL) {
        return false;
    }
    pixelmattowinimg();
    winimgshow();
    return true;
}
