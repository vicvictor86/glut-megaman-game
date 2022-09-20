#ifndef GAME_PROJECT_CAMERA_H
#define GAME_PROJECT_CAMERA_H

class Camera {
    public: int x=0, y=0, z=0;
    public: int width=0, height=1;
    public: double eyeX = 0;
    public: double eyeY = 0;
    public: double eyeZ = 5;
    public: double centerX = 0;
    public: double centerY = 0;
    public: double centerZ = 0;
    public: double upX = 0;
    public: double upY = 1;
    public: double upZ = 0;
    public: double fov = 60;
    public: double aspect = width/height;
    public: double nearZ = 1.0;
    public: double farZ = 20.0;
    public: Camera(int width, int height);
};

Camera :: Camera(int width, int height) {
    this->width = width;
    this->height = height;
};

#endif //GAME_PROJECT_CAMERA_H
