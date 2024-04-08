#include "raylib.h"
#include "RandomBlock.h"
#include <vector>
#include <ctime>
#include <queue>
#include <iostream>


class Grid {
    public:
        Grid();
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
        void drop() {
            block.drop(grid);
            this->placeBlock();
            this->generateBlock();
        }

        //block placement, generation
        void placeBlock();
        void generateBlock();

        //removes rows in parameter and plays animation to make it look cool!
        int removeRow(std::vector<int>& rows);
        //returns which rows need to be removed in a vector
        const std::vector<int> checkRowComplete() const;
        //fixes rows after row has been removed from parameter row up, int movement tells how many rows to move upper blocks down
        int fixRows(int r, int movement);

        
        //the class that keeps track of the current moving block
        class Block {
            public:
                //creates a block based on given id
                Block(int id);
                Block() {};

                //getters and setters
                const std::pair<int, int>* getPos() {
                    return pos;
                }

                const int getId() {
                    return id;
                }

                //block movements (bool for downwards to tell if block needs to be placed)
                bool moveDown(const int grid[20][10]);
                void moveLeft(const int grid[20][10]);
                void moveRight(const int grid[20][10]);
                void drop(const int grid[20][10]);
                void rotate(const int grid[20][10]);

                //collision checks, returns true if a collision is imminent
                bool checkVerticalCollision(const int grid[20][10]) const;
                // 0 for left side 1 for right side
                bool checkHorizontalCollision(int side, const int grid[20][10]) const;
                //helper functions to check for rotational collisions and move block accordingly for seemless rotations
                static bool checkRotationalCollision(std::pair<int, int>* position, const int grid[20][10]);
                static bool isRotationalCollision(const std::pair<int, int>* position, const int grid[20][10]);

            private:
                //rotational nature of block (0-3)
                int rotation;
                //x and y position from the top left corner of the grid of each sqaure of the block (each block has only 4 squares)
                std::pair<int, int> pos[4];
                int id;
        };


    private:
        //number of pixels from the left and top of the window to the top left corner of grid
        const int xpos = 200;
        const int ypos = 100;
        //pixel size of each grid
        const int gridsize = 30;
        //1: I-block 2: J-block 3: L-block 4: O-block 5: S-block 6: T-block 7: Z-block
        const std::vector<Color> colors = {WHITE, SKYBLUE, DARKBLUE, ORANGE, YELLOW, LIME, PURPLE, RED, DARKGRAY};
        //current block that is moving
        Block block;
        //next blocks
        std::queue<int> next;
        //used to get random blocks
        RandomBlock randBlock = RandomBlock();
        //our grid of placed blocks
        int grid[20][10];
        
};