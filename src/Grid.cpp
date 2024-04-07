#include "Grid.h"



/*****
*** START OF GRID CLASS 
******/


Grid::Grid() {
    srand(time(NULL));
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = 0;
        }
    }

    //setting up our next array for upcoming blocks and giving a starting block
    for (int i = 0; i < 3; i++) {
        next.push(/*rand()%7+1*/1);
    }
    block = Block(/*rand()%7+1*/1);
}

void Grid::drawGrid(bool drawBlock) {
    int borderWidth = 10;
    Color borderColor = GRAY;

    //border for grid
    DrawRectangle(xpos-borderWidth, ypos-borderWidth, 10*gridsize+2*borderWidth, borderWidth, borderColor);
    DrawRectangle(xpos-borderWidth, ypos, borderWidth, gridsize*20+borderWidth, borderColor);
    DrawRectangle(xpos+10*gridsize, ypos, borderWidth, gridsize*20+borderWidth, borderColor);
    DrawRectangle(xpos-10, ypos+20*gridsize, 10*gridsize+2*borderWidth, borderWidth, borderColor);

    //filling in 10x20 grid based on the 2d array and considering the skiped rows
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] != 0) {
                DrawRectangle(xpos+j*gridsize, ypos+i*gridsize, gridsize, gridsize, colors[grid[i][j]]);
            }
        }
    }

    //drawing our block
    if (drawBlock) {
        for (int i = 0; i < 4; i++) {
            DrawRectangle(xpos+block.getPos()[i].first*gridsize, ypos+block.getPos()[i].second*gridsize, gridsize, gridsize, colors[block.getId()]);
        }
    }
}

void Grid::placeBlock() {
    const std::pair<int, int>* pos = block.getPos();
    for (int i = 0; i < 4; i++) {
        grid[pos[i].second][pos[i].first] = block.getId();
    }
}

void Grid::generateBlock() {
    block = Block(next.front());
    next.pop();
    next.push(/*rand()%7+1*/1);
}

const std::vector<int> Grid::checkRowComplete() const{
    std::vector<int> rows;
    for (int i = 0; i < 20; i++) {
        bool addRow = true;
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == 0) {
                addRow = false;
                break;
            }
        }
        if (addRow) {
            rows.push_back(i);
        }
    }
    return rows;
}

int Grid::removeRow(std::vector<int>& rows) {
    int animationCount = 0;
    bool first = true;
    for (int r: rows) {
        for (int i = 0; i < 10; i++) {
            grid[r][i] = 8;
        }
    }
    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            if (!first && animationCount < rows.size()*10) {
                grid[animationCount/10+rows[0]][animationCount%10] = 0;
                animationCount++;
                WaitTime(1/60);
            } else if (animationCount == rows.size()*10) {
                WaitTime(0.4);
                return 0;
            }
            this->drawGrid(false);
        EndDrawing();
        if (first) {
            WaitTime(0.2);
            first = false;
        }
    }
    return -1;
}

int Grid::fixRows(int r, int movement) {
    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            if (movement == 0) {
                return 0;
            }
            for (int i = r; i >= 0; i--) {
                for (int j = 0; j < 10; j++) {
                    grid[i+1][j] = grid[i][j];
                }
            }
            r++;
            for (int i = 0; i < 10; i++) {
                grid[0][i] = 0;
            }
            this->drawGrid(false);
            WaitTime(0.15);
            movement--;
        EndDrawing();
    }
    return -1;
}

/*****
*** END OF GRID CLASS 
******/

/*****
*** START OF BLOCK CLASS 
******/

Grid::Block::Block(int id): id(id), rotation(0) {

    //I-block
    if (id == 1) {
        pos[0] = std::make_pair(3, 1);
        pos[1] = std::make_pair(4, 1);
        pos[2] = std::make_pair(5, 1);
        pos[3] = std::make_pair(6, 1);
    }

    //J-block
    else if (id == 2) {
        pos[0] = std::make_pair(3, 0);
        pos[1] = std::make_pair(3, 1);
        pos[2] = std::make_pair(4, 1);
        pos[3] = std::make_pair(5, 1);

    }

    //L-block
    else if (id == 3) {
        pos[0] = std::make_pair(5, 0);
        pos[1] = std::make_pair(5, 1);
        pos[2] = std::make_pair(4, 1);
        pos[3] = std::make_pair(3, 1);
    }

    //O-block
    else if (id == 4) {
        pos[0] = std::make_pair(4, 0);
        pos[1] = std::make_pair(4, 1);
        pos[2] = std::make_pair(5, 1);
        pos[3] = std::make_pair(5, 0);
    }

    //S-block
    else if (id == 5) {
        pos[0] = std::make_pair(5, 0);
        pos[1] = std::make_pair(4, 0);
        pos[2] = std::make_pair(4, 1);
        pos[3] = std::make_pair(3, 1);
    }

    //T-block
    else if (id == 6) {
        pos[0] = std::make_pair(3, 1);
        pos[1] = std::make_pair(4, 1);
        pos[2] = std::make_pair(4, 0);
        pos[3] = std::make_pair(5, 1);
    }
    
    //Z-block
    else if (id == 7) {
        pos[0] = std::make_pair(4, 0);
        pos[1] = std::make_pair(5, 0);
        pos[2] = std::make_pair(5, 1);
        pos[3] = std::make_pair(6, 1);
    }
}

/*
* MOVEMENTS
*/
bool Grid::Block::moveDown(const int grid[20][10]) {
    if (checkVerticalCollision(grid)) {
        return false;
    }
    for (int i = 0; i < 4; i++) {
        pos[i].second++;
    }
    return true;
}

void Grid::Block::moveRight(const int grid[20][10]) {
   if (checkHorizontalCollision(1, grid)) {
    return;
   }
    for (int i = 0; i < 4; i++) {
        pos[i].first++;
    }
}

void Grid::Block::moveLeft(const int grid[20][10]) {
    if (checkHorizontalCollision(0, grid)) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        pos[i].first--;
    }
}

void Grid::Block::drop(const int grid[20][10]) {
    while (!checkVerticalCollision(grid)) {
        for (int i = 0; i < 4; i++) {
            pos[i].second++;
        }
    }
}

void Grid::Block::rotate(const int grid[20][10]) {
    std::pair<int, int> backUpPos[4];
    for (int i = 0; i < 4; i++) {
        backUpPos[i] = pos[i];
    }

    //I-block rotation
    if (id == 1) {
        if (rotation == 0) {
            pos[0].first+=2;
            pos[0].second--;
            pos[1].first++;
            pos[2].second++;
            pos[3].first--;
            pos[3].second+=2;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].first-=2;
            pos[0].second+=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].first++;
            pos[0].second-=2;
            pos[1].second--;
            pos[2].first--;
            pos[3].first-=2;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }

        else if (rotation == 3) {
            pos[0].first--;
            pos[0].second++;
            pos[2].first++;
            pos[2].second--;
            pos[3].first+=2;
            pos[3].second-=2;
            if (checkRotationalCollision(pos, grid)) {
                rotation = 0;
                return;
            }
        }
    }
    //J-block rotation
    else if (id == 2) {
        if (rotation == 0) {
            pos[0].first+=2;
            pos[1].first++;
            pos[1].second--;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].second+=2;
            pos[1].first++;
            pos[1].second++;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].first-=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].second-=2;
            pos[1].first--;
            pos[1].second--;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation = 0;
                return;
            }
        }
    }
    else if (id == 3) {
        if (rotation == 0) {
            pos[0].second+=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].first-=2;
            pos[1].first--;
            pos[1].second--;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].second-=2;
            pos[1].first++;
            pos[1].second--;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].first+=2;
            pos[1].first++;
            pos[1].second++;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation = 0;
                return;
            }
        }
    }
    else if (id == 5) {
        if (rotation == 0) {
            pos[0].second+=2;
            pos[1].first++;
            pos[1].second++;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].first-=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].second-=2;
            pos[1].first--;
            pos[1].second--;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].first+=2;
            pos[1].first++;
            pos[1].second--;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation = 0;
                return;
            }
        }
    }
    else if (id == 6) {
        if (rotation == 0) {
            pos[0].first++;
            pos[0].second--;
            pos[2].first++;
            pos[2].second++;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].first++;
            pos[0].second++;
            pos[2].first--;
            pos[2].second++;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].first--;
            pos[0].second++;
            pos[2].first--;
            pos[2].second--;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].first--;
            pos[0].second--;
            pos[2].first++;
            pos[2].second--;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation = 0;
                return;
            }
        }
    }
    else if (id == 7) {
        if (rotation == 0) {
            pos[0].first+=2;
            pos[1].first++;
            pos[1].second++;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].second+=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].first-=2;
            pos[1].first--;
            pos[1].second--;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].second-=2;
            pos[1].first++;
            pos[1].second--;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                rotation = 0;
                return;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        pos[i] = backUpPos[i];
    }
}

/*
* COLLISION CHECKS
*/

bool Grid::Block::checkHorizontalCollision(int side, const int grid[20][10]) const{
    if (side == 0) {
        for (int i = 0; i < 4; i++) {
            if (pos[i].first == 0 || grid[pos[i].second][pos[i].first-1] != 0) {
                return true;
            }
        }
        return false;
    }
    if (side == 1) {
        for (int i = 0; i < 4; i++) {
            if (pos[i].first == 9 || grid[pos[i].second][pos[i].first+1] != 0) {
                return true;
            }
        }
        return false;
    }
    return false;
}

bool Grid::Block::checkVerticalCollision(const int grid[20][10]) const {
    for (int i = 0; i < 4; i++) {
        if (pos[i].second == 19 || grid[pos[i].second+1][pos[i].first] != 0) {
            return true;
        }
    }
    return false;
}

bool Grid::Block::checkRotationalCollision(std::pair<int, int>* position, const int grid[20][10]) {
    //translations that the function will try for a valid rotation
    std::vector<int> xVals = {1, 2, -1, -2};
    std::vector<int> yVals = {0, 0, 0, 0};

    std::pair<int, int> positionCheck[4];
    for (int i = 0; i < 4; i++) {
        positionCheck[i] = position[i];
    }

    //if there is a collision then proceed
    if (isRotationalCollision(position, grid)) {
        //go through xVals and yVals for different translations
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                positionCheck[j].first+=xVals[i];
                positionCheck[j].second+=yVals[i];
            }
            if (isRotationalCollision(positionCheck, grid)) {
                //revert back because this translation doesn't work
                for (int i = 0; i < 4; i++) {
                    positionCheck[i] = position[i];
                }
            }
            else {
                //the first translation that works will be the one that we use
                for (int i = 0; i < 4; i++) {
                    position[i] = positionCheck[i];
                }
                return true;
            }
        }
        return false;
    } else {
        return true;
    }
}

bool Grid::Block::isRotationalCollision(const std::pair<int, int>* position, const int grid[20][10]) {
    for (int i = 0; i < 4; i++) {
        //checking for out of bounds and collision with another block
        if (position[i].first > 9 || position[i].first < 0 || position[i].second < 0 || position[i].second > 19 || grid[position[i].second][position[i].first] != 0) {
            return true;
        }
    }
    return false;
}

/*****
*** END OF BLOCK CLASS 
******/
