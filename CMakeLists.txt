cmake_minimum_required(VERSION 3.23)
project(game_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(classes Models)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(game_project
        classes/Model.h
        classes/Collision.h
        classes/fire.h
        classes/Object.h
        classes/player.h
        classes/Speed.h
        classes/Enemy.h
        classes/Camera.h
        megaman.cpp classes/EnemyHorizontal.h classes/EnemyVertical.h classes/EnemyDepth.h classes/EnemiesImport.h classes/EnemyJumping.h)

target_link_libraries(${PROJECT_NAME} -Wall -Wextra -lglut32 -lglu32 -lopengl32 -lwinmm)