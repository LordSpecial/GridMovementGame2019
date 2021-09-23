#ifndef PICKUPITEM_H
#define PICKUPITEM_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

class PickUpItem:public QObject, public QGraphicsPixmapItem
{
public:
    PickUpItem(QPixmap skin, QGraphicsItem * parent=0);
};

#endif // PICKUPITEM_H
