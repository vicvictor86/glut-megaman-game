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
    public: double scaleSizeModelX = 1, scaleSizeModelY = 1, scaleSizeModelZ = 1, rotateModelX = 0, rotateModelY = 0, rotateModelZ = 0, angle = 0;
    public: double modelX=0, modelY=0, modelZ=0;
    public: map<string, vector<Model>> animations;
    public: map<string, int> animationFPS;
    public: string animationStatus = "idle";
    public: void drawModel(double scaleSize);
    public: void drawObject(const string& animationName, int animationFrame, double x, double y, double z, double sizeH, double sizeV,  double angle, double r, double g, double b, bool drawCollider);

    public: static map<char, double> createRetangleCollider(double x, double y, double z, double sizeH, double sizeV);
    public: virtual void setModel(const string& path);
    public: void setX(double updateX);
    public: void setY(double updateY);
    public: void setZ(double updateZ);
    public: void setSize(float sizeH, float sizeV);
    public: void setScaleSizeModel(double scaleSizeModelX, double scaleSizeModelY, double scaleSizeModelZ);
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

void Object:: drawObject(const string& animationName="", int animationFrame=1, double x = 0, double y = 0, double z = -6, double sizeH = 1, double sizeV = 1, double angle = 90, double r=0, double g=0, double b=0, bool drawCollider = false){
    if(drawCollider) {
        glPushMatrix();
            glColor3d(r, g, b);
            glTranslated(x, y, z);
            glScaled(sizeH, sizeV, sizeH);
            glutWireCube(1);
        glPopMatrix();
    }

    if(!(animationName.empty())) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef((float) x, (float) y, (float) z);
        glRotatef(angle, 0, 1, 0);
        glScaled(sizeH, sizeV, sizeH);

        if (!this->animations[animationName].empty()) {
            if (animationFrame > this->animations[animationName].size()) {
                animationFrame = 0;
            }
            this->animations[animationName][animationFrame].draw();
        } else {
            //                cout << "A animacao " << animationName << " nao existe" << endl;
        }
        glPopMatrix();
    }

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

void Object:: setScaleSizeModel(double scaleSizeModelX, double scaleSizeModelY=0, double scaleSizeModelZ=0){
    if (scaleSizeModelY == 0) {
        scaleSizeModelY = scaleSizeModelX;
    }

    if (scaleSizeModelZ == 0) {
        scaleSizeModelZ = scaleSizeModelX;
    }

    this->scaleSizeModelX = scaleSizeModelX;
    this->scaleSizeModelY = scaleSizeModelY;
    this->scaleSizeModelZ = scaleSizeModelZ;
}

#endif