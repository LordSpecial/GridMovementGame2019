#include "player.h"
#include "UISkins.h"
#include <QDebug>

Player::Player(QPixmap skin, QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    setPixmap(skin);
}

bool MoveStraight(double oldLocation[], int newLocation[]) {
    if ((oldLocation[0] == newLocation[0]) != (oldLocation[1] == newLocation[1])) {
        return true;
    } else {
        return false;
    }
}

void Player::MovePlayer(double oldPos[], int location[]){
    int nLocation[2];
    nLocation[0] = (53 + 61*location[0]);
    nLocation[1]= (53 + 61*location[1]);
    if(MoveStraight(oldPos, nLocation)){
        qDebug() << "Straigt | " << oldPos[0] << "," << nLocation[0] << " " << oldPos[1] << "," << nLocation[1];
        if (location[0] <10 && location[1] < 10){
            this->setPos((53 + 61*location[0]),(53 + 61*location[1]));
        }
    }/* else {
        if (nLocation[0] != oldPos[0]){
        } else{

        }

    }*/
}
