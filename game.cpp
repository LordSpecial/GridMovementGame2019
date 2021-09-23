#include "game.h"
#include "mainwindow.cpp"

#include <QTimer>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QFont>
#include <QImage>


Game::Game(QGraphicsScene &scene)
{
    grid = new Grid();

    grid->setPos(45, 45);
    scene.addItem(grid);

}
