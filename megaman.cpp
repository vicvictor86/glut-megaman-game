// Bibliotecas utilizadas pelo OpenGL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Biblioteca com funcoes matematicas
#include <math.h>
#include <cstdio>
#include <cstdlib>

//Imports proprios
#include <ctime>
#include <vector>
#include "classes/fire.h"
#include "classes/player.h"
#include "classes/Collision.h"
#include "classes/Enemy.h"
#include "classes/Camera.h"

#define FPS 70

using namespace std; 

static int slices = 16;
static int stacks = 16;
int initialTime = -1;
int countFpsInitialTime = time(nullptr), countFpsFinalTime, frameCount, cooldDownWallJump = 1, initialWallJump = -1;

int WIDTH = 640;
int HEIGHT = 480;

struct WallWithCollider {
    Object wall;
    map<char, double> mapColliderWall;
};

bool keyBuffer[256];
vector<Fire> fireObjects;
vector<WallWithCollider> walls;
vector<Enemy> enemies;

Player player(0, 0, -6, 1, 0, 0, Speed(0, 0, 0), 1, 16, 1, 3, Collision(0, 0, -6, 1, 0, 0, 1));
Camera camera(WIDTH, HEIGHT);

/* GLUT callback Handlers */

void countFps(){
    frameCount++;
    countFpsFinalTime = time(NULL);
    if(countFpsFinalTime - countFpsInitialTime > 0){
        printf("FPS: %d\n", frameCount / (countFpsFinalTime - countFpsInitialTime));
        frameCount = 0;
        countFpsInitialTime = countFpsFinalTime;
    }
}

static void resize(int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    camera.width = width;
    camera.height = height > 0 ? height : camera.height;
    camera.aspect = (double)camera.width / (double)camera.height;
    glMatrixMode (GL_PROJECTION);
        glLoadIdentity();
        glViewport (camera.x, camera.y, camera.width, camera.height);

        gluPerspective(camera.fov, camera.aspect, camera.nearZ, camera.farZ);

        gluLookAt(camera.eyeX,camera.eyeY, camera.eyeZ,camera.centerX,camera.centerY,camera.centerZ,camera.upX,camera.upY,camera.upZ);
    glMatrixMode (GL_MODELVIEW);
}

void updateCamera(){
    camera.eyeX = player.x;
    camera.centerX = player.x;
    camera.eyeY = player.y;
    camera.centerY = player.y;
    glMatrixMode (GL_PROJECTION);
        glLoadIdentity();
        glViewport (camera.x, camera.y, camera.width, camera.height);

        gluPerspective(camera.fov, camera.aspect, camera.nearZ, camera.farZ);

        gluLookAt(camera.eyeX,camera.eyeY, camera.eyeZ,camera.centerX,camera.centerY,camera.centerZ,camera.upX,camera.upY,camera.upZ);
    glMatrixMode (GL_MODELVIEW);
}

static void display()
{
    // const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1, 0, 0);

    player.drawPlayer(player.x, player.y, -6, 0.5, true, 1);

    //Desenho de paredes e detecção de colisão
    int quantityOverLapping = 0;
    for(int i = 0; i < walls.size(); i++){
        bool lastIteration = i + 1 >= walls.size();

        collisionDirections typeCollision = Collision::checkCollision(player.mapColliderPlayer, player.x, player.y, walls[i].mapColliderWall, lastIteration, &quantityOverLapping);

        if(typeCollision == RIGHTCOLLISION){
            player.x = walls[i].mapColliderWall['L'] - 0.51;
            printf("Colidiu na direita do player\n");
        }
        else if(typeCollision == LEFTCOLLISION){
            player.x = walls[i].mapColliderWall['R'] + 0.51;
            printf("Colidiu na esquerda do player\n");
        }

        if(typeCollision == TOPCOLLISION){
            player.y = walls[i].mapColliderWall['B'] - 0.51;
            player.speed.y = 0;
            printf("Colidiu em cima do player\n");
        }
        else if(typeCollision == BOTTOMCOLLISION){
            player.collision.isOnPlataform = true;
            player.y = walls[i].mapColliderWall['T'] + 0.5;
            player.speed.y = 0;
            printf("Colidiu em baixo do player\n");
        }

        if(typeCollision == NOCOLLISION || typeCollision == RIGHTCOLLISION || typeCollision == LEFTCOLLISION || typeCollision == TOPCOLLISION){
            player.collision.isOnPlataform = false;
        }

        if(keyBuffer[' '] && initialWallJump == -1){
            player.speed.y = 0.05;
            initialWallJump = time(nullptr);
        }

        int finalWallJump = time(nullptr);
        if((typeCollision == RIGHTCOLLISION || typeCollision == LEFTCOLLISION) && finalWallJump - initialWallJump >= cooldDownWallJump && keyBuffer[' ']){
            printf("Wall jump\n");
            player.speed.y = 0.05;
            initialWallJump = -1;
        }
    }

    for (auto & wall : walls){
        Object ::drawnObject(wall.wall.x, wall.wall.y, wall.wall.z, wall.wall.size);
    }

    for (auto & enemie : enemies){
        Enemy ::drawnObject(enemie.x, enemie.y, enemie.z, enemie.size);
    }

    if (!fireObjects.empty()) {
        for (int i = 0; i < fireObjects.size(); i++) {
            fireObjects[i].mapCollider = Object::createRetangleCollider(fireObjects[i].collision.x, fireObjects[i].collision.y, fireObjects[i].collision.z, fireObjects[i].collision.size);
            fireObjects[i].drawFire(player.x, player.y, true);

            for(int j = 0; j < enemies.size(); j++){
                collisionDirections typeCollision = Collision::checkCollision(fireObjects[i].mapCollider, fireObjects[i].x, fireObjects[i].y, enemies[j].mapCollider, j + 1 >= enemies.size(), &quantityOverLapping);
                if(typeCollision != NOCOLLISION && typeCollision != NULLCOLLISION){
                    enemies.erase(enemies.begin() + j);
                    fireObjects.erase(fireObjects.begin() + i);
                    break;
                }
            }
        }
    }

    player.move(keyBuffer);

    updateCamera();

    glutSwapBuffers();

    countFps();
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

    if (!fireObjects.empty()) {
        if (keyBuffer['g']) {
            fireObjects.pop_back();
        }
    }

    if (keyBuffer['f']){
        if(initialTime == -1){
            initialTime = time(nullptr);
        }
    }
}

static void keyboardUp(unsigned char key, int x, int y)
{
    keyBuffer[key] = false;

    if (!keyBuffer['f'] && key == 'f'){
        Fire fire;

        int finalTime = time(nullptr);
        if(finalTime - initialTime >= 2) {
            printf("Tiro carregado\n");
            fire.chargedFire = true;
        }
        initialTime = -1;

        double spawnPoint = player.x + 1;
        double heightOfPlayer = player.y + 0;
        float shootSpeed = 0.06f;
        float radiusOfFire = 0.5;

        fire.x = spawnPoint;
        fire.y = heightOfPlayer;
        fire.z = player.z;

        if(fire.chargedFire){
            fire.speed.x = shootSpeed * 2;
            fire.size = radiusOfFire * 2;
            fire.collision.size = 0.55 * 2;
        }
        else{
            fire.speed.x = shootSpeed;
            fire.size = radiusOfFire;
            fire.collision.size = 0.55;
        }

        fire.slicesAndStacks = 16;
        fire.collision.x = 0;
        fire.collision.y = 0;
        fire.collision.z = fire.z;

        fireObjects.push_back(fire);
    }

    if (!keyBuffer['o'] && key == 'o'){
        player.x = 0;
        player.y = 0;
    }
}

static void idle(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, idle, 0);
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

    vector<Object> tempWalls;

    Object wall1;
    wall1.x = 0;
    wall1.y = -2;
    wall1.z = player.z;
    wall1.size = 2;
    tempWalls.push_back(wall1);

    Object wall2;
    wall2.x = 2;
    wall2.y = -2;
    wall2.z = player.z;
    wall2.size = 2;
    tempWalls.push_back(wall2);

    Object wall3;
    wall3.x = -2;
    wall3.y = 0;
    wall3.z = player.z;
    wall3.size = 2;
    tempWalls.push_back(wall3);

    Object wall4;
    wall4.x = 4;
    wall4.y = -2;
    wall4.z = player.z;
    wall4.size = 2;
    tempWalls.push_back(wall4);

    Object wall5;
    wall5.x = 0;
    wall5.y = 2;
    wall5.z = player.z;
    wall5.size = 2;
    tempWalls.push_back(wall5);

    for (auto & tempWall : tempWalls){
        Object wall;
        WallWithCollider wallWithCollider;
        wall = tempWall;
        wallWithCollider.wall = wall;
        wallWithCollider.mapColliderWall = Object ::createRetangleCollider(wall.x, wall.y, wall.z, wall.size);
        walls.push_back(wallWithCollider);
    }

    Enemy enemy1;
    enemy1.x = 4;
    enemy1.y = 0;
    enemy1.z = player.z;
    enemy1.size = 1;
    enemy1.mapCollider = Object ::createRetangleCollider(enemy1.x, enemy1.y, enemy1.z, enemy1.size);
    enemies.push_back(enemy1);
}

/* Program entry point */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    init();

    glutCreateWindow("Mega Man");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(1000/FPS, idle, 0);

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
