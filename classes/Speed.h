#ifndef SPEED_H
#define SPEED_H

class Speed {
    public: float x=0, y=0, z=0;
    public: Speed() = default;
    public: Speed(float x, float y, float z);
};

Speed:: Speed(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
};

#endif