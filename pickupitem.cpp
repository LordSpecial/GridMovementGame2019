#include "pickupitem.h"

PickUpItem::PickUpItem(QPixmap pickupName, QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    setPixmap(pickupName);
}
