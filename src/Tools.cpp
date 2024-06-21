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
    return (Vector2){static_cast<float>(r*cos(theta)+400), static_cast<float>(r*sin(theta)+400)};       //returning new point
}

bool checkCollisionLineCircle(Vector2 p1, Vector2 p2, Vector2 c, double r) {
    //calculate slope and y-intercept for our line, y = mx + b
    double m = (p1.y-p2.y)/(p1.x-p2.x);
    double b = p1.y-m*p1.x;

    //setting derivative of distance formula between point and some point on the line to 0 to find point on the line that is closest distance to the center of circle
    double x = (c.x+m*c.y-m*b)/(1+m*m);

    //find shortest distance between center of circle and line
    double y = m*x+b;
    double d = sqrt(pow(c.x-x, 2) + pow(c.y-y, 2));

    //if the shortest distance between the center of circle and the line is less than the radius then that means there is a collision
    return d < r;
}

Vector2 closestPointLineCircle(Vector2 p1, Vector2 p2, Vector2 c) {
    //calculate slope and y-intercept for our line, y = mx + b
    double m = (p1.y-p2.y)/(p1.x-p2.x);
    double b = p1.y-m*p1.x;

    //setting derivative of distance formula between point and some point on the line to 0 to find point on the line that is closest distance to the center of circle
    double x = (c.x+m*c.y-m*b)/(1+m*m);

    //returning point
    return {float(x), float(m*x+b)};
}