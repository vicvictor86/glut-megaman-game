#ifndef GAME_PROJECT_ENEMYMET_H
#define GAME_PROJECT_ENEMYMET_H

#include "Enemy.h"

class EnemyMet : public Enemy {
    public: void move() override;
    public: void noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, double sizeOfVision, bool drawnCollision) override;
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

void EnemyMet:: noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, double sizeOfVision, bool drawnCollision) {
    map<char, double> mapCollisionoViewOfPlayer = Object::createRetangleCollider(this->collision.x, this->collision.y, playerZ, sizeOfVision);
    if(drawnCollision){
        Object::drawObject(this->x, this->y, this->z, 2);
    }

    int quantityOverLapping = 0;
    Collision::checkCollision(mapCollisionoViewOfPlayer, this->x, this->y, mapCollisionPlayer, playerX, playerY, true, &quantityOverLapping);
    if(quantityOverLapping > 0){
        this->canTakeDamage = false;
        cout << "Nao toma dano" << endl;
    } else {
        this->canTakeDamage = true;
    }
}


#endif //GAME_PROJECT_ENEMYMET_H
