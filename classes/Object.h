#include "speed.h"
#include "Collision.h"
#include <map>
using namespace std; 

#ifndef OBJECT_H
#define OBJECT_H

class Object {
    public: double x=0, y=0, z=0;
    public: float r=0, g=0, b=0;
    public: Speed speed;
    public: float size = 1;
    public: map<char, double> mapCollider;
    public: Collision collision;
    public: string tag;
    public: Model model;
    public: map<string, vector<Model>> animations;
    public: map<string, int> animationFPS;
    public: void drawnModel(double scaleSize);
    public: static void drawnObject(double x, double y, double z, double size);
    public: static map<char, double> createRetangleCollider(double x, double y, double z, double size);
    public: virtual void setModel(const string& path);
    public: void setX(double updateX);
    public: void setY(double updateY);
    public: void setZ(double updateZ);
    public: void setSize(float size);
    public: void setAnimations(const string& animationName, const string& directoryPath, const string& fileName, int numberOfFrames, int fps);
    public: Object() = default;
    public: Object(double x, double y, double z, float r, float g, float b, Speed speed, float size, Collision collision);

};

Object :: Object(double x, double y, double z, float r, float g, float b, Speed speed, float size, Collision collision){
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
    this->speed = Speed(speed.x, speed.y, speed.z);
    this->size = size;
    this->collision = collision;
}

void Object:: setX(double updateX){
    this->x = updateX;
}

void Object:: setY(double updateY){
    this->y = updateY;
}

void Object:: setZ(double updateZ){
    this->z = updateZ;
    this->collision.z = updateZ;
}

void Object:: setSize(float updateSize){
    this->size = updateSize;
    this->collision.size = updateSize;
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

map<char, double> Object:: createRetangleCollider(double x, double y, double z, double size){
    map<char, double> mapColliders;
    mapColliders.insert(pair<char, double>('L', x - (size / 2)));
    mapColliders.insert(pair<char, double>('R', x + (size / 2)));
    mapColliders.insert(pair<char, double>('T', y + (size / 2)));
    mapColliders.insert(pair<char, double>('B', y - (size / 2)));

    return mapColliders;
}

void Object:: drawnObject(double x, double y, double z, double size){
    glPushMatrix();
        glTranslated(x, y, z);
        glutWireCube(size);
    glPopMatrix();
}

void Object:: drawnModel(double scaleSize){
    glPushMatrix();
        glLoadIdentity();
        glTranslated(this->x, this->y, this->z);
        glScaled(scaleSize, scaleSize, scaleSize);
        this->model.draw();
    glPopMatrix();
}

#endif