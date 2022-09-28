#include "Object.h"
#include "WallWithCollider.h"

#ifndef GAME_PROJECT_SCENE_H
#define GAME_PROJECT_SCENE_H

class Scene {
public: WallWithCollider buildFloorBlock();
public: WallWithCollider buildRaisedBlock(int level);
public: void buildHole();
public: Scene() {currentX = 0;};
private: int currentX;
};

WallWithCollider Scene::buildFloorBlock() {
    Object wall;
    wall.x = this->currentX;
    wall.y = -2;
    wall.z = -6;
    wall.setSize(2);

    WallWithCollider wallWithCollider;
    wallWithCollider.wallObject = wall;
    wallWithCollider.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.sizeH, wall.sizeV);

    this->currentX += 2;

    return wallWithCollider;
}

void Scene::buildHole() {
    this->currentX += 2;
}

WallWithCollider Scene::buildRaisedBlock(int level) {
    Object wall;
    wall.x = this->currentX;
    switch(level) {
        case 0:
            wall.y = level;
            break;
        case 1:
            wall.y = 2;
            break;
        case 2:
            wall.y = 4;
            break;
        default:
            break;
    }

    wall.z = -6;
    wall.setSize(2);

    WallWithCollider wallWithCollider;
    wallWithCollider.wallObject = wall;
    wallWithCollider.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.sizeH);

    return wallWithCollider;
}

#endif //GAME_PROJECT_SCENE_H
