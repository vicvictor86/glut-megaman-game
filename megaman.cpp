#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-slicing"
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
#include "classes/FloatingBlocksHor.h"
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

int actualFps = 60;

bool keyBuffer[256];
vector<Fire> fireObjects;
vector<Wall *> walls;
vector<Enemy*> enemies;

Player player(0, 0, -6, 1, 1, 1, Speed(0, 0, 0), 0.5, 10, 1, 3, Collision(0, 1.1, -6, 0.5, 2.2));
Object playerMenu(0, 0, -6, 1, 1, 1, Speed(0, 0, 0), 0.5, Collision(0, 1.1, -6, 0.5, 2.2));
Camera camera(WIDTH, HEIGHT);
Menu menu;
Scene scene;

string actualAnimation = "idle";
string shootType = "shoot";
int framesInIdle = 0;

bool debug = false;

enum status
{
    mainMenu,
    onGame,
    gamePaused,
    gameOptions,
    playerDeath,
    playerWon
};

status gameStatus = mainMenu;

int frameAnimation = 0;

static void showMenu();

void countFps(){
    frameCount++;
    countFpsFinalTime = time(nullptr);
    if(countFpsFinalTime - countFpsInitialTime > 0){
        actualFps = frameCount / (countFpsFinalTime - countFpsInitialTime);
        printf("FPS: %d\n", actualFps);
        frameCount = 0;
        countFpsInitialTime = countFpsFinalTime;
    }
}

void playerDead() {
    Sounds::stopSounds();
    Sounds::playSound("death", false);
    player.life = player.maxLife;
    player.setY(0);
    player.setX(0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    vector<string> options = {"Reiniciar Jornada", "Sair para o Menu"};
    menu.setOptions(options);

    menu.setOption(0);
    gameStatus = playerDeath;
    glutDisplayFunc(showMenu);
    printf("Game over\n");
}

void winGame() {
    Sounds::stopSounds();
    Sounds::playSound("death", false);
    player.life = player.maxLife;
    player.setY(0);
    player.setX(0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    vector<string> options = {"(Aperte Enter para continuar)"};
    menu.setOptions(options);

    menu.setOption(0);
    gameStatus = playerWon;
    glutDisplayFunc(showMenu);
    printf("Game win\n");
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

        collisionDirections typeCollision = Collision::checkCollision(object->mapCollider, object->x, object->y, walls[i]->mapColliderWall, 0, 0, lastIteration, &quantityOverLapping);

        if(typeCollision == RIGHTCOLLISION){
            object->x = walls[i]->mapColliderWall['L'] - object->collision.sizeH / 2 - object->collision.x;
            printf("Colidiu na direita do object\n");
        }
        else if(typeCollision == LEFTCOLLISION){
            object->x = walls[i]->mapColliderWall['R'] + object->collision.sizeH / 2 - object->collision.x;
            printf("Colidiu na esquerda do object\n");
        }

        if(typeCollision == TOPCOLLISION){
            object->y = walls[i]->mapColliderWall['B'] - object->collision.sizeV / 2 - object->collision.y;
            object->speed.y = 0;
            printf("Colidiu em cima do object\n");
        }
        else if(typeCollision == BOTTOMCOLLISION){
            object->collision.isOnPlataform = true;
            object->y = walls[i]->mapColliderWall['T'] + object->collision.sizeV / 2 - object->collision.y;
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

            fireObjects[i].drawFire(debug);

            collisionDirections collideWithPlayer = Collision::checkCollision(player.mapCollider, player.x, player.y, fireObjects[i].mapCollider, fireObjects[i].x, fireObjects[i].y, i + 1 >= fireObjects.size(), &quantityOverLapping);
            if(collideWithPlayer != NOCOLLISION && collideWithPlayer != NULLCOLLISION && fireObjects[i].tagShoot == "Enemy"){
                player.getDamage(fireObjects[i].damage);
                fireObjects.erase(fireObjects.begin() + i);
                if(player.life <= 0){
                    playerDead();
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

void checkCollisionWithEnemies(){
    int quantityOverLapping = 0;
    for(int i = 0; i < enemies.size(); i++){
        collisionDirections collideWithPlayer = Collision::checkCollision(player.mapCollider, player.x, player.y, enemies[i]->mapCollider, enemies[i]->x, enemies[i]->y, i + 1 >= enemies.size(), &quantityOverLapping);
        if(collideWithPlayer != NOCOLLISION && collideWithPlayer != NULLCOLLISION){
            player.getDamage(enemies[i]->damage);
            if(player.life <= 0){
                playerDead();
            }
        }
    }
}

void drawLifeHud(){
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 1, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            glBegin(GL_QUADS);
                glColor3d(1.0, 0.0, 0.0);
                double percentLifeReduction = (double)player.life / player.maxLife;
                double xQuadLeft = 0.05;
                double xQuadRight = 0.3 * percentLifeReduction;
                double yQuadBottom = 0.85;
                double yQuadTop = 0.95;
                glTexCoord2d(0, 1); glVertex2d(xQuadLeft, yQuadBottom);
                glTexCoord2d(1, 1); glVertex2d(xQuadRight, yQuadBottom);
                glTexCoord2d(1, 0); glVertex2d(xQuadRight, yQuadTop);
                glTexCoord2d(0, 0); glVertex2d(xQuadLeft, yQuadTop);
            glEnd();
            glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void executeAnimation(bool *animationCondition, const string& animationName, Object animationObject, bool isLoop=false, bool lockInEnd=false){
    if(animationCondition == nullptr){
        return;
    }

    if(player.isShooting && animationName != "shoot" && animationName != "chargShoot"){
        return;
    }

    if(*animationCondition && animationName != actualAnimation){
        frameAnimation = 0;
    }

    if(*animationCondition && animationName != "idle" && animationName != "sadIdle"){
        framesInIdle = 0;
    }

    if(*animationCondition && isLoop){
        countFramesShootAnimationFinalTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        if(countFramesShootAnimationFinalTime - countFramesShootAnimationInitialTime >= animationObject.animationFPS[actualAnimation] && frameAnimation < animationObject.animations[actualAnimation].size()){
            actualAnimation = animationName;
            frameAnimation++;
            countFramesShootAnimationInitialTime = countFramesShootAnimationFinalTime;

            if(actualAnimation == "idle" || actualAnimation == "sadIdle"){
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

        return;
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

    playerMenu.drawObject("running", frameAnimation, -3, 0, -6, 1.2, 1.2, 1, 1, 1);
    bool playerIsMoving = true;
    executeAnimation(&playerIsMoving, "running", player, true);

    float adjustmentX, adjustmentY;
    switch (gameStatus) {
        case mainMenu:
            adjustmentX = -0.68;
            adjustmentY = 0.7;
            break;
        case gamePaused:
            adjustmentX = -0.78;
            adjustmentY = 0.45;
            break;
        case playerDeath:
            adjustmentX = -0.68;
            adjustmentY = 0.7;
            break;
        case gameOptions:
            adjustmentX = -0.68;
            adjustmentY = 0.7;
            break;
        case playerWon:
            adjustmentX = -0.68;
            adjustmentY = 0.7;
            break;
    }
    menu.openMenu(player.x + adjustmentX,  player.y + adjustmentY);
    glutSwapBuffers();

}

void showRayCast(Enemy *enemy){
    if(debug){
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
            glPushMatrix();
            glBegin(GL_LINE_LOOP);
                glColor3d(1, 0, 0);
                glVertex3d(enemy->x - 1, enemy->y, -6);
                glVertex3d(player.x, enemy->y, -6);
            glEnd();

            glBegin(GL_LINE_LOOP);
                glColor3d(1, 0, 0);
                glVertex3d(enemy->x - 1, enemy->y, -6);
                glVertex3d(enemy->x - 1, player.y + player.collision.sizeV/2, -6);
            glEnd();
            glPopMatrix();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    }
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    drawLifeHud();

    if(player.x >= 141 && enemies.empty()) {
        winGame();
    }

    frameAnimation = player.drawPlayer(actualAnimation, frameAnimation, 1.5, debug);

    int quantityOverLapping = checkCollisionWithWalls(&player);
    checkCollisionWithEnemies();

    for (auto & wall : walls){
//        wall->drawObject("",1, wall->x, wall->y, wall->z, wall->sizeH, wall->sizeV);
        wall->drawWall("block", 0,  debug);
        wall->move();
    }

    for (auto & enemy : enemies){
        enemy->drawEnemy(enemy->animationStatus, player, 0, enemy->scaleSizeModelX, 0, 0, debug);
        checkCollisionWithWalls(enemy);
        enemy->move();
        enemy->noticedEnemy(enemy->animationStatus, 0, player.mapCollider, player.x, player.y, player.z, debug);
        enemy->shoot(&fireObjects, player, actualFps);
        showRayCast(enemy);
    }

    if (player.y <= -18) {
        playerDead();
    }

    map<string, bool> animationsConditions;
    bool idleForTooLong = framesInIdle >= 400;
    executeAnimation(&idleForTooLong, "sadIdle", player, true);

    bool playerIsMovingInGround = player.speed.isMoving() && !player.speed.isInTheAir();
    executeAnimation(&playerIsMovingInGround, "running", player, true);

    bool isInTheAir = player.speed.isInTheAir();
    executeAnimation(&isInTheAir, "jumping", player, false, true);

    chargingShott();
    executeAnimation(&player.isShooting, shootType, player);

    vector<bool> animationCondition = {player.isShooting, playerIsMovingInGround, idleForTooLong, isInTheAir};
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

    checkCollisionsFires(quantityOverLapping);

    updateCamera();

    countFps();

    glutSwapBuffers();

}

static void key(unsigned char key, int x, int y) {
    keyBuffer[key] = true;

    vector<string> options;

    if (gameStatus == mainMenu) {
        if (key == 13) {
            switch (menu.getOption()) {
                case 0:
                    gameStatus = onGame;
                    player.x = 0;
                    player.y = 0;
                    cout << "Jogo Iniciado\n";
                    frameAnimation = 0;
                    actualAnimation = "idle";

                    Sounds::stopSounds();
                    Sounds::playSound("background", true);

                    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                    glutDisplayFunc(display);
                    break;
                case 1:
                    options = {"Volume  <  " + to_string(menu.getSoundSetting()) + "%  >", "Retornar"};
                    menu.setOptions(options);
                    menu.setOption(0);
                    gameStatus = gameOptions;
                    break;
                case 2:
                    exit(0);
                    break;
            }

        } else if (key == 'w' || key == 'W') {
            menu.switchOption(-1);
        } else if (key == 's' || key == 'S') {
            menu.switchOption(1);
        }
    } else if (gameStatus == gamePaused) {
        if (key == 13) {
            switch (menu.getOption()) {
                case 0:
                    gameStatus = onGame;
                    cout << "Jogo Iniciado\n";
                    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                    glutDisplayFunc(display);
                    break;
                case 1:
                    Sounds::stopSounds();
                    Sounds::playSound("menu", true);
                    gameStatus = mainMenu;
                    options = {"Iniciar Jornada", "Ajustes", "Sair do Jogo"};
                    menu.setOptions(options);
                    menu.setOption(0);
                    break;
            }

        } else if (key == 'w' || key == 'W') {
            menu.switchOption(-1);
        } else if (key == 's' || key == 'S') {
            menu.switchOption(1);
        }
    } else if (gameStatus == playerDeath) {
        if (key == 13) {
            switch (menu.getOption()) {
                case 0:
                    Sounds::stopSounds();
                    Sounds::playSound("background", true);
                    gameStatus = onGame;
                    cout << "Jogo Reiniciado\n";
                    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                    glutDisplayFunc(display);
                    break;
                case 1:
                    Sounds::stopSounds();
                    Sounds::playSound("menu", true);
                    gameStatus = mainMenu;
                    vector<string> options = {"Iniciar Jornada", "Ajustes", "Sair do Jogo"};
                    menu.setOptions(options);
                    menu.setOption(0);
                    break;
            }

        } else if (key == 'w' || key == 'W') {
            menu.switchOption(-1);
        } else if (key == 's' || key == 'S') {
            menu.switchOption(1);
        }
    } else if (gameStatus == gameOptions) {
        if (menu.getOption() == 0) {
            if (key == 'a' || key == 'A') {
                menu.updateSoundSetting(-1);
                options = {"Volume  <  " + to_string(menu.getSoundSetting()) + "%  >", "Retornar"};
                menu.setOptions(options);
                Sounds::setVolume((float) menu.getSoundSetting() / 100);
            } else if (key == 'd' || key == 'D') {
                menu.updateSoundSetting(1);
                options = {"Volume  <  " + to_string(menu.getSoundSetting()) + "%  >", "Retornar"};
                menu.setOptions(options);
                Sounds::setVolume((float) menu.getSoundSetting() / 100);
            }
            cout << menu.getSoundSetting() / 100 << endl;
        }

            if (key == 'w' || key == 'W')
                menu.switchOption(-1);
            else if (key == 's' || key == 'S')
                menu.switchOption(1);

            if (key == 13) {
                switch (menu.getOption()) {
                    case 1:
                        gameStatus = mainMenu;
                        vector<string> options = {"Iniciar Jornada", "Ajustes", "Sair do Jogo"};
                        menu.setOptions(options);
                        menu.setOption(0);
                        break;
                }
            }
        } else if (gameStatus == onGame) {
            if (keyBuffer[27]) {
                frameAnimation = 0;
                actualAnimation = "running";

                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                vector<string> options = {"Continuar Jornada", "Menu Principal"};
                menu.setOptions(options);

                menu.setOption(0);
                gameStatus = gamePaused;
                cout << "Jogo Pausado\n";
                glutDisplayFunc(showMenu);
            } else {
                if (keyBuffer['d'] || keyBuffer['D']) {
                    player.speed.x = 0.1;
                    player.directionX = RIGHT;
                }

                if (keyBuffer['a'] || keyBuffer['A']) {
                    player.speed.x = 0.1;
                    player.directionX = LEFT;
                }

                if (keyBuffer[' '] && player.speed.y == 0) {
                    Sounds::playSound("jump");
                    player.speed.y = 0.05f;
                }

                if (!fireObjects.empty()) {
                    if (keyBuffer['g']) {
                        fireObjects.pop_back();
                    }
                }

                if (keyBuffer['f']) {
                    if (initialTime == -1) {
                        initialTime = time(nullptr);
                    }
                }

                if (key == 'q' || key == 'Q') {
                    exit(0);
                }
            }
        } else if(gameStatus == playerWon) {
        if (key == 13) {
            switch (menu.getOption()) {
                case 1:
                    gameStatus = mainMenu;
                    vector<string> options = {"Iniciar Jornada", "Ajustes", "Sair do Jogo"};
                    menu.setOptions(options);
                    menu.setOption(0);
                    break;
            }
        }
    }

    glutPostRedisplay();
}

static void keyboardUp(unsigned char key, int x, int y)
{
    keyBuffer[key] = false;
    
    if(gameStatus != onGame) return;

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

    if((!keyBuffer['c'] && key == 'c') || (keyBuffer['C'] && key == 'C')){
        debug = !debug;
    }

    if (!keyBuffer['o'] && key == 'o'){
        player.x = 0;
        player.y = 0;
    }
}

static void specialKey(int key, int x, int y)
{
    if(gameStatus == mainMenu || gameStatus == gamePaused) {
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
    } else if(gameStatus == gameOptions) {
        switch (key) {
            case GLUT_KEY_LEFT:
                menu.updateSoundSetting(-1);
                break;
            case GLUT_KEY_RIGHT:
                menu.updateSoundSetting(1);
                break;
        }
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


    playerMenu.setAnimations("running", "../Models/PlayerModel/animations/runningAnimation/", "running", 20, 20);

//    player.setAnimations("idle", "../Models/PlayerModel/animations/idleAnimation/", "idle", 60, 20);
//    player.setAnimations("shoot", "../Models/PlayerModel/animations/shootAnimation/", "shooting", 21, 10);
//    player.setAnimations("chargShoot", "../Models/PlayerModel/animations/chargShootAnimation/", "chargedShoot", 24, 20);
//    player.setAnimations("running", "../Models/PlayerModel/animations/runningAnimation/", "running", 20, 20);
//    player.setAnimations("jumping", "../Models/PlayerModel/animations/jumpingAnimation/", "jumping", 26, 20);
//    player.setAnimations("sadIdle", "../Models/PlayerModel/animations/sadIdleAnimation/", "sadIdle", 78, 20);


    player.mapCollider = Object:: createRetangleCollider(player.collision.x, player.collision.y, player.collision.z, player.collision.sizeH, player.collision.sizeV);

    enum sceneComponents {
        Floor,
        Wall1,
        Wall2,
        Hole,
        SmallHole,
        SmallWall,
        MetEnemy,
        HorizontalEnemy,
        VerticalEnemy,
        JumpingEnemy,
        BlockFloatingHor
    };

    vector<sceneComponents> componentsScene = {};
    for(int i = 0; i <= 60; i++) {
        if(i == 24) {
            componentsScene.push_back(Wall1);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Hole);
        } else if(i == 20) {
            componentsScene.push_back(HorizontalEnemy);
            componentsScene.push_back(Floor);
        }
        else if (i == 14) {
            componentsScene.push_back(Hole);
            componentsScene.push_back(BlockFloatingHor);
            componentsScene.push_back(Hole);
            componentsScene.push_back(Hole);
            componentsScene.push_back(Hole);
            componentsScene.push_back(Hole);
            componentsScene.push_back(BlockFloatingHor);
            componentsScene.push_back(Hole);
            componentsScene.push_back(VerticalEnemy);
        } else if(i == 8) {
            componentsScene.push_back(HorizontalEnemy);
        } else if(i == 45) {
            componentsScene.push_back(SmallWall);
            componentsScene.push_back(MetEnemy);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
            componentsScene.push_back(JumpingEnemy);
        } else if(i == 30) {
            componentsScene.push_back(JumpingEnemy);
        } else if(i == 38) {
            componentsScene.push_back(JumpingEnemy);
        } else if(i == 52) {
            componentsScene.push_back(VerticalEnemy);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
            componentsScene.push_back(HorizontalEnemy);
        } else if(i == 58){
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
            componentsScene.push_back(Floor);
        } else if (i == 59) {
            componentsScene.push_back(Wall1);
            componentsScene.push_back(Wall2);
        } else
            componentsScene.push_back(Floor);
    }

    for(auto & i : componentsScene) {
        switch (i) {
            case Floor:
                walls.push_back(new Wall(scene.buildFloorBlock()));
                break;
            case Wall1:
                walls.push_back(new Wall(scene.buildRaisedBlock(0)));
                break;
            case Wall2:
                walls.push_back(new Wall(scene.buildRaisedBlock(1)));
                break;
            case Hole:
                scene.buildHole();
                break;
            case SmallHole:
                scene.buildHole(1);
                break;
            case SmallWall:
                walls.push_back(new Wall(scene.buildRaisedBlock(-0.5, 1)));
                break;
            case MetEnemy:
                enemies.push_back(new EnemyMet(scene.spawnEnemyMet()));
                break;
            case HorizontalEnemy:
                enemies.push_back(new EnemyHorizontal(scene.spawnHorizontalEnemy()));
                break;
            case VerticalEnemy:
                enemies.push_back(new EnemyVertical(scene.spawnVerticalEnemy()));
                break;
            case JumpingEnemy:
                enemies.push_back(new EnemyJumping(scene.spawnJumpingEnemy()));
                break;
            case BlockFloatingHor:
                walls.push_back(new FloatingBlocksHor(scene.buildFloatBlockHor()));
                break;

        }
    }

    Sounds::playSound("menu", true);

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

    glEnable(GL_LIGHTING);

//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

//    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    init();

    glutMainLoop();

    return EXIT_SUCCESS;
}

#pragma clang diagnostic pop