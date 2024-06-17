#ifndef RANDOMBLOCK_H
#define RANDOMBLOCK_H

#include <raylib.h>
#include <vector>
#include <math.h>
#include <iostream>

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

//to get accurate mouse coordinates when there is a camera rotation
Vector2 getAdjustedCoordinates(Vector2 coor, double rotation);

#endif