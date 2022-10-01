cmake_minimum_required(VERSION 3.23)
project(game_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(classes Models)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(game_project
        classes/Model.h
        classes/Collision.h
        classes/Fire.h
        classes/Object.h
        classes/Player.h
        classes/Speed.h
        classes/Enemies/Enemy.h
        classes/Camera.h
        megaman.cpp classes/Enemies/EnemyHorizontal.h classes/Enemies/EnemyVertical.h classes/Enemies/EnemyDepth.h classes/Enemies/EnemiesImport.h classes/Enemies/EnemyJumping.h classes/Sounds.h classes/Enemies/EnemyMet.h classes/Wall.h)

target_link_libraries(${PROJECT_NAME} -Wall -Wextra -lglut32 -lglu32 -lopengl32 -lwinmm)