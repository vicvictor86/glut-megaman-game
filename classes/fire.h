#include "Object.h"

class Fire : public Object
{
    // public: Fire(double x, double y, double z, float r, float g, float b, Speed speed, float radius, int slicesAndStacks);
    public: int slicesAndStacks;   
    public: Fire(){};
    public: Fire(double x, double y, double z, float r, float g, float b, Speed speed, float radius, int slicesAndStacks) : Object(x, y, z, r, g, b, speed, radius){
        this->slicesAndStacks = slicesAndStacks;
    };
};