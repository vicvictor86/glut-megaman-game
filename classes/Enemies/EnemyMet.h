#ifndef GAME_PROJECT_ENEMYMET_H
#define GAME_PROJECT_ENEMYMET_H

#include "Enemy.h"

class EnemyMet : public Enemy {
    public: void move() override;
    public: void noticedEnemy(const string& animationName, int animationFrame, map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, bool drawnCollision) override;
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

    if (this->speed.y != 0) {
        this->y += this->speed.y;
    }

    if (!this->collision.isOnPlataform){
        this->speed.y -= 0.001f;
    }
}

void EnemyMet:: noticedEnemy(const string& animationName, int animationFrame, map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, bool drawnCollision) {
    map<char, double> mapCollisionCanShoot = Object::createRetangleCollider(this->collision.x, this->collision.y, playerZ, this->sizeVisionX);
    map<char, double> mapCollisionCantTakeDamage = Object::createRetangleCollider(this->collision.x, this->collision.y, playerZ, this->sizeVisionX / 2);
    if(drawnCollision){
        glPushMatrix();
            Object::drawObject("", animationFrame, this->x, this->y, this->z, this->sizeVisionX, this->sizeVisionY, 1, 0, 0);
        glPopMatrix();
    }

    int triggerWithPlayer = 0;
    Collision::checkCollision(mapCollisionCanShoot, this->x, this->y, mapCollisionPlayer, playerX, playerY, true, &triggerWithPlayer);

    int triggerTooCloseWithPlayer = 0;
    Collision::checkCollision(mapCollisionCantTakeDamage, this->x, this->y, mapCollisionPlayer, playerX, playerY, true, &triggerTooCloseWithPlayer);
    this->canTakeDamage = triggerTooCloseWithPlayer > 0 ? false : true;
    this->animationStatus = this->canTakeDamage ? "idle" : "hidden";
    this->canShoot = triggerWithPlayer > 0 && this->canTakeDamage;
}


#endif //GAME_PROJECT_ENEMYMET_H
