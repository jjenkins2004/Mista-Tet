#include "Tools.h"

int RandomBlock::getBlock() {
    if (blocks.empty()) resetBag();
    int rand = GetRandomValue(0, blocks.size()-1);
    int num = blocks[rand];
    std::vector<int>::iterator it = blocks.begin()+rand;
    blocks.erase(it);
    return num;
}

void RandomBlock::resetBag() {
    for (int i = 1; i < 8; i++) blocks.push_back(i);
}

Vector2 getAdjustedCoordinates(Vector2 coor, double rotation) {
    double x = coor.x-400; double y = coor.y-400;       //finding vectors of the coordinates with (400, 400) as the center
                                                        //(400,400) acts as (0,0) because it is in the center of the screen
    double r = sqrt(pow(x, 2) + pow(y, 2));             //compute the radius
    double theta = atan2(y, x) - rotation*PI/180;       //find angle of current point in radians and add rotation
    std::cout << "rotation is " << rotation << " new position is " << r*cos(theta)+400 << ", " << r*sin(theta)+400 << std::endl;
    return (Vector2){static_cast<float>(r*cos(theta)+400), static_cast<float>(r*sin(theta)+400)};       //returning new point
    
}