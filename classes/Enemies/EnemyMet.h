#ifndef GAME_PROJECT_ENEMYMET_H
#define GAME_PROJECT_ENEMYMET_H

#include "Enemy.h"

class EnemyMet : public Enemy {
    public: void move() override;
    public: void noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, bool drawnCollision) override;
    public: EnemyMet() = default;
};

void EnemyMet :: move() {
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

void EnemyMet:: noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, bool drawnCollision) {
    map<char, double> mapCollisionCanShoot = Object::createRetangleCollider(this->collision.x, this->collision.y, playerZ, this->sizeVisionX);
    map<char, double> mapCollisionCantTakeDamage = Object::createRetangleCollider(this->collision.x, this->collision.y, playerZ, this->sizeVisionX / 2);
    if(drawnCollision){
        glPushMatrix();
            Object::drawObject(this->x, this->y, this->z, this->sizeVisionX, this->sizeVisionY, 1, 0, 0);
        glPopMatrix();
    }

    int quantityOverLapping = 0;
    Collision::checkCollision(mapCollisionCanShoot, this->x, this->y, mapCollisionPlayer, playerX, playerY, true, &quantityOverLapping);
    this->canShoot = quantityOverLapping > 0;

    quantityOverLapping = 0;
    Collision::checkCollision(mapCollisionCantTakeDamage, this->x, this->y, mapCollisionPlayer, playerX, playerY, true, &quantityOverLapping);
    this->canTakeDamage = quantityOverLapping > 0 ? false : true;
}


#endif //GAME_PROJECT_ENEMYMET_H
