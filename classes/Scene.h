#include <cstdio>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

#ifndef GAME_PROJECT_SCENES_H
#define GAME_PROJECT_SCENES_H

class Scene {
private: vector<string> options;
private: int option;
public: Scene(){option = 0;};
public: void openMenu();
public: vector<string> getOptions();
public: void setOptions(vector<string> options);
public: int getOption();
public: void switchOption(int op);
};

static void writeOnScreen(double x, double y, string text)
{
    glRasterPos2f(x, y);
    for(int i = 0; i < (int)text.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

vector<string> Scene::getOptions() {
    return this->options;
}

void Scene::setOptions(vector<string> options) {
    this->options = options;
}

void Scene::openMenu(void) {
    float x = -0.75;
    float y = 0.6;
    for(int i = 0; i < this->getOptions().size(); i++) {
        if(option == i)
            glColor3f(0.0f,0.0f,1.0f);
        else
            glColor3f(1.0f,1.0f,1.0f);

        writeOnScreen(x, y, options[i]);
        y -= 0.7;
    }
}

int Scene::getOption(){
    return this->option;
}

void Scene::switchOption(int op) {
    if(this->option == 0 && op == -1) {
        this->option = getOptions().size() - 1;
    } else if (this->option == getOptions().size() - 1 && op == 1){
        this->option = 0;
    } else {
        this->option += op;
    }
}



#endif //GAME_PROJECT_SCENES_H
