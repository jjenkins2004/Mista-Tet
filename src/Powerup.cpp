#include "Powerup.h"

Powerup::Powerup() {
    for (int i = 0; i < 3; i++) {
        currPower.push_back(new PowerupItem());
    }
}

void Powerup::drawPowerup() {
    
    //drawing border and powerup placeholders
    DrawRectangleRoundedLines(powerBoard, 0.2, 100, 5, (Color){80, 60, 60, 255});
    DrawCircleV(circle1, 25, circleColor);
    DrawCircleV(circle1, 23, BLACK);
    DrawCircleV(circle1, 23, (Color){static_cast<unsigned char>(0+level/2), 0, 0});
    DrawCircleV(circle2, 25, circleColor);
    DrawCircleV(circle2, 23, BLACK);
    DrawCircleV(circle2, 23, (Color){static_cast<unsigned char>(0+level/2), 0, 0});
    DrawCircleV(circle3, 25, circleColor);
    DrawCircleV(circle3, 23, BLACK);
    DrawCircleV(circle3, 23, (Color){static_cast<unsigned char>(0+level/2), 0, 0});

    //drawing text
    Color DarkRed = {static_cast<unsigned char>(255-level*1.4), 0, 0, 255};
    DrawTextEx(allFont, "1", (Vector2){240, 715}, 15, 0, DarkRed);
    DrawTextEx(allFont, "1", (Vector2){240, 715}, 15, 0, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    DrawTextEx(allFont, "2", (Vector2){310, 715}, 15, 0, DarkRed);
    DrawTextEx(allFont, "2", (Vector2){310, 715}, 15, 0, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    DrawTextEx(allFont, "3", (Vector2){380, 715}, 15, 0, DarkRed);
    DrawTextEx(allFont, "3", (Vector2){380, 715}, 15, 0, Fade((Color){65, 170, 255, 255}, 1-level/60.0));

    //drawing powerups
    for (int i = 0; i < 3; i++) {   
        if (currPower[i]->id != "null") {
            currPower[i]->DrawItem();
        }
    }
    item* it = spawnedPower.head;
    bool removed = false;
    while(it != nullptr) {
        std::cout << "before removed" << std::endl;
        if (it->curr->removed) {
            it->curr->spaz();
            if (it->curr->fade <= 0) {
                    item* temp = it; it = it->next;
                    removed = true;
                    bool added = false;
                    if (temp->curr->id == "multiplier" && !temp->curr->positive) {
                        Multiplier* m = dynamic_cast<Multiplier*>(temp->curr);
                        src->addMultiplier(m->multiplier);
                    }
                    else {
                        for (int i = 0; i < 3; i++) {
                            if (currPower[i]->id == "null") {
                                currPower[i] = temp->curr;
                                if (i == 0) {
                                    currPower[i]->pos.first = circle1.x; currPower[i]->pos.second = circle1.y;
                                }
                                else if (i == 1) {
                                    currPower[i]->pos.first = circle2.x; currPower[i]->pos.second = circle2.y;
                                }
                                else if (i == 2) {
                                    currPower[i]->pos.first = circle3.x; currPower[i]->pos.second = circle3.y;
                                }
                                currPower[i]->fade = 1;
                                currPower[i]->rotation.first = 0;
                                added = true;
                                break;
                            }
                        }
                    }
                    if (!added) {
                        delete temp->curr;
                    }
                    spawnedPower.remove(temp);
                }
        }
        else {
            it->curr->moveItem();
            it->curr->DrawItem();
            it->curr->time--;
            if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){it->curr->pos.first, it->curr->pos.second}, 30) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Sound powerupSFX = LoadSound("resources/audio/powerupCollection.wav");
                PlaySound(powerupSFX);
                it->curr->removed = true;
            }
        }
        std::cout << "after removed" << std::endl;
        if (it != nullptr && it->curr->time <= 0 && !it->curr->removed) {
            it->curr->fade-=0.01;
            if (it->curr->fade <= 0) {
                 item* temp = it; it = it->next;
                 delete temp->curr;
                 spawnedPower.remove(temp);
                 removed = true;
            }
        }
        if (!removed) {
            it = it->next;
        }
        else {
            removed = false;
        }
    }
    std::cout << "after loop" << std::endl;
}

void Powerup::spawnPowerup() {
    bool positive = false;
    int rand1 = GetRandomValue(1, 9);
    
    if (rand1 <= 3) {
        int rand2 = GetRandomValue(1, 50);
        //x2 multiplier
        if (rand2 <= 5) {
            spawnedPower.push_back(new Multiplier(2, 600, x2));
        }
        //x1.5 multiplier
        else if (rand2 <= 15) {
            spawnedPower.push_back(new Multiplier(1.5, 1200, x1_5));
        }
        //x1.2 multiplier
        else if (rand2 <= 30) {
            spawnedPower.push_back(new Multiplier(1.2, 1500, x1_2));
        }  
        else if (rand2 <= 40) {
            spawnedPower.push_back(new Multiplier(-1, 1500, xNegative));
        }
        else {
            spawnedPower.push_back(new Multiplier(0.7, 1500, x0_7));
        }
    }
    else if (rand1 <= 6) {
        int rand2 = GetRandomValue(1, 50);
        if (rand2 <= 10) {
            spawnedPower.push_back(new ThreeBlock(1200, Iblock, 1));
        }
        else if (rand2 <= 20) {
            spawnedPower.push_back(new ThreeBlock(1200, Jblock, 2));
        }
        else if (rand2 <= 30) {
            spawnedPower.push_back(new ThreeBlock(1200, Lblock, 3));
        }
        else if (rand2 <= 40) {
            spawnedPower.push_back(new ThreeBlock(1200, Oblock, 4));
        }
        else if (rand2 <= 50) {
            spawnedPower.push_back(new ThreeBlock(1200, Tblock, 6));
        }
    }
    else if (rand1 <= 9) { 
        int rand2 = GetRandomValue(1, 50);
        if (rand2 <= 20) {
            spawnedPower.push_back(new Laser(1200, lasers));
        }
        else if (rand2 <= 40) {
            spawnedPower.push_back(new Bomb(1200, bomb));
        }
        else {
            spawnedPower.push_back(new Nuke(1000, nuke));
        }
    }
}

PowerupItem* Powerup::usePowerup(int k) {
    PowerupItem* p;
    p = currPower[k-1];
    currPower[k-1] = new PowerupItem();
    return p;
}