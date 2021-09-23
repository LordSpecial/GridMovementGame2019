#include <QDebug>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>

#include "opencv2/opencv.hpp"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include "cameraworker.h"
#include "maps.h"
#include "detection.cpp"
#include "UISkins.h"

void MainWindow::setKnightSkin() {
    playerSkin = ":img/player_knight.png";
    backgroundSkin = ":img/background_knight.png";
    backgroundFadedSkin = ":img/backgroundFaded_knight.png";
    pickup1Skin = ":img/pickup1_knight.png";
    pickup2Skin = ":img/pickup2_knight.png";
    pickup3Skin = ":img/pickup3_knight.png";
    this->ui->stackedWidget->setStyleSheet("background-image: url(:/img/backgroundFaded_knight.png);");
    this->ui->GameView->setStyleSheet("background-image: url(:/img/backgroundFaded_knight.png);");
}

/*
void MainWindow::setKnightSkin() {
    playerSkin = ":img/player_space.png";
    backgroundSkin = ":img/background_space.png";
    backgroundFadedSkin = ":img/background_space.png";
    pickup1Skin = ":img/pickup1_space.png";
    pickup2Skin = ":img/pickup2_space.png";
    pickup3Skin = ":img/pickup3_space.png";
    this->ui->stackedWidget->setStyleSheet("background-image: url(:/img/background_space.png);");
    this->ui->GameView->setStyleSheet("background-image: url(:/img/background_space.png);");
}
*/
void MainWindow::setBeeSkin() {
    playerSkin = ":img/player_bee.png";
    backgroundSkin = ":img/background_bee.png";
    backgroundFadedSkin = ":img/backgroundFaded_bee.png";
    pickup1Skin = ":img/pickup1_bee.png";
    pickup2Skin = ":img/pickup2_bee.png";
    pickup3Skin = ":img/pickup3_bee.png";
    this->ui->stackedWidget->setStyleSheet("background-image: url(:/img/backgroundFaded_bee.png);");
    this->ui->GameView->setStyleSheet("background-image: url(:/img/backgroundFaded_bee.png);");
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setBeeSkin();
    cap.open(0);
}


MainWindow::~MainWindow()
{

    qDebug() << "Initiate CLOSE";
    destroyAllWindows();
    cap.release();

    delete ui;

}

//Buttons and Shit

void MainWindow::on_LevelSelectButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}


void MainWindow::on_CheckCameraButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    if(!cap.isOpened())  // Check if we succeeded
    {
        QMessageBox Msgbox;
        Msgbox.setText("Camera Not Detected\nPlease Ensure a camera is connected and restart application");
        Msgbox.exec();
    }
    else
    {
        cout << "camera is open" << endl;

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(GetCameraImage()));
        timer->start(50);
    }
}

void MainWindow::on_HomeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_HomeButton2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    disconnect(timer, SIGNAL(timeout()), this, SLOT(GetCameraImage()));
}

void MainWindow::on_HomeButton3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_SettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_HomeButton4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    detectionTimer->stop();
}

void MainWindow::GetCameraImage()
{
    cap >> frame;

    cvtColor(frame, frame, COLOR_BGR2RGB);

    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(qt_image));

    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::SetupGrid(int map) {

    qDebug() << "SETUP GRID";

    score = 0;

    scene = new QGraphicsScene();

    ui->GameView->setScene(scene);

    ui->NextLevel->hide();
    ui->ExitLevel->hide();
    ui->LevelCompleteBox->hide();
    ui->NextLevelQuestion->hide();
    ui->WellDone->hide();
    ui->ScoreLabel->hide();
    ui->BestScoreLabel->hide();

    ui->LevelLabel->setText(QString("Level %1").arg(map));

    game = new Game(*scene);

    pickup1 = new PickUpItem(pickup1Skin);
    pickup1->setPos((53 + maps[map][0][0] * 61), (53 + maps[map][0][1] * 61));
    scene->addItem(pickup1);

    pickup2 = new PickUpItem(pickup2Skin);
    pickup2->setPos((53 + maps[map][1][0] * 61), (53 + maps[map][1][1] * 61));
    scene->addItem(pickup2);

    pickup3 = new PickUpItem(pickup3Skin);
    pickup3->setPos((53 + maps[map][2][0] * 61), (53 + maps[map][2][1] * 61));
    scene->addItem(pickup3);

    player = new Player(playerSkin);
    scene->addItem(player);

    player->setPos(53, (53 + 61*9));
    qDebug() << "SETUP GRID FINISHED";

    ui->stackedWidget->setCurrentIndex(4);
    if (!cap.isOpened()){
        QMessageBox Msgbox;
        Msgbox.setText("Camera Not Detected\nPlease Ensure a camera is connected and restart application");
        Msgbox.exec();
    } else {

        detectionTimer = new QTimer(this);
        connect(detectionTimer, SIGNAL(timeout()), this, SLOT(GameLoop()));
        detectionTimer->start(500); // Run every 0.5 seconds
    }
}

void MainWindow::GameLoop() {
//    auto t1 = std::chrono::high_resolution_clock::now();

    cap >> frame;

    int playerPosition[2];
    double oldPos[2] = {player->x(), player->y()};
    DetectGrid(playerPosition, frame);
    //    qDebug() << playerPosition[0] << ", " << playerPosition[1];
    player->MovePlayer(oldPos, playerPosition);
    PlayerCollect(oldPos);
//    auto t2 = std::chrono::high_resolution_clock::now();

//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
//    qDebug() << "function time " << duration;
}

void MainWindow::PlayerCollect(double * oldPos) {
    if (oldPos[0] == player->x() && oldPos[1] == player->y()) {
        return;
    }
    score++;

    if(player->x() == pickup1->x() && player->y() == pickup1->y()) {
        pickup1->hide();
    }
    if(!pickup1->isVisible() && player->x() == pickup2->x() && player->y() == pickup2->y()) {
        pickup2->hide();
    }
    if(!pickup2->isVisible() && player->x() == pickup3->x() && player->y() == pickup3->y()) {
        pickup3->hide();

        ui->NextLevel->show();
        ui->ExitLevel->show();
        ui->LevelCompleteBox->show();
        ui->NextLevelQuestion->show();
        ui->WellDone->show();
        ui->ScoreLabel->setText(QString("You took %1 Moves").arg(score));
        ui->BestScoreLabel->setText(QString("Best score: %1 Moves").arg(maps[currentLevel--][3][0]));
        ui->ScoreLabel->show();
        ui->BestScoreLabel->show();
    }
}

void MainWindow::on_Level1_clicked()
{
    currentLevel = 1;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level2_clicked()
{
    currentLevel = 2;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level3_clicked()
{
    currentLevel = 3;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level4_clicked()
{
    currentLevel = 4;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level5_clicked()
{
    currentLevel = 5;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level6_clicked()
{
    currentLevel = 6;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level7_clicked()
{
    currentLevel = 7;
    SetupGrid(7);
}

void MainWindow::on_Level8_clicked()
{
    currentLevel = 8;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level9_clicked()
{
    currentLevel = 9;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level10_clicked()
{
    currentLevel = 10;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level11_clicked()
{
    currentLevel = 11;
    SetupGrid(currentLevel);
}

void MainWindow::on_Level12_clicked()
{
    currentLevel = 12;
    SetupGrid(currentLevel);
}

void MainWindow::on_setThemeKnight_clicked()
{
    setKnightSkin();
}

void MainWindow::on_setThemeBee_clicked()
{
    setBeeSkin();
}

void MainWindow::on_ExitLevel_clicked()
{
    //    ui->stackedWidget->setCurrentIndex(1);
    currentLevel++;
    SetupGrid(currentLevel);
}

void MainWindow::on_NextLevel_clicked()
{

    if (currentLevel < 12) {
        currentLevel+=2;
        SetupGrid(currentLevel);
    }
}
