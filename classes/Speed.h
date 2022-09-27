#ifndef SPEED_H
#define SPEED_H

class Speed {
    public: double x=0, y=0, z=0;
    public: bool isMoving();
    public: Speed() = default;
    public: Speed(double x, double y, double z);
};

Speed:: Speed(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
};

bool Speed:: isMoving(){
    return this->x != 0 || this->y != 0 || this->z != 0;
}

#endif