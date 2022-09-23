#include <cstdio>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

#ifndef GAME_PROJECT_SCENES_H
#define GAME_PROJECT_SCENES_H

class Scene {
public: Scene(){};
public: static void menu(void);

};

static void writeOnScreen(double x, double y, string text)
{
    glRasterPos2f(x, y);
    for(int i = 0; i < (int)text.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void Scene::menu(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    glColor3f(0.0f,0.0f,1.0f);
    writeOnScreen(-0.8, 0.5, "Iniciar Jornada");
    writeOnScreen(-0.8, -0.2, "Sair do Jogo");

    glutSwapBuffers();

}

#endif //GAME_PROJECT_SCENES_H
