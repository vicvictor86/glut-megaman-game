
#ifndef GAME_PROJECT_ENEMY_H
#define GAME_PROJECT_ENEMY_H
#include "Object.h"

class Enemy : public Object {
    public: int life = 3;
    public: int damage=1;
    public: int timeToShoot=-1;
    public: int shootColdDown=2;
    public: int coldDown=2;
    public: int timeToChangeDirection=-1;
    public: bool canTakeDamage=true;
    public: Collision viewOfEnemy;
    public: virtual void move();
    public: virtual void shoot(vector<Fire>* fireObjects);
    public: void getDamage(int takedDamage);
    public: virtual void noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, double sizeOfVision, bool drawnCollision);
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

void Enemy::shoot(vector<Fire>* fireObjects) {
    if (this->timeToShoot == -1) {
        this->timeToShoot = time(nullptr);
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToShoot > this->shootColdDown) {
        Fire fire;

        double spawnPoint = this->x + 1;
        double heightOfPlayer = this->y + 0;
        float shootSpeed = 0.06f;
        float radiusOfFire = 0.5;

        fire.x = spawnPoint;
        fire.y = heightOfPlayer;
        fire.z = this->z;

        fire.speed.x = shootSpeed;
        fire.size = radiusOfFire;
        fire.collision.size = 0.55;

        fire.slicesAndStacks = 16;
        fire.collision.x = 0;
        fire.collision.y = 0;
        fire.collision.z = fire.z;

        fire.tagShoot = "Enemy";
        fire.damage = this->damage;

        fireObjects->push_back(fire);
        this->timeToShoot = -1;
    }
}

void Enemy::getDamage(int takedDamage) {
    if(this->canTakeDamage){
        this->life -= takedDamage;
    }
}

void Enemy::noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, double sizeOfVision, bool drawnCollision) {
//    cout << "Enemy noticed" << endl;
}

#endif //GAME_PROJECT_ENEMY_H