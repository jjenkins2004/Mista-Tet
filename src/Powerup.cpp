#include "Powerup.h"

Powerup::Powerup() {

}

void Powerup::drawPowerup() {
    
    DrawRectangleLinesEx(powerBoard, 5, GRAY);
    DrawCircleV(circle1, 25, circleColor);
    DrawCircleV(circle1, 22, BLACK);
    DrawCircleV(circle2, 25, circleColor);
    DrawCircleV(circle2, 22, BLACK);
    DrawCircleV(circle3, 25, circleColor);
    DrawCircleV(circle3, 22, BLACK);
    DrawText("1", 245, 720, 7, WHITE);
    DrawText("2", 315, 720, 7, WHITE);
    DrawText("3", 385, 720, 7, WHITE);
}

void Powerup::spawnPowerup() {

}