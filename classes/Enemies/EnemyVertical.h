#ifndef GAME_PROJECT_ENEMYVERTICAL_H
#define GAME_PROJECT_ENEMYVERTICAL_H

#include "Enemies/Enemy.h"

class EnemyVertical : public Enemy {
    public: void move() override;
    public: EnemyVertical() = default;
};

void EnemyVertical :: move() {
    if (this->timeToChangeDirection == -1) {
        this->timeToChangeDirection = time(nullptr);
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToChangeDirection > this->coldDown) {
        this->speed.y *= -1;
        this->timeToChangeDirection = -1;
    }

    if (this->speed.y != 0) {
        this->y += this->speed.y;
    }
}

#endif //GAME_PROJECT_ENEMYVERTICAL_H
