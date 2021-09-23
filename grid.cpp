#include "grid.h"
#include <QGraphicsScene>

Grid::Grid(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":img/Grid.png"));
}
