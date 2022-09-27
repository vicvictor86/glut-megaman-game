#include "classes/Model.h"

// Biblioteca com funcoes matematicas
#include <cstdio>
#include <cstdlib>

//Imports proprios
#include <typeinfo>
#include <ctime>
#include <vector>
#include <Windows.h>
#include <iostream>
#include "classes/Fire.h"
#include "classes/Player.h"
#include "classes/Collision.h"
#include "Enemies/EnemiesImport.h"
#include "classes/Camera.h"
#include "classes/Scene.h"
#include "classes/Menu.h"
#include "classes/Sounds.h"
#include "WallWithCollider.h"
#include <chrono>
#pragma comment(lib, "Winmm.lib")


#define FPS 70
#define SHOOTKEY 'j'

using namespace std; 

int initialTime = -1;
int countFpsInitialTime = time(nullptr), countFpsFinalTime, frameCount, cooldDownWallJump = 1, initialWallJump = -1;
uint64_t countFramesShootAnimationFinalTime, countFramesShootAnimationInitialTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

int WIDTH = 640;
int HEIGHT = 480;

bool keyBuffer[256];
vector<Fire> fireObjects;
vector<WallWithCollider> walls;
vector<Enemy*> enemies;

Player player(0, 0, -6, 1, 1, 1, Speed(0, 0, 0), 0.5, 10, 1, 3, Collision(0, 1.1, -6, 0.5, 2.2));
Camera camera(WIDTH, HEIGHT);
Menu menu;
Scene scene;

string actualAnimation = "idle";
string shootType = "shoot";
int framesInIdle = 0;

bool gameStarted = false;

int frameAnimation = 0;

void countFps(){
    frameCount++;
    countFpsFinalTime = time(nullptr);
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

int checkCollisionWithWalls(Object * object){
    //Desenho de paredes e detecção de colisão
    int quantityOverLapping = 0;
    for(int i = 0; i < walls.size(); i++){
        bool lastIteration = i + 1 >= walls.size();

        collisionDirections typeCollision = Collision::checkCollision(object->mapCollider, object->x, object->y, walls[i].mapColliderWall, 0, 0, lastIteration, &quantityOverLapping);

        if(typeCollision == RIGHTCOLLISION){
            object->x = walls[i].mapColliderWall['L'] - object->collision.sizeH / 2 - object->collision.x;
            printf("Colidiu na direita do object\n");
        }
        else if(typeCollision == LEFTCOLLISION){
            object->x = walls[i].mapColliderWall['R'] + object->collision.sizeH / 2 - object->collision.x;
            printf("Colidiu na esquerda do object\n");
        }

        if(typeCollision == TOPCOLLISION){
            object->y = walls[i].mapColliderWall['B'] - object->collision.sizeV / 2 - object->collision.y;
            object->speed.y = 0;
            printf("Colidiu em cima do object\n");
        }
        else if(typeCollision == BOTTOMCOLLISION){
            object->collision.isOnPlataform = true;
            object->y = walls[i].mapColliderWall['T'] + object->collision.sizeV / 2 - object->collision.y;
            object->speed.y = 0;
            printf("Colidiu em baixo do object\n");
        }

        if(typeCollision == NOCOLLISION || typeCollision == RIGHTCOLLISION || typeCollision == LEFTCOLLISION || typeCollision == TOPCOLLISION){
            object->collision.isOnPlataform = false;
        }

        if(keyBuffer[' '] && initialWallJump == -1){
            object->speed.y = 0.05;
            initialWallJump = time(nullptr);
        }

        int finalWallJump = time(nullptr);
        if((typeCollision == RIGHTCOLLISION || typeCollision == LEFTCOLLISION) && finalWallJump - initialWallJump >= cooldDownWallJump && keyBuffer[' ']){
            Sounds::playSound("jump");
            printf("Wall jump\n");
            object->speed.y = 0.05;
            initialWallJump = -1;
        }
    }

    return quantityOverLapping;
}

void checkCollisionsFires(int quantityOverLapping){
    if (!fireObjects.empty()) {
        for (int i = 0; i < fireObjects.size(); i++) {
            bool isAlive = fireObjects[i].isAlive();

            if(!isAlive){
                fireObjects.erase(fireObjects.begin() + i);
            }

            fireObjects[i].drawFire(true);

            collisionDirections collideWithPlayer = Collision::checkCollision(player.mapCollider, player.x, player.y, fireObjects[i].mapCollider, fireObjects[i].x, fireObjects[i].y, i + 1 >= fireObjects.size(), &quantityOverLapping);
            if(collideWithPlayer != NOCOLLISION && collideWithPlayer != NULLCOLLISION && fireObjects[i].tagShoot == "Enemy"){
                player.getDamage(fireObjects[i].damage);
                fireObjects.erase(fireObjects.begin() + i);
                if(player.life <= 0){
                    printf("Game over\n");
                    exit(0);
                }
            }

            for(int j = 0; j < enemies.size(); j++) {
                collisionDirections fireCollideWithEnemy = Collision::checkCollision(fireObjects[i].mapCollider, fireObjects[i].x, fireObjects[i].y, enemies[j]->mapCollider, enemies[j]->x, enemies[j]->y, j + 1 >= enemies.size(), &quantityOverLapping);
                if(fireCollideWithEnemy != NOCOLLISION && fireCollideWithEnemy != NULLCOLLISION){
                    if(fireObjects[i].tagShoot == "Player"){
                        enemies[j]->getDamage(player.damage);

                        if(enemies[j]->life <= 0){
                            delete enemies[j];
                            enemies.erase(enemies.begin() + j);
                        }

                        fireObjects.erase(fireObjects.begin() + i);
                        break;
                    }
                }
            }

        }
    }
}

void drawnLifeHud(){
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            glColor3d(1.0, 0.0, 0.0);
            double percentLifeReduction = (double)player.life / player.maxLife;
            double xQuadLeft = -8;
            double xQuadRight = xQuadLeft + 5.0 * percentLifeReduction;
            double yQuadTop = 5.5;
            double yQuadBottom = yQuadTop - 1;
            glVertex3d(xQuadLeft + player.x, yQuadBottom + player.y, -6);
            glVertex3d(xQuadRight + player.x, yQuadBottom + player.y, -6);
            glVertex3d(xQuadRight + player.x, yQuadTop + player.y, -6);
            glVertex3d(xQuadLeft + player.x, yQuadTop + player.y, -6);
        glEnd();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void executeAnimation(bool *animationCondition, string animationName, Object animationObject, bool isLoop=false, bool lockInEnd=false){
    if(*animationCondition && isLoop){
        countFramesShootAnimationFinalTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        if(countFramesShootAnimationFinalTime - countFramesShootAnimationInitialTime >= animationObject.animationFPS[actualAnimation] && frameAnimation < animationObject.animations[actualAnimation].size()){
            actualAnimation = animationName;
            frameAnimation++;
            countFramesShootAnimationInitialTime = countFramesShootAnimationFinalTime;

            if(actualAnimation == "idle"){
                framesInIdle++;
            }
        }

        if(frameAnimation >= animationObject.animations[actualAnimation].size()){
            frameAnimation = 0;
        }
        return;
    }

    if(*animationCondition && lockInEnd){
        if(frameAnimation > animationObject.animations[actualAnimation].size()){
            frameAnimation = 0;
        }

        countFramesShootAnimationFinalTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        if(countFramesShootAnimationFinalTime - countFramesShootAnimationInitialTime >= animationObject.animationFPS[actualAnimation] && frameAnimation < animationObject.animations[actualAnimation].size() - 1){
            actualAnimation = animationName;
            frameAnimation++;
            countFramesShootAnimationInitialTime = countFramesShootAnimationFinalTime;
        }

        return;
    }

    if(*animationCondition){
        countFramesShootAnimationFinalTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        if(countFramesShootAnimationFinalTime - countFramesShootAnimationInitialTime >= animationObject.animationFPS[actualAnimation] && frameAnimation < animationObject.animations[actualAnimation].size()){
            actualAnimation = animationName;
            frameAnimation++;
            countFramesShootAnimationInitialTime = countFramesShootAnimationFinalTime;

            framesInIdle = 0;
        }

        if(frameAnimation >= animationObject.animations[actualAnimation].size()){
            frameAnimation = 0;
            *animationCondition = false;
            actualAnimation = "idle";
        }
    }
}

void chargingShott(){
    if (keyBuffer[SHOOTKEY]){
        if(initialTime == -1){
            initialTime = time(nullptr);
        }

        int actualTime = time(nullptr);
        int pastTime = actualTime - initialTime;
        float percent = (float)(pastTime + 1) / (float)player.timeChargedShot;

        if(player.g >= 0.01 && player.b >= 0.01 && pastTime > 0){
            player.g = 1 - player.g * percent;
            player.b = 1 - player.b * percent;
        }
    }
}

static void showMenu(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    menu.openMenu();

    glutSwapBuffers();
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor4f(1, 0, 0, 1);
    glLoadIdentity();

    drawnLifeHud();

    glColor3d(1, 1, 1);
    player.drawnPlayer(actualAnimation, frameAnimation, 1.5, true);

    int quantityOverLapping = checkCollisionWithWalls(&player);

    for (auto & wall : walls){
        Object ::drawnObject(wall.wallObject.x, wall.wallObject.y, wall.wallObject.z, wall.wallObject.sizeH);
    }

    for (auto & enemy : enemies){
        Enemy ::drawnObject(enemy->x, enemy->y, enemy->z, enemy->sizeH);
        checkCollisionWithWalls(enemy);
        enemy->move();
        enemy->shoot(&fireObjects);
        enemy->noticedEnemy(player.mapCollider, player.x, player.y, player.z, 2, false);
    }

    checkCollisionsFires(quantityOverLapping);

    executeAnimation(&player.isShooting, shootType, player);

    bool playerIsMoving = player.speed.isMoving();
    executeAnimation(&playerIsMoving, "running", player, true);

    bool idleForTooLong = framesInIdle >= 400;
    executeAnimation(&idleForTooLong, "sadIdle", player, true);

    bool isInTheAir = player.speed.isInTheAir();
    cout << isInTheAir << endl;
    executeAnimation(&isInTheAir, "jumping", player, false, true);

    vector<bool> animationCondition = {player.isShooting, playerIsMoving, idleForTooLong, isInTheAir};
    for(int i = 0; i < animationCondition.size(); i++){
        if(animationCondition[i]){
            break;
        }

        if(i == animationCondition.size() - 1){
            bool idleAnimation = true;
            executeAnimation(&idleAnimation, "idle", player, true);
        }
    }

    player.move(keyBuffer);
    chargingShott();

    updateCamera();

    countFps();

    glutSwapBuffers();

}

static void key(unsigned char key, int x, int y)
{
    keyBuffer[key] = true;

    if(!gameStarted) {
        if(key == 13) {
            switch(menu.getOption()) {
                case 0:
                    gameStarted = true;
                    cout << "Jogo Iniciado\n";
                    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                    glutDisplayFunc(display);
                    break;
                case 1:
                    break;
                case 2:
                    exit(0);
                    break;
            }

        }
    }

    if(!gameStarted) {
        if(key == 'w' || key =='W') {
            menu.switchOption(-1);
        } else if(key == 's' || key == 'S') {
            menu.switchOption(1);
        }
        return;
    }

    if(key == 'q' || key == 'Q') {
        exit(0);
    }

    if (keyBuffer['d'] || keyBuffer['D']) {
        player.speed.x = 0.1;
        player.directionX = RIGHT;
    }

    if (keyBuffer['a'] || keyBuffer['A']) {
        player.speed.x = 0.1;
        player.directionX = LEFT;
    }

    if (keyBuffer[' '] && player.speed.y == 0){
        Sounds::playSound("jump");
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

    glutPostRedisplay();

}

static void keyboardUp(unsigned char key, int x, int y)
{
    keyBuffer[key] = false;
    cout << key << endl;

    if(!gameStarted) return;

    if (!keyBuffer[SHOOTKEY] && key == SHOOTKEY){
        player.isShooting = true;
        shootType = "shoot";
        Sounds::playSound("shoot");

        Fire fire;
        int finalTime = time(nullptr);
        if(finalTime - initialTime >= player.timeChargedShot) {
            printf("Tiro carregado\n");
            fire.chargedFire = true;
            shootType = "chargShoot";
        }
        initialTime = -1;

        player.r = 1;
        player.g = 1;
        player.b = 1;

        double spawnPoint;
        float shootSpeed;
        double shootDistance = 1.2;

        if(player.directionX == RIGHT){
            spawnPoint = player.x + shootDistance;
            shootSpeed = 0.06f;
        } else if(player.directionX == LEFT){
            spawnPoint = player.x - shootDistance;
            shootSpeed = -0.06f;
        }

        double spawnPointY = player.y + 1.5;
        float radiusOfFire = 0.5;

        fire.x = spawnPoint;
        fire.y = spawnPointY;
        fire.z = player.z;

        if(fire.chargedFire){
            fire.speed.x = shootSpeed * 2;
            fire.sizeH = radiusOfFire * 2;
            fire.collision.sizeH = 0.55 * 2;
            fire.damage = 2;
        }
        else{
            fire.speed.x = shootSpeed;
            fire.sizeH = radiusOfFire;
            fire.collision.sizeH = 0.55;
            fire.damage = 1;
        }

        fire.slicesAndStacks = 16;
        fire.collision.x = 0;
        fire.collision.y = 0;
        fire.collision.z = fire.z;

        fire.tagShoot = "Player";

        fireObjects.push_back(fire);
    }

    if (!keyBuffer['o'] && key == 'o'){
        player.x = 0;
        player.y = 0;
    }
}

static void specialKey(int key, int x, int y)
{
    if(!gameStarted) {
        switch (key) {
            case GLUT_KEY_UP:
                menu.switchOption(-1);
                break;
            case GLUT_KEY_DOWN:
                menu.switchOption(1);
                break;
            default:
                break;
        }
        glutPostRedisplay();
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    vector<string> options = {"Iniciar Jornada", "Ajustes", "Sair do Jogo"};

    menu.setOptions(options);

    player.setAnimations("idle", "../Models/PlayerModel/animations/idleAnimation/", "idle", 60, 20);
//    player.setAnimations("shoot", "../Models/PlayerModel/animations/shootAnimation/", "shoot", 27, 10);
//    player.setAnimations("chargShoot", "../Models/PlayerModel/animations/chargShootAnimation/", "chargShoot", 27, 10);
    player.setAnimations("running", "../Models/PlayerModel/animations/runningAnimation/", "running", 20, 20);
    player.setAnimations("jumping", "../Models/PlayerModel/animations/jumpingAnimation/", "jumping", 26, 20);
//    player.setAnimations("sadIdle", "../Models/PlayerModel/animations/sadIdleAnimation/", "sadIdle", 78, 20);


    player.mapCollider = Object:: createRetangleCollider(player.collision.x, player.collision.y, player.collision.z, player.collision.sizeH, player.collision.sizeV);

    for(int i = 0; i < 3; i++) {
        WallWithCollider wallWithCollider = scene.buildFloorBlock();
        walls.push_back(wallWithCollider);
    }

    EnemyHorizontal enemy1;
    enemy1.setX(4);
    enemy1.setY(0);
    enemy1.setZ(player.z);
    enemy1.setSize(1);
    enemy1.speed.x = 0.01;
    enemy1.collision.setSize(enemy1.sizeH + 0.2f);
    enemy1.mapCollider = Object ::createRetangleCollider(enemy1.collision.x, enemy1.collision.y, enemy1.collision.z, enemy1.collision.sizeH);
    enemies.push_back(new EnemyHorizontal(enemy1));

    EnemyVertical enemy2;
    enemy2.setX(8);
    enemy2.setY(0);
    enemy2.setZ(player.z);
    enemy2.setSize(1);
    enemy2.speed.y = 0.01;
    enemy2.collision.setSize(enemy2.sizeH + 0.2f);
    enemy2.mapCollider = Object ::createRetangleCollider(enemy2.collision.x, enemy2.collision.y, enemy2.collision.z, enemy2.collision.sizeH);
    enemies.push_back(new EnemyVertical(enemy2));

    EnemyMet enemy3;
    enemy3.setX(2);
    enemy3.setY(0);
    enemy3.setZ(player.z);
    enemy3.setSize(1);
    enemy3.speed.z = 0.01;
    enemy3.collision.setSize(enemy3.sizeH + 0.2f);
    enemy3.mapCollider = Object ::createRetangleCollider(enemy3.collision.x, enemy3.collision.y, enemy3.collision.z, enemy3.collision.sizeH);
    enemies.push_back(new EnemyMet(enemy3));
}

/* Program entry point */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Mega Man");

    glutReshapeFunc(resize);
    glutDisplayFunc(showMenu);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKey);
    glutTimerFunc(1000/FPS, idle, 0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
//    glEnable(GL_LIGHTING);
//
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

//    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    init();

//    Sounds::playSound("background");

    glutMainLoop();

    return EXIT_SUCCESS;
}
