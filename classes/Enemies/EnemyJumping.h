#ifndef GAME_PROJECT_ENEMYJUMPING_H
#define GAME_PROJECT_ENEMYJUMPING_H

#include "Enemies/Enemy.h"

class EnemyJumping : public Enemy{
    public: void move() override;
    public: EnemyJumping() = default;
};

void EnemyJumping :: move() {
    if (this->timeToChangeDirection == -1) {
        this->timeToChangeDirection = time(nullptr);
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToChangeDirection > this->coldDown && this->collision.isOnPlataform) {
        this->speed.y = 0.08;
    }

    if (this->speed.y != 0) {
        this->y += this->speed.y;
    }

    if (!this->collision.isOnPlataform){
        this->speed.y -= 0.001f;
    }
}

#endif //GAME_PROJECT_ENEMYJUMPING_H
