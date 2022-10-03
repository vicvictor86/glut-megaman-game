#ifndef GAME_PROJECT_SCENE_H
#define GAME_PROJECT_SCENE_H

#include "Object.h"
#include "Wall.h"
#include "FloatingBlocksHor.h"

class Scene {
public: bool changeDirection;
public: Wall buildFloorBlock();
public: Wall buildRaisedBlock(float yValue, float size);
public: FloatingBlocksHor buildFloatBlockHor(float y);
public: void buildHole(float x);
public: EnemyMet spawnEnemyMet();
public: EnemyHorizontal spawnHorizontalEnemy();
public: EnemyVertical spawnVerticalEnemy();
public: EnemyJumping spawnJumpingEnemy();
public: Scene() {currentX = -2;changeDirection = true;};

private: int currentX;
};

Wall Scene::buildFloorBlock() {
    Wall wall;
    wall.x = this->currentX;
    wall.y = -2;
    wall.z = -6;
    wall.setSize(2);
    wall.setScaleSizeModel(5.5);
    wall.modelY = -0.4;

    wall.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.sizeH, wall.sizeV);
    wall.setAnimations("block", "../Models/Environment/ground/", "metalBlock", 0, 20);

    this->currentX += 2;

    return wall;
}

void Scene::buildHole(float x = 2) {
    this->currentX += x;
}

Wall Scene::buildRaisedBlock(float yValue, float size = 2) {
    Wall wall;
    wall.x = this->currentX;
    if(yValue == 1)
        wall.y = 1;
    else
        wall.y = yValue;

    if(size == 1 && yValue == -0.5)
        wall.z = -6.1;
    else
        wall.z = -6;

    wall.setSize(size);
    wall.setScaleSizeModel(5.5);
    wall.modelY = -0.4 + yValue;

    wall.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.sizeH, wall.sizeV);
    wall.setAnimations("block", "../Models/Environment/ground/", "metalBlock", 0, 20);

    return wall;
}

FloatingBlocksHor Scene::buildFloatBlockHor(float y = -1.5) {
    FloatingBlocksHor floating;
    floating.x = this->currentX;
    floating.y = y;
    floating.z = -6;
    floating.setSize(3, 1);

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

    floating.modelY = -0.45;

    floating.setScaleSizeModel(1.5, 2, 2);
    floating.mapColliderWall = Object ::createRetangleCollider(floating.x, floating.y, floating.z, floating.sizeH, floating.sizeV);
    floating.setAnimations("block", "../Models/Environment/ground/", "movingBlock", 0, 20);

    this->currentX += 2;

    return floating;
}

EnemyMet Scene::spawnEnemyMet() {
    EnemyMet enemy;
    enemy.setX(this->currentX);
    enemy.setY(0);
    enemy.setZ(-6);
    enemy.setSize(1);
    enemy.setScaleSizeModel(4);

    double heightCollisionY = 0.5;
    enemy.collision.y = heightCollisionY;
    enemy.collision.setSize(enemy.sizeH + 0.2f, enemy.sizeV);

    enemy.mapCollider = Object ::createRetangleCollider(enemy.collision.x, enemy.collision.y, enemy.collision.z, enemy.collision.sizeH, enemy.collision.sizeV);

    enemy.setSizeVision(10, 4);
    enemy.offSetShootX = 1;
    enemy.offSetShootY = 0.5;

    enemy.setAnimations("idle", "../Models/Enemies/metalMet/", "metalMet", 0, 20);
    enemy.setAnimations("hidden", "../Models/Enemies/metalMet/", "metalMetHidden", 0, 20);
    return enemy;
}

EnemyHorizontal Scene::spawnHorizontalEnemy() {
    EnemyHorizontal enemy;
    enemy.setX(this->currentX);
    enemy.setY(-0.5);
    enemy.setZ(-6);
    enemy.setSize(1, 2);

    enemy.setScaleSizeModel(0.5);

    enemy.speed.x = 0.01;
    enemy.collision.setSize(enemy.sizeH + 0.2f, enemy.sizeV);
    enemy.mapCollider = Object ::createRetangleCollider(enemy.collision.x, enemy.collision.y, enemy.collision.z, enemy.collision.sizeH, enemy.collision.sizeV);

    enemy.setSizeVision(10, 5);
    enemy.offSetShootY = 0.5;

    enemy.setAnimations("idle", "../Models/Enemies/horizontalAirPlane/", "horizontalAirPlane", 0, 20);

    return enemy;
}

EnemyVertical Scene::spawnVerticalEnemy(){
    EnemyVertical enemy;
    enemy.setX(this->currentX);
    enemy.setY(1);
    enemy.setZ(-6);
    enemy.setSize(1);

    enemy.speed.y = 0.01;
    enemy.collision.setSize(enemy.sizeH + 0.2f);

    enemy.setSizeVision(20, 5);

    enemy.mapCollider = Object ::createRetangleCollider(enemy.collision.x, enemy.collision.y, enemy.collision.z, enemy.collision.sizeH, enemy.collision.sizeV);
    enemy.setAnimations("idle", "../Models/Enemies/rounderingEnemy/", "rounderingEnemy", 0, 20);
    return enemy;
}

EnemyJumping Scene::spawnJumpingEnemy() {
    EnemyJumping enemy;
    enemy.setX(this->currentX);
    enemy.setY(0);
    enemy.setZ(-6);
    enemy.setSize(1);

    enemy.speed.y = 0.01;
    enemy.collision.setSize(enemy.sizeH + 0.2f);
    enemy.modelY = enemy.y - 1;

    enemy.setSizeVision(10, 5);

    enemy.mapCollider = Object ::createRetangleCollider(enemy.collision.x, enemy.collision.y, enemy.collision.z, enemy.collision.sizeH, enemy.collision.sizeV);
    enemy.setAnimations("idle", "../Models/Enemies/jumperEnemy/", "jumperEnemy", 0, 20);
    return enemy;
}

#endif //GAME_PROJECT_SCENE_H
