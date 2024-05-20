#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"
#include "Score.h"
#include <vector>
#include <string>
#include <iostream>

#define RECWIDTH 240
#define RECHEIGHT 70

class Grid;

struct PowerupItem {
    PowerupItem() {
        id = "null";
    }
    virtual ~PowerupItem() {}
    PowerupItem(std::string i, Texture2D txt, int t, Vector2 v): id(i), time(t), vel(v){
        if (i != "null") {
            pos.x = GetRandomValue(30, 770);
            pos.y = -25;
            texture = txt;
        }
    }
    void DrawItem() {
        if (id != "null") {
            DrawTexturePro(texture, (Rectangle){0, 0, 50, 50}, (Rectangle){pos.x, pos.y, 50, 50}, (Vector2){25, 25}, 0, Fade(WHITE, fade));
        }
    }
    void moveItem() {
        /*pos.x+=vel.x;*/ pos.y+=vel.y;
        /*if (pos.x >= 775) {
            pos.x = 775;
            vel.x*=-1;
        }
        else if (pos.x <= 25) {
            pos.x = 25;
            vel.x*=-1;
        }*/
        if (pos.y >= 775) {
            pos.y = 775;
            vel.y*=-0.90;
        }
        vel.y++;
    }
    //gives a eerie animation when powerups are collected
    void spaz() {
        //vel.x == 0 checks if it is our first time calling spaz
        if (vel.x == 0) {
            time = 60;
            vel.x = 1;
            vel.y = 1;
            spazpos.x = pos.x;
            spazpos.y = pos.y;
        }
        //if position goes some distance from the origin given by distFromOrigin, then reverse the velocity
        if (time == 0) {
            spazpos.x+=vel.x;
            spazpos.y+=vel.y;
            if (spazpos.x > pos.x+distFromOrigin.x) {
                vel.x = vel.x*-1-GetRandomValue(1, 2);
                spazpos.x = pos.x+distFromOrigin.x-25;
                distFromOrigin.x+=2;
            }
            else if (spazpos.x < pos.x-distFromOrigin.x) {
                vel.x = vel.x*-1+GetRandomValue(1, 2);
                spazpos.x = pos.x-distFromOrigin.x+25;
                distFromOrigin.x+=2;
            }
            if (spazpos.y > pos.y+distFromOrigin.y) {
                vel.y = vel.y*-1-GetRandomValue(-2, 2);
                spazpos.y = pos.y+distFromOrigin.y-25;
                distFromOrigin.y+=2;
            }
            else if (spazpos.y < pos.y-distFromOrigin.y) {
                vel.y = vel.y*-1+GetRandomValue(-2, 2);
                spazpos.y = pos.y-distFromOrigin.y+25;
                distFromOrigin.y+=2;
            }
            fade-=0.007;
        }
        //this is just for our iniitial pause before the spaz
        else {
            time--;
        }
        DrawTexturePro(texture, (Rectangle){0, 0, 50, 50}, (Rectangle){spazpos.x, spazpos.y, 50, 50}, (Vector2){25, 25}, 0, Fade(WHITE, fade));
    }
    Vector2 pos;
    Vector2 vel;
    std::string id;
    Texture2D texture;
    //how many frames before item despawns (60 frames per second)
    int time;
    double fade = 1;
    bool removed = false;
    bool positive;

    //for da spaz
    Vector2 spazpos;
    Vector2 distFromOrigin = (Vector2) {5, 5};
};

struct Multiplier: PowerupItem {
    Multiplier(double m, int time, Texture2D texture, Vector2 v): PowerupItem("multiplier", texture, time, v) {
        if (m < 1) {
            positive = false;
        }
        else {
            positive = true;
        }
        multiplier = m;
    }
    double multiplier;
};



//list we will use to store current powerups
struct item {
    PowerupItem* curr;
    item* next;
    item* prev;
};

struct powerList {
    void push_back(PowerupItem* i) {
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
    void remove(item* i) {
        if (i == head) {
            head = i->next;

        }
        if (i == tail) {
            tail = i->prev;
        }
        if (i->prev != nullptr) {
            i->prev->next = i->next;
        }
        if (i->next != nullptr) {
            i->next->prev = i->prev;
        }
        delete i;
    }
    item* head = nullptr;
    item* tail = nullptr;
};



class Powerup {
    public:
        Powerup();
        void updateLevel(int l) {
            level = l;
        }

        //setting access to other classes
        void setScore(Score* s) {
            src = s;
        }
        void setGrid(Grid* g) {
            grid = g;
        }

        //powerup related functions
        void drawPowerup();
        void spawnPowerup();
        void usePowerup(int k);

    private:
    //font
    Font allFont = LoadFont("resources/allFont.ttf");

    //textures
    Texture2D x2 = LoadTexture("resources/powerup/x2Multiplier.png");
    Texture2D x1_5 = LoadTexture("resources/powerup/x1,5Multiplier.png");
    Texture2D x1_2 = LoadTexture("resources/powerup/x1,2Multiplier.png");
    Texture2D x0_7 = LoadTexture("resources/powerup/x0,7Multiplier.png");
    Texture2D xNegative = LoadTexture("resources/powerup/-Multiplier.png");

    //level
    int level;

    //locations for powerboard
    Rectangle powerBoard = (Rectangle) {350-RECWIDTH/2, 705, RECWIDTH, RECHEIGHT};
    Vector2 circle1 = (Vector2) {280, 740};
    Vector2 circle2 = (Vector2) {350, 740};
    Vector2 circle3 = (Vector2) {420, 740};
    Color circleColor = (Color) {40, 40, 40, 255};
    

    //storage for spawned powerups
    powerList spawnedPower;
    std::vector<PowerupItem*> currPower;

    //other classes
    Grid* grid;
    Score* src;
};

#endif