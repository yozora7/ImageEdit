#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include <QMainWindow>
#include <QTextCodec>
#include <cmath>
#include <cstdlib>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

   private:
    Ui::MainWindow *ui;
    QImage *winimg;
    int winimgheight;
    int winimgwidth;
    int **winimgpixelmat;
    int **framepixelmat;
    bool geometricflag;
    bool winimgtopixelmat();
    void pixelmattowinimg();
    void initframepixelmat();
    void framepixelmattoshow();
    bool winimgshow();
    bool winimgpixelmatshow();

   private slots:
    //
    // basic entries
    //
    bool on_slotOpenImage_triggered();
    void on_slotSaveImage_triggered();
    void on_slotFreshImage_triggered();
    void on_slotExit_triggered();
    void on_slotInfo_triggered();
    //
    // calculate parameter
    //
    bool on_slotMaxValueBtn_triggered();
    bool on_slotMinValueBtn_triggered();
    bool on_slotAvgValueBtn_triggered();
    bool on_slotVarValueBtn_triggered();
    //
    // enhancement
    //
    // contrast enhancement
    bool on_slotHistogramBtn_triggered();
    bool on_slotThresholdBtn_triggered();
    bool on_slotNegativeBtn_triggered();
    bool on_slotLogTranBtn_triggered();
    bool on_slotHistEqualBtn_triggered();
    bool on_slotGrayStretchBtn_triggered();
    // smoothing & sharpening
    bool on_slotMidFitBtn_triggered();
    bool on_slotAvgFitBtn_triggered();
    bool on_slotLpFitBtn_triggered();
    bool on_slotSobelBtn_triggered();
    bool on_slotLaplaceBtn_triggered();
    bool on_slotHpFitBtn_triggered();
    // restore
    bool on_slotInvertFitBtn_triggered();
    bool on_slotWienerFitBtn_triggered();
    //
    // transform
    //
    // geometric transform
    bool on_slotCenterBtn_triggered();
    bool on_slotRotateBtn_triggered();
    // linear transform
    bool on_slotDFTBtn_triggered();
    bool on_slotDCTBtn_triggered();
    bool on_slotHaarBtn_triggered();
    bool on_slotWalshBtn_triggered();
    //
    // noise & blur
    //
    // add noise
    bool on_slotRandomNoiseBtn_triggered();
    bool on_slotImpulseNoiseBtn_triggered();
    // motion blur
    bool on_slotMotionBlurBtn_triggered();
    //
    // coding
    //
    bool on_slotDCTCodingBtn_triggered();
};

#endif  // MAINWINDOW_H
