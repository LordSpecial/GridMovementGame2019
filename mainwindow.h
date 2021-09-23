#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "pickupitem.h"

#include <iostream>

using namespace std;
using namespace cv;

#include "game.h"
#include "grid.h"
#include "player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QGraphicsScene * scene;
    Game * game;
    Grid * grid;

    PickUpItem * pickup1;
    PickUpItem * pickup2;
    PickUpItem * pickup3;
    Player * player;

    Ui::MainWindow *ui;

    QTimer *timer;
    QTimer *detectionTimer;
    VideoCapture cap;


    Mat frame;
    QImage qt_image;

    QString playerSkin;
    QString backgroundSkin;
    QString backgroundFadedSkin;
    QString pickup1Skin;
    QString pickup2Skin;
    QString pickup3Skin;

    int currentLevel;
    int score;

    ~MainWindow();

public slots:
    void on_CheckCameraButton_clicked();

    void on_LevelSelectButton_clicked();

    void on_HomeButton_clicked();

    void on_HomeButton2_clicked();

    void on_HomeButton3_clicked();

    void on_SettingsButton_clicked();
    void on_Level1_clicked();

    void on_HomeButton4_clicked();

    void GetCameraImage();
    void SetupGrid(int);
    void GameLoop();

    void PlayerCollect(double[2]);

    void setKnightSkin();
    void setBeeSkin();
private slots:
    void on_Level2_clicked();
    void on_Level3_clicked();
    void on_Level4_clicked();
    void on_Level5_clicked();
    void on_Level6_clicked();
    void on_Level7_clicked();
    void on_Level8_clicked();
    void on_Level9_clicked();
    void on_Level10_clicked();
    void on_Level11_clicked();
    void on_Level12_clicked();
    void on_setThemeKnight_clicked();
    void on_setThemeBee_clicked();
    void on_ExitLevel_clicked();
    void on_NextLevel_clicked();
};

#endif // MAINWINDOW_H
