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
    for (int i = 0; i < 3; i++) if (currPower[i]->id != "null") currPower[i]->DrawItem();

    item* it = spawnedPower.head;
    bool removed = false;
    while(it != nullptr) {
        if (it->curr->removed) { //checks if the current powerup has been clicked
            if (it->curr->id == "mystery") { //we need to do something special for mystery powerups
                Mystery* m = dynamic_cast<Mystery*>(it->curr);
                if (m->fade <= 0) {
                    spawnedPower.push_back(m->mystery);
                    item* temp = it;
                    it = it->next;
                    spawnedPower.remove(temp);
                    continue;
                }
                else m->collect();
            }
            else { //the animation that plays everytime powerup is collected
                it->curr->spaz();
                if (it->curr->fade <= 0) {
                    item* temp = it; it = it->next;
                    removed = true;
                    bool added = false;
                    if (temp->curr->id == "multiplier" && !temp->curr->positive) { //we don't need to add bad multipliers to the collected powers, effects are realized immediately
                        Multiplier* m = dynamic_cast<Multiplier*>(temp->curr);
                        src->addMultiplier(m->multiplier);
                    }
                    else if (temp->curr->id == "fiverandom") { //don't need to add fiverandom to collected powerups, effects are realized immediately
                        for (int i = 0; i < 5; i++) spawnPowerup(false);
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
                    if (!added) delete temp->curr;
                    spawnedPower.remove(temp);
                }
            }
        }
        else {
            it->curr->moveItem();
            it->curr->DrawItem();
            it->curr->time--;
            if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){it->curr->pos.first, it->curr->pos.second}, 30) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                it->curr->removed = true;
            }
        }
        if (it != nullptr && it->curr->time <= 0 && !it->curr->removed) {
            it->curr->fade-=0.01;
            if (it->curr->fade <= 0) {
                 item* temp = it; it = it->next;
                 delete temp->curr;
                 spawnedPower.remove(temp);
                 removed = true;
            }
        }
        if (!removed) it = it->next; //if nothing has been removed, move on to next node
        else removed = false; //otherwise iterator stays on current one as this will be the next node
    }
}

void Powerup::spawnPowerup(bool include5Rand) {
    bool positive = false;
    int rand1;
    spawnedPower.push_back(new Nuke(800, nuke));
    return;
    if (include5Rand) rand1 = GetRandomValue(1, 100);
    else rand1 = GetRandomValue(1, 13);

    //multiplier powerup
    if (rand1 <= 25) {
        int rand2 = GetRandomValue(1, 5);
        if (rand2 <= 1) spawnedPower.push_back(new Multiplier(2, 800, x2));
        else if (rand2 <= 2) spawnedPower.push_back(new Multiplier(1.5, 1200, x1_5));
        else if (rand2 <= 3) spawnedPower.push_back(new Multiplier(1.2, 1500, x1_2));
        else if (rand2 <= 4) spawnedPower.push_back(new Multiplier(-1, 1500, xNegative));
        else spawnedPower.push_back(new Multiplier(0.7, 1500, x0_7));
    }
    //next three blocks powerup
    else if (rand1 <= 45) {
        int rand2 = GetRandomValue(1, 5);
        if (rand2 <= 1) spawnedPower.push_back(new ThreeBlock(1000, Iblock, 1));
        else if (rand2 <= 2) spawnedPower.push_back(new ThreeBlock(1200, Jblock, 2));
        else if (rand2 <= 3) spawnedPower.push_back(new ThreeBlock(1200, Lblock, 3));
        else if (rand2 <= 4) spawnedPower.push_back(new ThreeBlock(1200, Oblock, 4));
        else if (rand2 <= 5) spawnedPower.push_back(new ThreeBlock(1200, Tblock, 6));
    }
    //change block movedown speed powerup
    else if (rand1 <= 65) {
        int rand2 = GetRandomValue(1, 5);
        if (rand2 == 1) spawnedPower.push_back(new SpeedChange(0, 800, pause));
        if (rand2 == 2) spawnedPower.push_back(new SpeedChange(1, 1000, fast1));
        if (rand2 == 3) spawnedPower.push_back(new SpeedChange(2, 1000, fast2));
        if (rand2 == 4) spawnedPower.push_back(new SpeedChange(-1, 1000, slow1));
        if (rand2 == 3) spawnedPower.push_back(new SpeedChange(-2, 1000, slow2));
    }
    //bomb, nuke, and laser powerups
    else if (rand1 <= 80) { 
        int rand2 = GetRandomValue(1, 5);
        if (rand2 <= 2) spawnedPower.push_back(new Laser(1000, lasers));
        else if (rand2 <= 4) spawnedPower.push_back(new Bomb(1000, bomb));
        else spawnedPower.push_back(new Nuke(800, nuke));
    }
    //permanent multiplier boost powerup
    else if (rand1 <= 86) {
        int rand2 = GetRandomValue(1, 4);
        if (rand2 <= 3) spawnedPower.push_back(new PlusMultiplier(0.1, 1000, plus0_1));
        else spawnedPower.push_back(new PlusMultiplier(0.2, 800, plus0_2));
    }
    else if (rand1 <= 95) spawnedPower.push_back(new Mystery(1000, mystery));
    else if (rand1 <= 100) spawnedPower.push_back(new FiveRandom(1000, fiveRandom));
}

PowerupItem* Powerup::usePowerup(int k) {
    PowerupItem* p;
    p = currPower[k-1];
    currPower[k-1] = new PowerupItem();
    return p;
}