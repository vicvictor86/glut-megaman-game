cmake_minimum_required(VERSION 3.23)
project(game_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(classes)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(game_project
        classes/Collision.h
        classes/fire.h
        classes/Object.h
        classes/player.h
        classes/Speed.h
        megaman.cpp)

target_link_libraries(${PROJECT_NAME} -Wall -Wextra -lglut32 -lglu32 -lopengl32 -lwinmm)