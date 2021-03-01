/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *OpenImage;
    QAction *SaveImage;
    QAction *ActionExit;
    QAction *ActionDemo;
    QAction *ActionInfo;
    QAction *Actionhomepage;
    QAction *FreshImage;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QFrame *horizontalFrame;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ParameterBtn;
    QPushButton *EnhanceBtn;
    QPushButton *TransformBtn;
    QPushButton *NoiseBlurBtn;
    QPushButton *CodingBtn;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *FileMenu;
    QMenu *AboutMenu;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(640, 480));
        MainWindow->setMaximumSize(QSize(1280, 960));
        OpenImage = new QAction(MainWindow);
        OpenImage->setObjectName(QString::fromUtf8("OpenImage"));
        SaveImage = new QAction(MainWindow);
        SaveImage->setObjectName(QString::fromUtf8("SaveImage"));
        ActionExit = new QAction(MainWindow);
        ActionExit->setObjectName(QString::fromUtf8("ActionExit"));
        ActionDemo = new QAction(MainWindow);
        ActionDemo->setObjectName(QString::fromUtf8("ActionDemo"));
        ActionInfo = new QAction(MainWindow);
        ActionInfo->setObjectName(QString::fromUtf8("ActionInfo"));
        Actionhomepage = new QAction(MainWindow);
        Actionhomepage->setObjectName(QString::fromUtf8("Actionhomepage"));
        FreshImage = new QAction(MainWindow);
        FreshImage->setObjectName(QString::fromUtf8("FreshImage"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalFrame = new QFrame(centralWidget);
        horizontalFrame->setObjectName(QString::fromUtf8("horizontalFrame"));
        horizontalLayout_2 = new QHBoxLayout(horizontalFrame);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        ParameterBtn = new QPushButton(horizontalFrame);
        ParameterBtn->setObjectName(QString::fromUtf8("ParameterBtn"));

        horizontalLayout_2->addWidget(ParameterBtn);

        EnhanceBtn = new QPushButton(horizontalFrame);
        EnhanceBtn->setObjectName(QString::fromUtf8("EnhanceBtn"));

        horizontalLayout_2->addWidget(EnhanceBtn);

        TransformBtn = new QPushButton(horizontalFrame);
        TransformBtn->setObjectName(QString::fromUtf8("TransformBtn"));

        horizontalLayout_2->addWidget(TransformBtn);

        NoiseBlurBtn = new QPushButton(horizontalFrame);
        NoiseBlurBtn->setObjectName(QString::fromUtf8("GrainBtn"));

        horizontalLayout_2->addWidget(NoiseBlurBtn);

        CodingBtn = new QPushButton(horizontalFrame);
        CodingBtn->setObjectName(QString::fromUtf8("CodingBtn"));

        horizontalLayout_2->addWidget(CodingBtn);


        verticalLayout_2->addWidget(horizontalFrame);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy1);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        horizontalLayout->addWidget(graphicsView);


        verticalLayout_2->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 26));
        FileMenu = new QMenu(menuBar);
        FileMenu->setObjectName(QString::fromUtf8("FileMenu"));
        AboutMenu = new QMenu(menuBar);
        AboutMenu->setObjectName(QString::fromUtf8("AboutMenu"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(FileMenu->menuAction());
        menuBar->addAction(AboutMenu->menuAction());
        FileMenu->addAction(OpenImage);
        FileMenu->addAction(SaveImage);
        FileMenu->addAction(FreshImage);
        FileMenu->addSeparator();
        FileMenu->addAction(ActionExit);
        AboutMenu->addAction(ActionInfo);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        OpenImage->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
        SaveImage->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        ActionExit->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
        ActionDemo->setText(QApplication::translate("MainWindow", "\345\256\236\351\252\214\350\257\264\346\230\216", nullptr));
        ActionInfo->setText(QApplication::translate("MainWindow", "About", nullptr));
        Actionhomepage->setText(QApplication::translate("MainWindow", "\350\257\276\347\250\213\344\270\273\351\241\265", nullptr));
        FreshImage->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
        ParameterBtn->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217\345\217\202\346\225\260", nullptr));
        EnhanceBtn->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217\345\242\236\345\274\272", nullptr));
        TransformBtn->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217\345\217\230\346\215\242", nullptr));
        NoiseBlurBtn->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217\345\212\240\345\231\252", nullptr));
        CodingBtn->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217\347\274\226\347\240\201", nullptr));
        FileMenu->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        AboutMenu->setTitle(QApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
