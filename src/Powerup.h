#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"
#include "Score.h"
#include <vector>
#include <string>
#include <iostream>

#define RECWIDTH 240
#define RECHEIGHT 70

struct PowerupItem {
    PowerupItem() {
        id = "null";
    }
    virtual ~PowerupItem() {}
    PowerupItem(std::string i, Texture2D txt, int t): id(i), time(t) {
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
    void DrawItem() {
        if (id != "null") {
            DrawTexturePro(texture, (Rectangle){0, 0, 50, 50}, (Rectangle){pos.first, pos.second, 50, 50}, (Vector2){25, 25}, rotation.first, Fade(WHITE, fade));
        }
    }
    void moveItem() {
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
    void spaz() {
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
    
    //positioning
    std::pair<float, float> pos;
    std::pair<float, float> vel;
    //angular position and angular velocity
    std::pair<float, float> rotation;
    std::string id;

    Texture2D texture;
    //how many frames before item despawns (60 frames per second)
    int time;
    double fade = 1;
    bool removed = false;
    bool positive;

    //for da spaz
    bool spazzed = false;
    Vector2 spazpos;
    Vector2 distFromOrigin = (Vector2) {5, 5};
};

struct Multiplier: PowerupItem {
    Multiplier(double m, int time, Texture2D texture): PowerupItem("multiplier", texture, time) {
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

struct Laser: PowerupItem {
    Laser(int time, Texture2D texture): PowerupItem("laser", texture, time) {}
};

struct Bomb: PowerupItem{
    Bomb(int time, Texture2D texture): PowerupItem("bomb", texture, time) {}
};

struct Nuke: PowerupItem {
    Nuke(int time, Texture2D texture): PowerupItem("nuke", texture, time) {}
};

struct ThreeBlock: PowerupItem {
    ThreeBlock(int time, Texture2D texture, int id): PowerupItem("threeblock", texture, time), blockID(id) {}
    int blockID;
};

struct PlusMultiplier: PowerupItem {
    PlusMultiplier(double m, int time, Texture2D texture): PowerupItem("plusmultiplier", texture, time), multiplier(m) {}
    double multiplier;
};

struct FiveRandom: PowerupItem {
    FiveRandom(int time, Texture2D texture): PowerupItem("fiverandom", texture, time) {}
};

struct SpeedChange: PowerupItem {
    SpeedChange(int var, int time, Texture2D texture): PowerupItem("speedchange", texture, time), variant(var) {}
    int variant; //0 for pause, 1 for fast1, 2 for fast2, -1 for slow1, -2 for slow2
};

struct Mystery: PowerupItem {
    Mystery(int time, Texture2D texture): PowerupItem("mystery", texture, time) {};

    void collect() {
        if (!createMystery) {
            int rand = GetRandomValue(1, 21);

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
            createMystery = true;
            PlaySound(LoadSound("resources/audio/MysteryReveal.wav"));
        }
        mystery->fade+=0.0125;
        fade-=0.0125;
        this->DrawItem();
        mystery->DrawItem();
    }

    bool createMystery = false;
    PowerupItem* mystery;
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
        if (i == head) head = i->next;
        if (i == tail) tail = i->prev;
        if (i->prev != nullptr) i->prev->next = i->next;
        if (i->next != nullptr) i->next->prev = i->prev;
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

        //powerup related functions
        void drawPowerup();
        void spawnPowerup(bool include5Rand);
        PowerupItem* usePowerup(int k);
        std::vector<std::pair<double, int>> checkFastSpeed() { //function is used so our changespeed powerup that makes speed faster isn't added to powerup collection and effects are immediate
            std::vector<std::pair<double, int>> temp = fastSpeed;
            fastSpeed.clear();
            return temp;
        }; 

    private:
    //font
    Font allFont = LoadFont("resources/allFont.ttf");

    //textures
    Texture2D x2 = LoadTexture("resources/powerup/x2Multiplier.png");
    Texture2D x1_5 = LoadTexture("resources/powerup/x1,5Multiplier.png");
    Texture2D x1_2 = LoadTexture("resources/powerup/x1,2Multiplier.png");
    Texture2D x0_7 = LoadTexture("resources/powerup/x0,7Multiplier.png");
    Texture2D xNegative = LoadTexture("resources/powerup/-Multiplier.png");
    Texture2D lasers = LoadTexture("resources/powerup/Lasers.png");
    Texture2D bomb = LoadTexture("resources/powerup/Bomb.png");
    Texture2D nuke = LoadTexture("resources/powerup/Nuke.png");
    Texture2D Jblock = LoadTexture("resources/powerup/Jblock.png");
    Texture2D Lblock = LoadTexture("resources/powerup/Lblock.png");
    Texture2D Oblock = LoadTexture("resources/powerup/Oblock.png");
    Texture2D Tblock = LoadTexture("resources/powerup/Tblock.png");
    Texture2D Iblock = LoadTexture("resources/powerup/Iblock.png");
    Texture2D plus0_2 = LoadTexture("resources/powerup/+0,2Multiplier.png");
    Texture2D plus0_1 = LoadTexture("resources/powerup/+0,1Multiplier.png");
    Texture2D mystery = LoadTexture("resources/powerup/Mystery.png");
    Texture2D fiveRandom = LoadTexture("resources/powerup/5Random.png");
    Texture2D slow1 = LoadTexture("resources/powerup/Slow1.png");
    Texture2D slow2 = LoadTexture("resources/powerup/Slow2.png");
    Texture2D fast1 = LoadTexture("resources/powerup/Fast1.png");
    Texture2D fast2 = LoadTexture("resources/powerup/Fast2.png");
    Texture2D pause = LoadTexture("resources/powerup/Pause.png");

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
    std::vector<std::pair<double, int>> fastSpeed;


    //other classes
    Score* src;
};

#endif