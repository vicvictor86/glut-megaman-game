
#ifndef GAME_PROJECT_ENEMY_H
#define GAME_PROJECT_ENEMY_H
#include "Object.h"

class Enemy : public Object {
    public: int life = 3;
    public: float damage=1;
    public: int timeToShoot=0;
    public: int coldDown=2;
    public: int timeToChangeDirection=-1;
    public: map<char, double> mapCollider;
    public: virtual void move();
    public: Enemy() = default;
};

void Enemy:: move(){
    if (this->speed.x != 0) {
        this->x += this->speed.x;
    }

    if (this->speed.y != 0){
        this->y += this->speed.y;
    }
}

#endif //GAME_PROJECT_ENEMY_H
