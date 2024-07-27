#ifndef GRID_H
#define GRID_H


//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------

#include "raylib.h"
#include "tools.h"
#include <vector>
#include <ctime>
#include <queue>
#include <iostream>
#include "score.h"
#include "tet.h"
#include "powerup.h"



class Grid {

    public:

        //--------------------------------------------------------------------------
        // Constructors, setters, getters, drawing, updating
        //--------------------------------------------------------------------------

        Grid();                                                 //constructor
        ~Grid();                                                //destuctor
        void setScoreBoard(Score* score) { scr = score; }       //set scr to pointer
        void setTet(Tet* t) { tet = t; t->setGrid(grid);}       //set tet to pointer and give access to grid
        void setPowerup(Powerup* p) { pow = p; }                //set powerup to pointer
        void drawAll(bool block, bool useCamera);               //convinient funciton that draws everything
        void drawGrid(bool block);                              //draws the grid and bool tells grid if block should also be drawn
        void updateAll();                                       //convinient function that calls all updating functions
        void updateCamera();                                    //called everyframe to update the camera
        void tetFlyCamUpdate();                                 //called to create screen shake for tet flying powerup
        int finalStage();                                       //function called when bringing tet into final stage
        void updatelevel();                                     //updating function for level and level related powers
        int getLevel() { return level; }                        //get current level  
        bool checkGameOver() { return gameover; }               //check if player lost

        //--------------------------------------------------------------------------
        // Block Related Functions
        //--------------------------------------------------------------------------

        void moveDown();                                        //move block down one tile (calls move in block class)
        void moveRight();                                       //move block right one tile
        void moveLeft();                                        //move block left one tile
        void rotate();                                          //rotate block once
        void drop();                                            //drop block
        void placeBlock();                                      //place block into grid at current location
        void generateBlock();                                   //generate a new random block
        void hold();                                            //add the current block to the hold and current block in the hold becomes active block

        //--------------------------------------------------------------------------
        // Row Related Functions
        //--------------------------------------------------------------------------

        int removeRow(std::vector<int>& rows);                      //removes rows in argument and plays animation to make it look cool!
        const std::vector<int> checkRowComplete() const;            //returns which rows need to be removed in a vector if empty then no rows need to be removed
        int fixRows(std::vector<int> rows);                         //fixes row gap after removal of rows in argument vector 

        //--------------------------------------------------------------------------
        // Player powerup and Tet power functions
        //--------------------------------------------------------------------------
        
        void randomRotate();                                //get a random tet rotate power, randomizes active length and number of 90 degree rotations
        void extremeRandomRotate();                         //extreme version of random rotate, called at the beginning of final stage
        void increaseLevel(int x);                          //increase level by x 30 seconds (tet power)
        void blind(int num);                                //blind random rows on board, parameter is number of rows (tet power)
        int lasers();                                       //shoots three lasers from sky, adds score and multiplier based on how many blocks were destroyed (player power)
        int bomb();                                         //blows up specified area by player, adds score based on blocks destroyed (player power)
        int nuke();                                         //blows up whole board (player power)
        void changeNext(int i);                             //changes the next three blocks (player power & tet power)

        //--------------------------------------------------------------------------
        // Internal Block Class that handles block manipulation functions
        //--------------------------------------------------------------------------
        class Block {

            public:

                //--------------------------------------------------------------------------
                // Constructors and Getters, Setters
                //--------------------------------------------------------------------------

                Block(int id, int grid[21][10]);                        //constructor that is used
                Block() {};                                             //default constructor does nothing
                const std::pair<int, int>* getPos() { return pos; }     //get position of the block
                const int getId() { return id; }                        //get the ID of the block

                //--------------------------------------------------------------------------
                // Block Movement Functions
                //--------------------------------------------------------------------------

                bool moveDown(const int grid[21][10]);      //the bool return on downward moving functions tells if the block needs to be placed
                void moveLeft(const int grid[21][10]);      //move block left
                void moveRight(const int grid[21][10]);     //move block right
                int drop(const int grid[21][10]);           //int tells how many squares the block was dropped
                void rotate(const int grid[21][10]);        //try to rotate block, will move block left or right to successfully rotate

                //--------------------------------------------------------------------------
                // Collision Checks
                //--------------------------------------------------------------------------

                bool checkVerticalCollision(const int grid[21][10]) const;
                bool checkHorizontalCollision(int side, const int grid[21][10]) const;                                  // 0 for left side 1 for right side
                static bool checkRotationalCollision(std::pair<int, int>* position, const int grid[21][10]);            //helper functions to check for rotational collisions and move block accordingly for seemless rotations
                static bool isRotationalCollision(const std::pair<int, int>* position, const int grid[21][10]);         

            private:

                int rotation;                           //rotational nature of block (0-3)
                std::pair<int, int> pos[4];             //x and y position from the top left corner of the grid of each sqaure of the block (each block has only 4 squares)
                int id;                                 //id of block

        };


    private:

        //--------------------------------------------------------------------------
        // Tet's Power related variables
        //--------------------------------------------------------------------------

        //change level
        std::vector<std::pair<int, int>> changeLevel;                   //for changeLevel tet power. First int is increase by how many levels and second int is time

        //blind rows
        std::vector<std::tuple<int, int, double>> blindRows;            //first int is row number, second int is time left being blind, third double is the fade of the texture

        //rotate screen
        std::vector<std::tuple<int, int, bool>> rotations;              //first int is numeber of 90 degree rotations, second int is time, bool is for animation
                                                                        //Why a vector:
                                                                        //Tet class will determine what powerup tet does next, there is a chance that tet calls a screen rotate powerup
                                                                        //before the current one has finished. So a vector will allow us to have multipler Tet rotations active at the same time.

        int numRotations = 0;                                           //number of rotations done by a specific tet power, resets back to 0 once screen has rotated correct number of times
        bool subtract = false;                                          //for when we rotate the screen back to its original state
        std::vector<std::tuple<int, int, bool>>::iterator rotationIt;   //iterator for which rotation powerup we are currently on
        std::vector<std::tuple<int, int, bool>> null;                   //acts as a nullptr for rotationIt iterator
        double angVel = 0;                                              //current angular velocity of the screen rotation, max is 1.5
        double angAcc = 0.075;                                          //accelerates the angular velocity until it reaches 1.5
        bool shake = false;                                             //bool to determine whether we should shake the screen at each 90 degree rotation
        float maxOffsetRotation = 15;                                   //max offset of x and y for screen shake

        //tet fly screen shake
        float maxOffsetFly = 5;                                         //the max offset of x and y for when tet flys

        //resources
        Texture2D fog = LoadTexture("resources/tet/fog.png");           //for blind power

        //--------------------------------------------------------------------------
        // General Grid class variables
        //--------------------------------------------------------------------------

        //pointers for other classes
        Score* scr;
        Tet* tet;
        Powerup* pow;

        //constants
        const int xpos = 200, ypos = 100;               //number of pixels from the left and top of the window to the top left corner of grid
        const float gridsize = 30;                      //pixel size of each grid
        //1: I-block 2: J-block 3: L-block 4: O-block 5: S-block 6: T-block 7: Z-block
        const std::vector<Color> colors = {WHITE, SKYBLUE, DARKBLUE, ORANGE, YELLOW, LIME, PURPLE, RED, DARKGRAY};

        //upcoming blocks related variables
        Block block;                                    //current block that is moving
        std::deque<int> next;                           //next blocks                        
        RandomBlock randBlock = RandomBlock();          //used to get random blocks

        //block hold related variables
        int held = 0;                                   //current block being held
        bool ableToHold = true;                         //can we hold a new block now?

        //level and gameover variables
        int level = 0;                                  //level
        int rawLevel = 0;                               //raw level without tet power
        bool gameover = false;                          //true if player lost

        //camera
        Camera2D cameraMain = { 0 };                    //camera will mainly used for tet rotation power

        //font for the game
        Font allFont = LoadFont("resources/all_font.ttf");
        
        //our grid of placed blocks
        int grid[21][10];
};

#endif