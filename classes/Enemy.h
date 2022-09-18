#include "Object.h"

#ifndef GAME_PROJECT_ENEMY_H
#define GAME_PROJECT_ENEMY_H

class Enemy : public Object {
    public: int life;
    public: float damage;
    public: map<char, double> mapCollider;
    public: Enemy(){};
};

#endif //GAME_PROJECT_ENEMY_H
