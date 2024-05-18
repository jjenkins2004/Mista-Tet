#include "Powerup.h"

Powerup::Powerup() {
    for (int i = 0; i < 3; i++) {
        currPower.push_back(PowerupItem());
    }
}

void Powerup::drawPowerup() {
    
    //drawing border and powerup placeholders
    DrawRectangleRoundedLines(powerBoard, 0.2, 100, 5, (Color){80, 60, 60, 255});
    DrawCircleV(circle1, 25, circleColor);
    DrawCircleV(circle1, 22, BLACK);
    DrawCircleV(circle2, 25, circleColor);
    DrawCircleV(circle2, 22, BLACK);
    DrawCircleV(circle3, 25, circleColor);
    DrawCircleV(circle3, 22, BLACK);

    //drawing text
    Color DarkRed = {static_cast<unsigned char>(255-level*1.4), 0, 0, 255};
    DrawTextEx(allFont, "1", (Vector2){240, 715}, 15, 0, DarkRed);
    DrawTextEx(allFont, "1", (Vector2){240, 715}, 15, 0, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    DrawTextEx(allFont, "2", (Vector2){310, 715}, 15, 0, DarkRed);
    DrawTextEx(allFont, "2", (Vector2){310, 715}, 15, 0, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    DrawTextEx(allFont, "3", (Vector2){380, 715}, 15, 0, DarkRed);
    DrawTextEx(allFont, "3", (Vector2){380, 715}, 15, 0, Fade((Color){65, 170, 255, 255}, 1-level/60.0));

    //drawing powerups
    item* it = spawnedPower.head;
    bool removed = false;
    while(it != nullptr) {
        if (it->curr.removed) {
            it->curr.spaz();
        }
        else {
            it->curr.moveItem();
            it->curr.DrawItem();
            it->curr.time--;
            if (CheckCollisionPointCircle(GetMousePosition(), it->curr.pos, 25) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                it->curr.removed = true;
            }
        }
        if (it->curr.removed || it->curr.time <= 0) {
                if (it->curr.fade <= 0) {
                    item* temp = it; it = it->next;
                    removed = true;
                    for (int i = 0; i < 3; i++) {
                        if (currPower[i].id == "null") {
                            currPower[i] = temp->curr;
                            if (i == 0) {
                                currPower[i].pos.x = circle1.x; currPower[i].pos.y = circle1.y;
                            }
                            else if (i == 1) {
                                currPower[i].pos.x = circle2.x; currPower[i].pos.y = circle2.y;
                            }
                            else if (i == 2) {
                                currPower[i].pos.x = circle3.x; currPower[i].pos.y = circle3.y;
                            }
                            currPower[i].fade = 1;
                            break;
                        }
                    }
                    spawnedPower.remove(temp);
                }
                else if (!it->curr.removed) {
                    it->curr.fade-=0.01;
                }
            }
        if (!removed) {
            it = it->next;
        }
        else {
            removed = false;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (currPower[i].id != "null") {
            currPower[i].DrawItem();
        }
    }
}

void Powerup::spawnPowerup() {
    bool positive = false;
    if (GetRandomValue(1,2) == 1) {
        positive = true;
    }
    if (positive) {
        int rand = GetRandomValue(1, 50);
        //x2 multiplier
        if (rand <= 10) {
            int rand = GetRandomValue(20, 35);
            spawnedPower.push_back(Multiplier(2, 600, "resources/powerup/x2Multiplier.png", (Vector2){0, static_cast<float>(GetRandomValue(15, 30))}));
        }
        //x1.5 multiplier
        else if (rand <= 25 && rand > 10) {
            int rand = GetRandomValue(10, 20);
            spawnedPower.push_back(Multiplier(2, 1200, "resources/powerup/x1,5Multiplier.png", (Vector2){0, static_cast<float>(GetRandomValue(0, 20))}));
        }
        //x1.2 multiplier
        else if (rand <=50 && rand > 25) {
            int rand = GetRandomValue(5, 15);
            spawnedPower.push_back(Multiplier(2, 1500, "resources/powerup/x1,2Multiplier.png", (Vector2){0, static_cast<float>(GetRandomValue(0, 15))}));
        }  
        //
    }
}