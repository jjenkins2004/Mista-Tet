//--------------------------------------------------------------------------
// includes and constants
//--------------------------------------------------------------------------

#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"
#include "score.h"
#include <vector>
#include <string>
#include <iostream>
#include "tools.h"

#define RECWIDTH 240
#define RECHEIGHT 70


//--------------------------------------------------------------------------
// PowerupItem parent struct and children
//--------------------------------------------------------------------------

//Individual PowerupItems that will go into the PowerupItem (see below) list when
//spawned in. All items which its own unique powers has its own struct which
//inherits the PowerupItem struct, for the movement and collection of each
//powerup item is very similar. However, some need special functions and variables
//that don't apply to every item. All items will be initially stored as PowerupItem
//types. To figure out what specific powerupitem it is, the ID will idenfity it
//and dynamic casting is used to access specific functions

struct PowerupItem {
    PowerupItem() { id = "null"; }                          //construct a null PowerupItem
    virtual ~PowerupItem();                                 //virtual destructor
    PowerupItem(std::string i, Texture2D txt, int t);       //main constructor that takes ID, texture path, and time until despawn
    void DrawItem(bool withRotation);                       //draw the item with its position
                                                            //with rotation tells whether to draw item with consideration of the board's rotation or to ignore
    void moveItem();                                        //move the item based on its velocity
    void spaz();                                            //eerie animation for when item is collected
                                                            //handles storing the item or immediately starting its effects
                                                            //handles appropriate actions when any powerup item is collected
    //identifying variables
    std::string id;                                         //ID of powerupitem
    Texture2D texture;                                      //texture of the powerup
    int time;                                               //frames left before despawn
    double fade = 0;                                        //opacity of item 
    bool positive;                                          //used to determine if the item should be immediately used or stored 

    //positioning of powerupitem
    Vector2 pos;                                            //x and y pos
    Vector2 vel;                                            //x and y vel
    Vector2 rotation;                                       //angular position and angular velocity for rolling
    static float cameraRotation;                            //current rotation of the screen
    static float rotationVel;                               //current angular velocity of camera, + is clockwise
    bool wait = true;                                       //fade in item

    //spaz function variables
    bool spazzed = false;                                   //to determine if it is the first time calling spaz()
    Vector2 spazpos;                                        //position of item during animation
    Vector2 distFromOrigin = (Vector2) {5, 5};              //how far should the item spaz away from its original position
    bool removed = false;                                   //should item be removed from list now
};

//multiplicative multiplier that is only active for a certain amount of time
struct Multiplier: PowerupItem {
    Multiplier(double m, int time, Texture2D texture);      //Multiplier constructor                  
    double multiplier;                                      //multiplier of the item
};

//shoots three lasers from the sky and gives a multiplier based on how many blocks were destroyed
struct Laser: PowerupItem {
    Laser(int time, Texture2D texture);                     //Laser constructor
};

//large bomb whose location is chosen by the player 
struct Bomb: PowerupItem{
    Bomb(int time, Texture2D texture);                      //Bomb constructor
};

//Nuke that destroyes everything
struct Nuke: PowerupItem {
    Nuke(int time, Texture2D texture);                      //Nuke constructor
};

//changes the next three blocks
struct ThreeBlock: PowerupItem {
    ThreeBlock(int time, Texture2D texture, int id);        //ThreeBlock constructor
    int blockID;                                            //block ID of the three blocks being added
};

//permanent additive multiplier
struct PlusMultiplier: PowerupItem {
    PlusMultiplier(double m, int time, Texture2D texture);  //PlusMultiplier constructor
    double multiplier;                                      //amount of permanent multiplier to add
};

//spawns five random powerups on activation
struct FiveRandom: PowerupItem {
    FiveRandom(int time, Texture2D texture);                //FiveRandom constructor
};

//changes the speed at which the current block automatically falls
struct SpeedChange: PowerupItem {
    SpeedChange(int var, int time, Texture2D texture);      //SpeedChange constructor
    int variant;                                            //0 for pause, 1 for fast1, 2 for fast2, -1 for slow1, -2 for slow2
};

//changes into a random powerup on activation, all powerups have an equal chance
struct Mystery: PowerupItem {
    Mystery(int time, Texture2D texture);                   //Mystery constructor
    void collect();                                         //called when mystery is collected and to fade in the new powerupitem
    PowerupItem* mystery = nullptr;                         //stores the powerupitem that mystery will change into
};

//--------------------------------------------------------------------------
// PowerupItem list
//--------------------------------------------------------------------------

//Doubly Linked List which stores current spawned PowerupItems. It is 
//iterated through by Powerup class when drawing all spawned powerups.
//List supports adding to the back and removing a specific item which are
//the only functions needed

//individual powerup item
struct item {
    PowerupItem* curr;                                      //current item
    item* next;                                             //next item
    item* prev;                                             //previous item
};

//list
struct powerList {
    void push_back(PowerupItem* i);                         //adds new powerupitem to the back of the list
    void remove(item* i);                                   //deletes the given item
    item* head = nullptr;                                   //head of the list
    item* tail = nullptr;                                   //tail of the list
};


//--------------------------------------------------------------------------
// Powerup Class
//--------------------------------------------------------------------------

class Powerup {
    public:
        //constructor
        Powerup();

        //destructor
        ~Powerup();

        //updating and setting
        void updateRotation(float r, float w, bool s) {rotation = r;}
        void updateLevel(int l) {level = l;}
        void setScore(Score* s) {src = s;}

        //powerup related functions
        void drawPowerupItems();                                //draw everything related to player powerups
        void drawPowerBoard();                                  //draws out powerup board
        void spawnPowerup(bool include5Rand);                   //spawn a powerup with set chances, bool tells whether to include the five
                                                                //random powerups power, set to false when a five random powerup is used to
                                                                //prevent overpowered chaining
        PowerupItem* usePowerup(int k);                         //use the powerup at the kth circle in the powerupboard
        std::vector<std::pair<double, int>> checkFastSpeed();   //special function for negative speed change powers

    private:

        //font
        Font allFont = LoadFont("resources/all_font.ttf");

        //locations for powerboard
        Rectangle powerBoard = (Rectangle) {350-RECWIDTH/2, 705, RECWIDTH, RECHEIGHT};  //board of stored powerups location
        Vector2 circle1 = (Vector2) {280, 740};                                         //circle1 location in board
        Vector2 circle2 = (Vector2) {350, 740};                                         //circle2 location in board
        Vector2 circle3 = (Vector2) {420, 740};                                         //circle3 location in board
        Color circleColor = (Color) {40, 40, 40, 255};                                  //color of circle outline

        //powerup generation variables
        std::vector<double> dividers = {0, 0.15, 0.25, 0.45, 0.55, 0.65, 0.69, 0.76, 0.85, 0.92, 0.97, 1};

            //positive multipliers: 15%, <= 0.15
            //negative multipliers: 10%, <= 0.25
            //three blocks: 20%, <= 0.45
            //slowing powerups: 10%, <= 0.55
            //fast powerups: 10%, <= 0.65
            //nuke: 4%, <= 0.69
            //laser: 7%, <= 0.76
            //bomb: 9%, <= 0.85
            //permanent multiplier: 7%, <= 0.92
            //mystery: 5%, <= 0.97
            //five random: 3%, <= 1
        std::vector<double> originalDividers = {0, 0.15, 0.25, 0.45, 0.55, 0.65, 0.69, 0.76, 0.85, 0.92, 0.97, 1};
        int numPow = 0;                                 //when 10 powerups have spawned, reset to original chances
        

        //storage for spawned powerups
        powerList spawnedPower;                         //the list of spawned powerups
        std::vector<PowerupItem*> currPower;            //vector of stored powerups
        std::vector<std::pair<double, int>> fastSpeed;  //special container for debuff speedChange powers

        //other variables
        float rotation = 0;                             //current rotation of camera, needed for mouse collision with powerup balls
        int level;                                      //current level
        Score* src;                                     //score class pointer
};

#endif