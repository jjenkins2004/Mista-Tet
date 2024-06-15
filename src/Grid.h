#ifndef GRID_H
#define GRID_H

#include "raylib.h"
#include "RandomBlock.h"
#include <vector>
#include <ctime>
#include <queue>
#include <iostream>
#include "Score.h"
#include "Tet.h"
#include "Powerup.h"


class Grid {
    public:
        Grid();
        //set scr to pointer
        void setScoreBoard(Score* score) {
            scr = score;
        }
        //set tet to pointer
        void setTet(Tet* t) {
            tet = t;
        }
        void setPowerup(Powerup* p) {
            pow = p;
        }
        //convinient funciton that draws everything
        void drawAll(bool block, bool useCamera);
        //draws the grid and bool tells grid if block should also be drawn
        void drawGrid(bool block);
        //convinient function that calls all updating functions
        void updateAll();
        //functions for adjusting camera related to tet turn power
        void updateCamera();
        void randomRotate();
        //block movements (true if block needs to be generated, false otherwise)
        void moveDown() {
            if (!block.moveDown(grid)) {
                this->placeBlock();
                this->generateBlock();
            }
        }
        void moveRight() {
            block.moveRight(grid);
        }
        void moveLeft() {
            block.moveLeft(grid);
        }
        void rotate() {
            block.rotate(grid);
        }
        int drop() {
            int r = block.drop(grid);
            this->placeBlock();
            this->generateBlock();
            return r;
        }

        //block placement, generation
        void placeBlock();
        void generateBlock();

        //removes rows in argument and plays animation to make it look cool!
        int removeRow(std::vector<int>& rows);
        //returns which rows need to be removed in a vector
        const std::vector<int> checkRowComplete() const;
        //fixes rows after removal of rows in argument vector 
        int fixRows(std::vector<int> rows);

        //game over
        bool checkGameOver() {
            return gameover;
        }

        //update level
        void updatelevel();

        //get level
        int getLevel() {
            return level;
        }

        //increase level for 30 seconds (tet power)
        void increaseLevel(int x);

        //blind random rows on board, parameter is number of rows
        void blind(int num);

        //add the current block to the hold and current block in the hold becomes active block
        void hold();

        //powerup functions
        int lasers();
        int bomb();
        int nuke();
        void changeNext(int i);
        
        //the class that keeps track of the current moving block
        class Block {
            public:
                //creates a block based on given id
                Block(int id, int grid[21][10]);
                Block() {};

                //getters and setters
                const std::pair<int, int>* getPos() {
                    return pos;
                }

                const int getId() {
                    return id;
                }

                //block movements (bool for downwards to tell if block needs to be placed)
                bool moveDown(const int grid[21][10]);
                void moveLeft(const int grid[21][10]);
                void moveRight(const int grid[21][10]);
                int drop(const int grid[21][10]);
                void rotate(const int grid[21][10]);

                //collision checks, returns true if a collision is imminent
                bool checkVerticalCollision(const int grid[21][10]) const;
                // 0 for left side 1 for right side
                bool checkHorizontalCollision(int side, const int grid[21][10]) const;
                //helper functions to check for rotational collisions and move block accordingly for seemless rotations
                static bool checkRotationalCollision(std::pair<int, int>* position, const int grid[21][10]);
                static bool isRotationalCollision(const std::pair<int, int>* position, const int grid[21][10]);

            private:
                //rotational nature of block (0-3)
                int rotation;
                //x and y position from the top left corner of the grid of each sqaure of the block (each block has only 4 squares)
                std::pair<int, int> pos[4];
                int id;
        };


    private:
        //tet power related variables
        std::vector<std::pair<int, int>> changeLevel;
        std::vector<std::tuple<int, int, double>> blindRows; //first int is row number, second int is time left being blind, third double is the fade of the texture
        Texture2D fog = LoadTexture("resources/tet/fog.png");
        std::vector<std::tuple<int, int, bool>> rotations; //first int is numeber of 90 degree rotations, second int is time, bool is for animation
        int numRotations = 0;
        bool subtract = false;
        std::vector<std::tuple<int, int, bool>>::iterator rotationIt; std::vector<std::tuple<int, int, bool>> null;
        double angVel = 0.05; double angAcc = 0.01;
        bool shake = false; float maxOffsetRotation = 15;
        Sound rotateSound = LoadSound("resources/audio/ScreenTurn.wav");
        Sound rotationRumble = LoadSound("resources/audio/RotationRumble.wav");


        //camera
        Camera2D cameraMain = { 0 };
        //font
        Font allFont = LoadFont("resources/allFont.ttf");
        //scoreboard pointer
        Score* scr;
        //tet pointer
        Tet* tet;
        //powerup class pointer
        Powerup* pow;
        //number of pixels from the left and top of the window to the top left corner of grid
        const int xpos = 200;
        const int ypos = 100;
        //pixel size of each grid
        const float gridsize = 30;
        //1: I-block 2: J-block 3: L-block 4: O-block 5: S-block 6: T-block 7: Z-block
        const std::vector<Color> colors = {WHITE, SKYBLUE, DARKBLUE, ORANGE, YELLOW, LIME, PURPLE, RED, DARKGRAY};
        //current block that is moving
        Block block;
        //next blocks
        std::deque<int> next;
        //current block being held
        int held = 0; bool ableToHold = true;
        //used to get random blocks
        RandomBlock randBlock = RandomBlock();
        //level
        int level = 0;
        //raw level without tet
        int rawLevel = 0;
        //is it game over?
        bool gameover = false;
        //our grid of placed blocks
        int grid[21][10];
        
};

#endif