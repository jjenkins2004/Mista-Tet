#include "Powerup.h"


/*********************************************************************************************
***** START OF POWERUPITEM STRUCTS 
*********************************************************************************************/


        //--------------------------------------------------------------------------
        // PowerupItem
        //--------------------------------------------------------------------------


PowerupItem::PowerupItem(std::string i, Texture2D txt, int t): id(i), time(t) {
    if (i != "null") {
        pos.first = GetRandomValue(30, 770);
        pos.second = -25;
        texture = txt;
        int xrand = GetRandomValue(15, 20);
        vel.first = xrand - 2*xrand*GetRandomValue(0, 1);
        vel.second = GetRandomValue(10, 20);
        rotation.first = 0; rotation.second = vel.first*2;
    }
}

void PowerupItem::DrawItem() {
    if (id != "null") {
        DrawTexturePro(texture, (Rectangle){0, 0, 50, 50}, (Rectangle){pos.first, pos.second, 50, 50}, (Vector2){25, 25}, rotation.first, Fade(WHITE, fade));
    }
}

void PowerupItem::moveItem() {
    pos.second+=vel.second; rotation.second = vel.first*2; pos.first+=vel.first; rotation.first += rotation.second;
    if (vel.first < 0) {
        vel.first+=0.02;
        if (vel.first > 0) {
            vel.first = 0;
        }
    }
    else if (vel.first > 0) {
        vel.first-=0.02;
        if (vel.first < 0) {
            vel.first = 0;
        }
    }
    else {
        vel.first = 0;
    }
    if (pos.first >= 775) {
        pos.first = 775;
        if (vel.first > 0) {
            vel.first*=-1;
        }
    }
    else if (pos.first <= 25) {
        pos.first = 25;
        if (vel.first < 0) {
            vel.first*=-1;
        }
    }
    if (pos.second >= 775) {
        pos.second = 775;
        vel.second*=-0.90;
    }
    vel.second++;
}
//gives a eerie animation when powerups are collected
void PowerupItem::spaz() {
    //checks if it is our first time calling spaz
    if (!spazzed) {
        Sound powerupSFX = LoadSound("resources/audio/powerupCollection.wav");
        PlaySound(powerupSFX);
        spazzed = true;
        time = 45;
        vel.first = 1;
        vel.second = 1;
        spazpos.x = pos.first;
        spazpos.y = pos.second;
    }
    //if position goes some distance from the origin given by distFromOrigin, then reverse the velocity
    if (time == 0) {
        spazpos.x+=vel.first;
        spazpos.y+=vel.second;
        if (spazpos.x > (int)(pos.first)+distFromOrigin.x) {
            vel.first = vel.first*-1-GetRandomValue(1, 2);
            spazpos.x = pos.first+distFromOrigin.x-26;
            distFromOrigin.x+=2;
        }
        else if (spazpos.x < (int)(pos.first)-distFromOrigin.x) {
            vel.first = vel.first*-1+GetRandomValue(1, 2);
            spazpos.x = pos.first-distFromOrigin.x+26;
            distFromOrigin.x+=2;
        }
        if (spazpos.y > pos.second+distFromOrigin.y) {
            vel.second = vel.second*-1-GetRandomValue(-2, 2);
            spazpos.y = pos.second+distFromOrigin.y-26;
            distFromOrigin.y+=2;
        }
        else if (spazpos.y < pos.second-distFromOrigin.y) {
            vel.second = vel.second*-1+GetRandomValue(-2, 2);
            spazpos.y = pos.second-distFromOrigin.y+26;
            distFromOrigin.y+=2;
        }
        fade-=0.007;
    }
    //this is just for our iniitial pause before the spaz
    else {
        time--;
    }
    DrawTexturePro(texture, (Rectangle){0, 0, 50, 50}, (Rectangle){spazpos.x, spazpos.y, 50, 50}, (Vector2){25, 25}, rotation.first, Fade(WHITE, fade));
}

        //--------------------------------------------------------------------------
        // Child structs
        //--------------------------------------------------------------------------
    

Multiplier::Multiplier(double m, int time, Texture2D texture): PowerupItem("multiplier", texture, time) {
    if (m < 1) positive = false;
    else positive = true;
    multiplier = m;
}

Laser::Laser(int time, Texture2D texture): PowerupItem("laser", texture, time) {}

Bomb::Bomb(int time, Texture2D texture): PowerupItem("bomb", texture, time) {}

Nuke::Nuke(int time, Texture2D texture): PowerupItem("nuke", texture, time) {}

ThreeBlock::ThreeBlock(int time, Texture2D texture, int id): PowerupItem("threeblock", texture, time), blockID(id) {}

PlusMultiplier::PlusMultiplier(double m, int time, Texture2D texture): PowerupItem("plusmultiplier", texture, time), multiplier(m) {}

FiveRandom::FiveRandom(int time, Texture2D texture): PowerupItem("fiverandom", texture, time) {}

SpeedChange::SpeedChange(int var, int time, Texture2D texture): PowerupItem("speedchange", texture, time), variant(var) {}

Mystery::Mystery(int time, Texture2D texture): PowerupItem("mystery", texture, time) {}

void Mystery::collect() {
    //checks if it is the first time calling this function and sets mystery var to a random powerup
    if (mystery == nullptr) {
        int rand = GetRandomValue(1, 21);

        //setting our mystery powerupitem
        if (rand == 1) mystery = new Multiplier(2, 800, LoadTexture("resources/powerup/x2Multiplier.png"));
        if (rand == 2) mystery = new Multiplier(1.5, 800, LoadTexture("resources/powerup/x1,5Multiplier.png"));
        if (rand == 3) mystery = new Multiplier(1.2, 800, LoadTexture("resources/powerup/x1,2Multiplier.png"));
        if (rand == 4) mystery = new Multiplier(0.7, 800, LoadTexture("resources/powerup/x0,7Multiplier.png"));
        if (rand == 5) mystery = new Multiplier(-1, 800, LoadTexture("resources/powerup/-Multiplier.png"));
        if (rand == 6) mystery = new ThreeBlock(1000, LoadTexture("resources/powerup/Iblock.png"), 1);
        if (rand == 7) mystery = new ThreeBlock(1200, LoadTexture("resources/powerup/Jblock.png"), 2);
        if (rand == 8) mystery = new ThreeBlock(1200, LoadTexture("resources/powerup/Lblock.png"), 3);
        if (rand == 9) mystery = new ThreeBlock(1200, LoadTexture("resources/powerup/Oblock.png"), 4);
        if (rand == 10) mystery = new ThreeBlock(1200, LoadTexture("resources/powerup/Tblock.png"), 6);
        if (rand == 11) mystery = new Laser(1000, LoadTexture("resources/powerup/Lasers.png"));
        if (rand == 12) mystery = new Bomb(1000, LoadTexture("resources/powerup/Bomb.png"));
        if (rand == 13) mystery = new Nuke(800, LoadTexture("resources/powerup/Nuke.png"));
        if (rand == 14) mystery = new PlusMultiplier(0.1, 1000, LoadTexture("resources/powerup/+0,1Multiplier.png"));
        if (rand == 15) mystery = new PlusMultiplier(0.2, 800, LoadTexture("resources/powerup/+0,2Multiplier.png"));
        if (rand == 16) mystery = new FiveRandom(1000, LoadTexture("resources/powerup/5Random.png"));
        if (rand == 17) mystery = new SpeedChange(0, 800, LoadTexture("resources/powerup/Pause.png"));
        if (rand == 18) mystery = new SpeedChange(1, 800, LoadTexture("resources/powerup/Fast1.png"));
        if (rand == 19) mystery = new SpeedChange(2, 800, LoadTexture("resources/powerup/Fast2.png"));
        if (rand == 20) mystery = new SpeedChange(-1, 800, LoadTexture("resources/powerup/Slow1.png"));
        if (rand == 21) mystery = new SpeedChange(-2, 800, LoadTexture("resources/powerup/Slow2.png"));

        fade = 1;
        mystery->removed = true;
        mystery->fade = 0;
        mystery->pos = pos;
        PlaySound(LoadSound("resources/audio/MysteryReveal.wav"));
    }

    //reduces opacity of the mystery powerup texture and increases opacity of new powerup texture
    mystery->fade+=0.0125;
    fade-=0.0125;
    this->DrawItem();
    mystery->DrawItem();
}


/*********************************************************************************************
***** END OF POWERUPITEM STRUCTS 
*********************************************************************************************/


/*********************************************************************************************
***** START OF POWERUPITEM LIST 
*********************************************************************************************/


void powerList::push_back(PowerupItem* i) {
    if (tail != nullptr) {
        tail->next = new item();
        tail->next->next = nullptr; tail->next->curr = i; tail->next->prev = tail;
        tail = tail->next;
    }
    else {
        head = new item();
        tail = head;
        head->curr = i; head->next = nullptr; head->prev = nullptr;
    }
}

void powerList::remove(item* i) {
    if (i == head) head = i->next;
    if (i == tail) tail = i->prev;
    if (i->prev != nullptr) i->prev->next = i->next;
    if (i->next != nullptr) i->next->prev = i->prev;
    delete i;
}


/*********************************************************************************************
***** END OF POWERUPITEM LIST 
*********************************************************************************************/

/*********************************************************************************************
***** START OF POWERUP CLASS
*********************************************************************************************/


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
                    else if (temp->curr->id == "speedchange") {
                        SpeedChange* s = dynamic_cast<SpeedChange*>(temp->curr);
                        if (s->variant > 0) fastSpeed.push_back(std::make_pair(s->variant == 1 ? 0.8: 0.65, 1800));
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
    if (include5Rand) rand1 = /*GetRandomValue(1, 100)*/64;
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

std::vector<std::pair<double, int>> Powerup::checkFastSpeed() { 
    std::vector<std::pair<double, int>> temp = fastSpeed;
    fastSpeed.clear();
    return temp;
}; 