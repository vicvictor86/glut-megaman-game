#include "Object.h"

class Fire : public Object
{
    public: int slicesAndStacks;   
    public: Fire(){};
    public: Fire(double x, double y, double z, float r, float g, float b, Speed speed, float size, int slicesAndStacks, Collision collision) : Object(x, y, z, r, g, b, speed, size, collision){
        this->slicesAndStacks = slicesAndStacks;
    };
};