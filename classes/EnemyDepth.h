#ifndef GAME_PROJECT_ENEMYDEPTH_H
#define GAME_PROJECT_ENEMYDEPTH_H

#include "Enemy.h"

class EnemyDepth : public Enemy {
    public: void move() override;
    public: EnemyDepth() = default;
};

void EnemyDepth :: move() {
    if (this->timeToChangeDirection == -1) {
        this->timeToChangeDirection = time(nullptr);
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToChangeDirection > this->coldDown) {
        this->speed.z *= -1;
        this->timeToChangeDirection = -1;
    }

    if (this->speed.z != 0) {
        this->z += this->speed.z;
    }
}


#endif //GAME_PROJECT_ENEMYDEPTH_H
