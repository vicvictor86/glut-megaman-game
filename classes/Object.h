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
    public: float sizeH = 1;
    public: float sizeV = 1;
    public: map<char, double> mapCollider;
    public: Collision collision;
    public: string tag;
    public: Model model;
    public: double scaleSizeModel = 1;
    public: map<string, vector<Model>> animations;
    public: map<string, int> animationFPS;
    public: void drawModel(double scaleSize);
    public: static void drawObject(double x, double y, double z, double sizeH, double sizeV, double r, double g, double b);
    public: static map<char, double> createRetangleCollider(double x, double y, double z, double sizeH, double sizeV);
    public: virtual void setModel(const string& path);
    public: void setX(double updateX);
    public: void setY(double updateY);
    public: void setZ(double updateZ);
    public: void setSize(float sizeH, float sizeV);
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
    this->sizeH = size;
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

map<char, double> Object:: createRetangleCollider(double x, double y, double z, double sizeH, double sizeV=-1){
    if(sizeV == -1){
        sizeV = sizeH;
    }

    map<char, double> mapColliders;
    mapColliders.insert(pair<char, double>('L', x - (sizeH / 2)));
    mapColliders.insert(pair<char, double>('R', x + (sizeH / 2)));
    mapColliders.insert(pair<char, double>('T', y + (sizeV / 2)));
    mapColliders.insert(pair<char, double>('B', y - (sizeV / 2)));

    return mapColliders;
}

void Object:: drawObject(double x, double y, double z, double sizeH, double sizeV, double r=0, double g=0, double b=0){
    glPushMatrix();
        glColor3d(r, g, b);
        glTranslated(x, y, z);
        glScaled(sizeH, sizeV, z);
        glutWireCube(1);
    glPopMatrix();
}

void Object:: drawModel(double scaleSize){
    glPushMatrix();
        glColor3d(1, 1, 1);
        glLoadIdentity();
        glTranslated(this->x, this->y, this->z);
        glScaled(scaleSize, scaleSize, scaleSize);
        this->model.draw();
    glPopMatrix();
}

#endif