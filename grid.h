#ifndef GRID_H
#define GRID_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

class Grid:public QObject, public QGraphicsPixmapItem
{
public:
    Grid(QGraphicsItem * parent=0);
};

#endif // GRID_H
