#ifndef GAME_PROJECT_CAMERA_H
#define GAME_PROJECT_CAMERA_H

class Camera {
    public: int x=0, y=0, z=0;
    public: int width=0, height=1;
    public: float eyeX = 0;
    public: float eyeY = 0;
    public: float eyeZ = 5;
    public: float centerX = 0;
    public: float centerY = 0;
    public: float centerZ = 0;
    public: float upX = 0;
    public: float upY = 1;
    public: float upZ = 0;
    public: float fov = 60;
    public: float aspect = (float)width/(float)height;
    public: float nearZ = 1.0;
    public: float farZ = 20.0;
    public: Camera(int width, int height);
};

Camera :: Camera(int width, int height) {
    this->width = width;
    this->height = height;
};

#endif //GAME_PROJECT_CAMERA_H
