#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include <QGraphicsView>

#include "grid.h"

class Game: public QGraphicsView
{
    Q_OBJECT
public:
    Game(QGraphicsScene &scene);
    Grid * grid;
};

#endif // GAME_H
