#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"

#define RECWIDTH 240
#define RECHEIGHT 70

class Powerup {
    public:
        Powerup();
        void drawPowerup();
        void spawnPowerup();
        int checkPowerup();
        
    private:
    //locations for powerboard
    Rectangle powerBoard = (Rectangle) {350-RECWIDTH/2, 705, RECWIDTH, RECHEIGHT};
    Vector2 circle1 = (Vector2) {280, 740};
    Vector2 circle2 = (Vector2) {350, 740};
    Vector2 circle3 = (Vector2) {420, 740};
    Color circleColor = (Color) {40, 40, 40, 255};

    //storage for spawned powerups

};

#endif