#include "speed.h"
#include "Collision.h"
#include <map>
using namespace std; 

#ifndef OBJECT_H
#define OBJECT_H

class Object {
    public: float x=0, y=0, z=0;
    public: float r=0, g=0, b=0;
    public: Speed speed;
    public: float sizeH = 1;
    public: float sizeV = 1;
    public: map<char, float> mapCollider;
    public: Collision collision;
    public: string tag;
    public: Model model;
    public: map<string, vector<Model>> animations;
    public: map<string, int> animationFPS;
    public: void drawnModel(float scaleSize);
    public: static void drawnObject(float x, float y, float z, float size);
    public: static map<char, float> createRetangleCollider(float x, float y, float z, float sizeH, float sizeV);
    public: virtual void setModel(const string& path);
    public: void setX(float updateX);
    public: void setY(float updateY);
    public: void setZ(float updateZ);
    public: void setSize(float sizeH, float sizeV);
    public: void setAnimations(const string& animationName, const string& directoryPath, const string& fileName, int numberOfFrames, int fps);
    public: Object() = default;
    public: Object(float x, float y, float z, float r, float g, float b, Speed speed, float size, Collision collision);

};

Object :: Object(float x, float y, float z, float r, float g, float b, Speed speed, float size, Collision collision){
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
    this->speed = Speed(speed.x, speed.y, speed.z);
    this->sizeH = size;
    this->collision = collision;
}

void Object:: setX(float updateX){
    this->x = updateX;
}

void Object:: setY(float updateY){
    this->y = updateY;
}

void Object:: setZ(float updateZ){
    this->z = updateZ;
    this->collision.z = updateZ;
}

void Object:: setSize(float updateSizeH, float updateSizeV=-1){
    if(updateSizeV == -1){
        updateSizeV = updateSizeH;
    }
    this->sizeH = updateSizeH;
    this->collision.sizeH = updateSizeH;
    this->sizeV = updateSizeV;
    this->collision.sizeV = updateSizeV;
}

void Object:: setModel(const string& path){
    this->model.load(path.c_str());
}

void Object:: setAnimations(const string& animationName, const string& directoryPath, const string& fileName, int numberOfFrames, int fps) {
    vector<Model> animation;
    for (int i = 1; i <= numberOfFrames; i++) {
        Model actualModel;
        actualModel.load((directoryPath + fileName + to_string(i) + ".obj").c_str());
        animation.push_back(actualModel);
    }

    if(numberOfFrames == 0){
        Model actualModel;
        actualModel.load((directoryPath + fileName + ".obj").c_str());
        animation.push_back(actualModel);
    }

    this->animations[animationName] = animation;
    this->animationFPS[animationName] = fps;
}

map<char, float> Object:: createRetangleCollider(float x, float y, float z, float sizeH, float sizeV=-1){
    if(sizeV == -1){
        sizeV = sizeH;
    }

    map<char, float> mapColliders;
    mapColliders.insert(pair<char, float>('L', x - (sizeH / 2)));
    mapColliders.insert(pair<char, float>('R', x + (sizeH / 2)));
    mapColliders.insert(pair<char, float>('T', y + (sizeV / 2)));
    mapColliders.insert(pair<char, float>('B', y - (sizeV / 2)));

    return mapColliders;
}

void Object:: drawnObject(float x, float y, float z, float size){
    glPushMatrix();
        glTranslatef(x, y, z);
        glutWireCube(size);
    glPopMatrix();
}

void Object:: drawnModel(float scaleSize){
    glPushMatrix();
        glLoadIdentity();
        glTranslated(this->x, this->y, this->z);
        glScalef(scaleSize, scaleSize, scaleSize);
        this->model.draw();
    glPopMatrix();
}

#endif