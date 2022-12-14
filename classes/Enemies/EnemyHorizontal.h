#ifndef GAME_PROJECT_ENEMYHORIZONTAL_H
#define GAME_PROJECT_ENEMYHORIZONTAL_H

#include "Enemies/Enemy.h"

class EnemyHorizontal : public Enemy {
    public: void move() override;
    public: EnemyHorizontal() = default;
};

void EnemyHorizontal :: move() {
    if (this->timeToChangeDirection == -1) {
        this->timeToChangeDirection = time(nullptr);
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToChangeDirection > this->coldDown) {
        this->speed.x *= -1;
        this->timeToChangeDirection = -1;
    }

    if (this->speed.x != 0) {
        this->x += this->speed.x;
    }
}

#endif //GAME_PROJECT_ENEMYHORIZONTAL_H
