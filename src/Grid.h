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
        void drawAll(bool block) {
            drawGrid(block);
            scr->drawScore();
            tet->drawTet();
            pow->drawPowerup();
        }
        //draws the grid and bool tells grid if block should also be drawn
        void drawGrid(bool block);
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
        void updatelevel(int l) {
            level = l;
            scr->updateLevel(l);
            pow->updateLevel(l);
        }

        //add the current block to the hold and current block in the hold becomes active block
        void hold();

        //powerup functions
        int lasers();
        int bomb();
        
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
        //is it game over?
        bool gameover = false;
        //our grid of placed blocks
        int grid[21][10];
        
};

#endif