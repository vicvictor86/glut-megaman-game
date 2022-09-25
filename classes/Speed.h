#ifndef SPEED_H
#define SPEED_H

class Speed {
    public: double x=0, y=0, z=0;
    public: Speed() = default;
    public: Speed(double x, double y, double z);
};

Speed:: Speed(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
};

#endif