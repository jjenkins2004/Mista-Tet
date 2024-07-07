#ifndef RANDOMBLOCK_H
#define RANDOMBLOCK_H

#include <raylib.h>
#include <vector>
#include <math.h>
#include <iostream>

//same as Vector2 type but uses more precise types
typedef struct v2 {
    long double x;
    long double y;
} v2;

//to get a random block from a bag
struct RandomBlock {
    public:
        //chooses from bag of 7 blocks
        int getBlock();

    private:
        //resets our bag and adds blocks back in when empty
        void resetBag();
        std::vector<int> blocks;
};

//to get accurate coordinates relative to the screen when there is a camera rotation
Vector2 getAdjustedCoordinates(Vector2 coor, double rotation);

//take derivative of distance between center of circle and line. If closest distance is less than radius of circle then there is a collision
bool checkCollisionLineCircle(Vector2 p1, Vector2 p2, Vector2 c, float r);

//return Vector2 of the closest point of line to center of circle
Vector2 closestPointLineCircle(Vector2 p1, Vector2 p2, Vector2 c);

#endif