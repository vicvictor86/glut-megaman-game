/*
 * FreeGLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in FreeGLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

// Bibliotecas utilizadas pelo OpenGL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Biblioteca com funcoes matematicas
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>

//Imports proprios
#include <vector>
#include "classes/fire.h"
#include "classes/player.h"
#include "classes/Collision.h"

using namespace std; 

static int slices = 16;
static int stacks = 16;

int width = 640;
int height = 480;

struct WallWithCollider {
    Object wall;
    map<char, double> mapColliderWall;
};

bool keyBuffer[256];
vector<Fire> fireObjects;
vector<WallWithCollider> walls;

Player player(0, 0, -6, 1, 0, 0, Speed(0, 0, 0), 1, 16, 1, 0, Collision(0, 0, -6, 1, 0, 0, 1));

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void display(void)
{
    // const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1, 0, 0);
    
    //Player
    glPushMatrix();
        glTranslated(0, 0, player.z);
        glRotated(90, 1, 0, 0);
        glutSolidCube(0.5);
    glPopMatrix();

    //Collision Cube
    glPushMatrix();
        glTranslated(0, 0, player.z);
        glRotated(90, 1, 0, 0);
        glutWireCube(1); 
    glPopMatrix();

    glPushMatrix();
        glTranslated(-4-player.x, 0-player.y, -6.5);
        glRotated(90, 1, 0, 0);
        glutWireCube(2);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0-player.x, -2-player.y, -6.5);
        glRotated(90, 1, 0, 0);
        glutWireCube(2);
    glPopMatrix();

    glPushMatrix();
        glTranslated(2-player.x, -1-player.y, -6.5);
        glRotated(90, 1, 0, 0);
        glutWireCube(2);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0-player.x, 2-player.y, -6.5);
    glRotated(90, 1, 0, 0);
    glutWireCube(2);
    glPopMatrix();

    // Juntar desenho com collider

    int quantityOverLapping = 0;
    for(int i = 0; i < walls.size(); i++){
        bool isOver = player.mapColliderPlayer['L'] + player.x <= walls[i].mapColliderWall['R']  &&  player.mapColliderPlayer['R'] + player.x >= walls[i].mapColliderWall['L']  &&
                      player.mapColliderPlayer['B'] + player.y <= walls[i].mapColliderWall['T']  &&
                      player.mapColliderPlayer['T'] + player.y >= walls[i].mapColliderWall['B'];
        if(isOver){
            quantityOverLapping++;
            if(player.mapColliderPlayer['R'] + player.x > walls[i].mapColliderWall['L'] && player.mapColliderPlayer['L'] + player.x < walls[i].mapColliderWall['L'] && player.mapColliderPlayer['T'] + player.y - 0.1 > walls[i].mapColliderWall['B'] && player.mapColliderPlayer['B'] + player.y + 0.1 < walls[i].mapColliderWall['T']){
                printf("Colidiu na direita do player\n");
                player.x = walls[i].mapColliderWall['L'] - 0.51;
            }
            else if(player.mapColliderPlayer['L'] + player.x < walls[i].mapColliderWall['R'] && player.mapColliderPlayer['R'] + player.x > walls[i].mapColliderWall['R'] && player.mapColliderPlayer['T'] + player.y - 0.1 > walls[i].mapColliderWall['B'] && player.mapColliderPlayer['B'] + player.y + 0.1 < walls[i].mapColliderWall['T']){
                printf("Colidiu na esquerda do player\n");

                player.x = walls[i].mapColliderWall['R'] + 0.51;
            }
            if(player.mapColliderPlayer['T'] + player.y > walls[i].mapColliderWall['B'] && player.mapColliderPlayer['R'] + player.x - 0.1 > walls[i].mapColliderWall['L'] && player.mapColliderPlayer['L'] + player.x + 0.1 < walls[i].mapColliderWall['R'] && player.mapColliderPlayer['T'] + player.y < walls[i].mapColliderWall['T']){
                printf("Colidiu em cima do player\n");

//                printf("%f %f\n", player.mapColliderPlayer['T'] + player.y, walls[i].mapColliderWall['B']);
                player.y = walls[i].mapColliderWall['B'] - 0.51;
                player.speed.y = 0;
//            printf("%f %f\n", player.mapColliderPlayer['T'] + player.y, walls[i].mapColliderWall['B']);
            }
            else if(player.mapColliderPlayer['B'] + player.y < walls[i].mapColliderWall['T'] && player.mapColliderPlayer['R'] + player.x - 0.1 > walls[i].mapColliderWall['L'] && player.mapColliderPlayer['L'] + player.x + 0.1 < walls[i].mapColliderWall['R'] && player.mapColliderPlayer['B'] + player.y > walls[i].mapColliderWall['B']){
                printf("Colidiu em baixo do player\n");

                player.collision.isOnPlataform = true;
                player.y = walls[i].mapColliderWall['T'] + 0.5;
                player.speed.y = 0;
            }
//        printf("%f %f\n", mapCollidersPlayer['B'] + player.y, mapCollidersWall2['T']);
        }

        bool lastIteration = i + 1 >= walls.size();
        bool notOverlapping = quantityOverLapping == 0;
        if (notOverlapping && lastIteration){
            player.collision.isOnPlataform = false;
        }
    }

    player.move(keyBuffer);

    if (fireObjects.size() > 0) {
        for (int i = 0; i < fireObjects.size(); i++) { 
            glPushMatrix();
                glTranslated(fireObjects[i].x, fireObjects[i].y, fireObjects[i].z);
                glRotated(90, 1, 0, 0);
                glutSolidSphere(fireObjects[i].size, fireObjects[i].slicesAndStacks, fireObjects[i].slicesAndStacks);
            glPopMatrix();

            glPushMatrix();
                glTranslated(fireObjects[i].collision.x + fireObjects[i].x, fireObjects[i].collision.y, fireObjects[i].collision.z);
                glRotated(90, 1, 0, 0);
                glutWireSphere(fireObjects[i].collision.size, fireObjects[i].slicesAndStacks, fireObjects[i].slicesAndStacks);
            glPopMatrix();
            fireObjects[i].x += fireObjects[i].speed.x;
        }
    }

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    keyBuffer[key] = true;

    if(key == 'q' || key == 'Q') {
        exit(0);
    }

    if (keyBuffer['d'] || keyBuffer['D']) {
        player.speed.x = 0.1;   
        player.direction = RIGHT;
    }

    if (keyBuffer['a'] || keyBuffer['A']) {
        player.speed.x = 0.1;
        player.direction = LEFT;
    }

    if (keyBuffer[' '] && player.speed.y == 0){
        player.speed.y = 0.05f;
    }

    if (keyBuffer['g']){
        fireObjects.pop_back();
    }

}

static void keyboardUp(unsigned char key, int x, int y)
{
    keyBuffer[key] = false;

    if (key == 'f'){
        double distanceOfPlayer = 2;
        double heightOfPlayer = 0;
        float shootSpeed = 0.1;
        float radiusOfFire = 0.5;
        Fire fire;
        fire.x = distanceOfPlayer;
        fire.y = heightOfPlayer;
        fire.z = player.z;
        fire.speed.x = shootSpeed;
        fire.size = radiusOfFire;
        fire.slicesAndStacks = 16;
        fire.collision.x = 0;
        fire.collision.y = 0;
        fire.collision.z = fire.z;
        fire.collision.size = 0.55;
        fireObjects.push_back(fire);
    }
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_position[] = {2.0f, 5.0f, 5.0f, 0.0f};

const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};

void init(){
    player.mapColliderPlayer = Object:: createRetangleCollider(0, 0, player.z, 1);

    WallWithCollider wallWithCollider1;
    Object wall1;
    wall1.x = 0;
    wall1.y = -2;
    wall1.z = player.z;
    wall1.size = 2;
    wallWithCollider1.wall = wall1;
    wallWithCollider1.mapColliderWall = Object ::createRetangleCollider(wall1.x, wall1.y, wall1.z, wall1.size);
    walls.push_back(wallWithCollider1);

    WallWithCollider wallWithCollider2;
    Object wall2;
    wall2.x = 2;
    wall2.y = -1;
    wall2.z = player.z;
    wall2.size = 2;
    wallWithCollider2.wall = wall2;
    wallWithCollider2.mapColliderWall = Object ::createRetangleCollider(wall2.x, wall2.y, wall2.z, wall2.size);
    walls.push_back(wallWithCollider2);
}

/* Program entry point */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    init();

    glutCreateWindow("Mega Man");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);

    glClearColor(1, 1, 1, 1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
