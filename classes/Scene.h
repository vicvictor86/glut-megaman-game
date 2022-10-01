#ifndef GAME_PROJECT_SCENE_H
#define GAME_PROJECT_SCENE_H

#include "Object.h"
#include "Wall.h"
#include "FloatingBlocksHor.h"

class Scene {
public: bool changeDirection;
public: Wall buildFloorBlock();
public: Wall buildRaisedBlock(int yValue);
public: FloatingBlocksHor buildFloatBlockHor(float y);
public: void buildHole(float x);
public: EnemyMet spawnEnemyMet();
public: EnemyHorizontal spawnHorizontalEnemy();
public: EnemyVertical spawnVerticalEnemy();
public: Scene() {currentX = -2;changeDirection = true;};

private: int currentX;
};

Wall Scene::buildFloorBlock() {
    Wall wall;
    wall.x = this->currentX;
    wall.y = -2;
    wall.z = -6;
    wall.setSize(2);
    wall.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.sizeH, wall.sizeV);

    this->currentX += 2;

    return wall;
}

void Scene::buildHole(float x = 2) {
    this->currentX += x;
}

Wall Scene::buildRaisedBlock(int yValue) {
    Wall wall;
    wall.x = this->currentX;
    switch(yValue) {
        case 0:
            wall.y = yValue;
            break;
        case 1:
            wall.y = 2;
            break;
        default:
            break;
    }

    wall.z = -6;
    wall.setSize(2);
    wall.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.sizeH);

    return wall;
}

FloatingBlocksHor Scene::buildFloatBlockHor(float y = -1) {
    FloatingBlocksHor floating;
    floating.x = this->currentX;
    floating.y = y;
    floating.z = -6;
    floating.setSize(1);

    floating.collision.setSize(floating.sizeH + 0.2f);
    floating.x = this->currentX;
    floating.y = y;
    floating.z = -6;

    if(this->changeDirection) {
        floating.speed.x = 0.02;
        changeDirection = false;
    } else {
        floating.speed.x = -0.02;
        changeDirection = true;
    }

    floating.mapColliderWall = Object ::createRetangleCollider(floating.x, floating.y, floating.z, floating.sizeH);

    this->currentX += 2;

    return floating;
}

EnemyMet Scene::spawnEnemyMet() {
    EnemyMet enemy;
    enemy.setX(this->currentX);
    enemy.setY(0);
    enemy.setZ(-6);
    enemy.setSize(1);
    enemy.speed.z = 0.01;
    enemy.collision.setSize(enemy.sizeH + 0.2f);
    enemy.mapCollider = Object ::createRetangleCollider(enemy.collision.x, enemy.collision.y, enemy.collision.z, enemy.collision.sizeH);
    return enemy;
}

EnemyHorizontal Scene::spawnHorizontalEnemy() {
    EnemyHorizontal enemy;
    enemy.setX(this->currentX);
    enemy.setY(0);
    enemy.setZ(-6);
    enemy.setSize(1);
    enemy.speed.x = 0.01;
    enemy.collision.setSize(enemy.sizeH + 0.2f);
    enemy.mapCollider = Object ::createRetangleCollider(enemy.collision.x, enemy.collision.y, enemy.collision.z, enemy.collision.sizeH);
    return enemy;
}

EnemyVertical Scene::spawnVerticalEnemy(){
    EnemyVertical enemy;
    enemy.setX(this->currentX);
    enemy.setY(0);
    enemy.setZ(-6);
    enemy.setSize(1);
    enemy.speed.y = 0.01;
    enemy.collision.setSize(enemy.sizeH + 0.2f);
    enemy.mapCollider = Object ::createRetangleCollider(enemy.collision.x, enemy.collision.y, enemy.collision.z, enemy.collision.sizeH);
    return enemy;
}

#endif //GAME_PROJECT_SCENE_H
