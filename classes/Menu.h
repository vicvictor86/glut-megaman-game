#include <cstdio>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

#ifndef GAME_PROJECT_SCENES_H
#define GAME_PROJECT_SCENES_H

class Menu {
private: vector<string> options;
private: int option;
private: int soundSetting;
public: Menu(){ option = 0; soundSetting = 60;};
public: void openMenu(float x, float y);
public: vector<string> getOptions();
public: void setOptions(vector<string> options);
public: void setOption(int option);
public: int getOption();
public: int getSoundSetting();
public: void updateSoundSetting(int op);
public: void switchOption(int op);
};

static void writeOnScreen(double x, double y, string text)
{
    glRasterPos2f(x, y);
    for(char i : text){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);
    }
}

vector<string> Menu::getOptions() {
    return this->options;
}

void Menu::setOptions(vector<string> ops) {
    this->options = ops;
}

void Menu::setOption(int op) {
    this->option = op;
}

void Menu::openMenu(float x, float y) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    for(int i = 0; i < this->getOptions().size(); i++) {
        if(option == i)
            glColor3f(0.0f,0.0f,1.0f);
        else
            glColor3f(1.0f,1.0f,1.0f);
        writeOnScreen(x, y, options[i]);
        y -= 0.7;
    }
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

int Menu::getOption(){
    return this->option;
}

int Menu::getSoundSetting(){
    return this->soundSetting;
}

void Menu::updateSoundSetting(int op) {
    if (this->soundSetting > 0 && this->soundSetting < 100)
        this->soundSetting += 10 * op;
    else {
        if (this->soundSetting == 0 && op == 1)
            this->soundSetting += 10;
        else if (this->soundSetting == 100 && op == -1)
            this->soundSetting -= 10;
    }
}

void Menu::switchOption(int op) {
    if(this->option == 0 && op == -1) {
        this->option = getOptions().size() - 1;
    } else if (this->option == getOptions().size() - 1 && op == 1){
        this->option = 0;
    } else {
        this->option += op;
    }
}

#endif //GAME_PROJECT_SCENES_H
