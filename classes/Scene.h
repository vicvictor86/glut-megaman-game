#include "Object.h"
#include "WallWithCollider.h"

#ifndef GAME_PROJECT_SCENE_H
#define GAME_PROJECT_SCENE_H

class Scene {
public: WallWithCollider buildFloorBlock();
public: void buildRaisedBlock();
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
    wallWithCollider.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.sizeH);

    this->currentX += 2;

    return wallWithCollider;
}

#endif //GAME_PROJECT_SCENE_H
