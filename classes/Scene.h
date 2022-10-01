#ifndef GAME_PROJECT_SCENE_H
#define GAME_PROJECT_SCENE_H

#include "Object.h"
#include "Wall.h"

class Scene {
public: Wall buildFloorBlock();
public: Wall buildRaisedBlock(int yValue);
public: void buildHole();
public: EnemyMet spawnEnemyMet();
public: EnemyHorizontal spawnHorizontalEnemy();
public: EnemyVertical spawnVerticalEnemy();
public: Scene() {currentX = -2;};

private: int currentX;
};

Wall Scene::buildFloorBlock() {
    Object wall;
    wall.x = this->currentX;
    wall.y = -2;
    wall.z = -6;
    wall.setSize(2);

    Wall wallWithCollider;
    wallWithCollider.wallObject = wall;
    wallWithCollider.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.sizeH, wall.sizeV);

    this->currentX += 2;

    return wallWithCollider;
}

void Scene::buildHole() {
    this->currentX += 2;
}

Wall Scene::buildRaisedBlock(int yValue) {
    Object wall;
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

    Wall wallWithCollider;
    wallWithCollider.wallObject = wall;
    wallWithCollider.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.sizeH);

    return wallWithCollider;
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
