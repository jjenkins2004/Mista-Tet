#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"
#include <vector>
#include <string>
#include <iostream>

#define RECWIDTH 240
#define RECHEIGHT 70


struct PowerupItem {
    PowerupItem() {
        id = "null";
    }
    PowerupItem(std::string i, std::string txt, int t, Vector2 v): id(i), time(t), vel(v){
        if (i != "null") {
            pos.x = GetRandomValue(30, 770);
            pos.y = -25;
            texture = LoadTexture(txt.c_str());
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
    Vector2 pos;
    Vector2 vel;
    std::string id;
    Texture2D texture;
    //how many frames before item despawns (60 frames per second)
    int time;
    double fade = 1;
};

struct Multiplier: PowerupItem {
    Multiplier(double m, int time, std::string texture, Vector2 v): PowerupItem("multiplier", texture, time, v) {
        multiplier = m;
    }
    double multiplier;
};



//list we will use to store current powerups
struct item {
    PowerupItem curr;
    item* next;
    item* prev;
};

struct powerList {
    void push_back(PowerupItem i) {
        std::cout << "adding item" << std::endl;
        if (tail != nullptr) {
            tail->next = new item();
            tail->next->next = nullptr; tail->next->curr = i; tail->next->prev = tail;
            tail = tail->next;
        }
        else {
            std::cout << "addddding item" << std::endl;
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
        void drawPowerup();
        void spawnPowerup();
        int checkPowerup();

        
    private:
    //font
    Font allFont = LoadFont("resources/allFont.ttf");
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
    PowerupItem currPower[3];
};

#endif