#ifndef PLAYER_H
#define PLAYER_H

#include<QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

class Player:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Player(QPixmap skin, QGraphicsItem * parent=0);
public slots:
    void MovePlayer(double oldPos[], int location[]);
};
#endif // PLAYER_Hs
