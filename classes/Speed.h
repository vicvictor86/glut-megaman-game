#ifndef SPEED_H
#define SPEED_H

class Speed {
    public: double x, y, z;
    public: Speed(){};
    public: Speed(double x, double y, double z);
};

Speed:: Speed(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
};

#endif