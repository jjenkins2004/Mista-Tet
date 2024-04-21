#include "Grid.h"



/*****
*** START OF GRID CLASS 
******/


Grid::Grid() {
    srand(time(NULL));
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = 0;
        }
    }

    //setting up our next array for upcoming blocks and giving a starting block
    for (int i = 0; i < 3; i++) {
        next.push(randBlock.getBlock());
    }
    block = Block(randBlock.getBlock(), grid);
}

void Grid::drawGrid(bool drawBlock) {
    int borderWidth = 10;
    Color borderColor = GRAY;

    //border for grid
    DrawRectangle(xpos-borderWidth, ypos-borderWidth, 10*gridsize+2*borderWidth, borderWidth, borderColor);
    DrawRectangle(xpos-borderWidth, ypos, borderWidth, gridsize*20+borderWidth, borderColor);
    DrawRectangle(xpos+10*gridsize, ypos, borderWidth, gridsize*20+borderWidth, borderColor);
    DrawRectangle(xpos-10, ypos+20*gridsize, 10*gridsize+2*borderWidth, borderWidth, borderColor);

    //filling in 10x20 grid based on the 2d array
    for (int i = 1; i < 21; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] != 0) {
                Rectangle rec; rec.x = xpos+j*gridsize; rec.y = ypos+i*gridsize; rec.width = gridsize; rec.height = gridsize;
                DrawRectangleRec(rec, colors[grid[i][j]]);
                DrawRectangleLinesEx(rec, 2, Fade(BLACK, 0.2));
            }
        }
    }

    //drawing our block
    if (drawBlock) {
        for (int i = 0; i < 4; i++) {
            if (block.getPos()[i].second != 0) {
                Rectangle rec; rec.x = xpos+block.getPos()[i].first*gridsize; rec.y = ypos+(block.getPos()[i].second-1)*gridsize;
                rec.height = gridsize; rec.width = gridsize; 
                DrawRectangleRec(rec, colors[block.getId()]);
                DrawRectangleLinesEx(rec, 2, Fade(BLACK, 0.2));
            }
        }
        int dist = 0;

        //finds out how much distance between block and the ground or another block below it to figure out where to draw the
        //the ghost block that shows where block is going
        while (true) {
            bool leave = false;
            for (int i = 0; i < 4; i++) {
                if (block.getPos()[i].second+dist == 19 || grid[block.getPos()[i].second+dist+1][block.getPos()[i].first] != 0) {
                    leave = true;
                    break;
                }
            }
            if (leave) {
                break;
            } else {
                dist++;
            }
        }
        for (int i = 0; i < 4; i++) {
            Rectangle rec; rec.x = xpos+block.getPos()[i].first*gridsize; rec.y = ypos+(block.getPos()[i].second+dist)*gridsize;
            rec.height = gridsize; rec.width = gridsize;
            DrawRectangleRec(rec, Fade(colors[block.getId()], 0.5));
            DrawRectangleLinesEx(rec, 2, Fade(BLACK, 0.1));
        }
    }
}

void Grid::placeBlock() {
    bool lose = false;
    const std::pair<int, int>* pos = block.getPos();
    for (int i = 0; i < 4; i++) {
        grid[pos[i].second][pos[i].first] = block.getId();
        if (pos[i].second == 1) {
            lose = true;
        }
    }
    if (lose) {
        int counter = 0;
        while (!WindowShouldClose()) {
            BeginDrawing();
                drawGrid(true);
                counter++;
                if (counter == 120) {
                    break;
                }
            EndDrawing();
        }
        gameOver();
    }
}

void Grid::generateBlock() {
    block = Block(next.front(), grid);
    next.pop();
    next.push(randBlock.getBlock());
}

const std::vector<int> Grid::checkRowComplete() const{
    std::vector<int> rows;
    for (int i = 1; i < 21; i++) {
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
    std::vector<int*> cells;
    bool first = true;
    //adds every cell that we need to remove as an int* in the correct order
    for (int r: rows) {
        for (int i = 0; i < 10; i++) {
            grid[r][i] = 8;
            cells.push_back(&grid[r][i]);
        }
    }
    std::vector<int*>::iterator it = cells.begin();
    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            if (!first && it != cells.end()) {
                **it = 0;
                it++;
                WaitTime(1/60);
            } else if (it == cells.end()) {
                WaitTime(0.3);
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

int Grid::fixRows(std::vector<int> rows) {
    //initial checks to see if any fixing is needed
    bool leave = true;
    if (rows[0] == 1) {
        return 0;
    }
    for (int i = 0; i < 10; i++) {
        if (grid[rows[0]-1][i] != 0) {
            leave = false;
            break;
        }
    }
    if (leave) {
        return 0;
    }

    /*****
     * The logic is that we start from the lowest row removed (which is the last element of the removed rows vector), then 
     * iterate through the removed rows vector, in order to find from which row we need to move all our tetris pieces down.
     * However, this program also takes into account if the removed rows are not all together. For example, if rows 19 and 17
     * were removed, then the program would first find row 18 as the start and move everything about row 18 down a distance of one.
     * Then it would find row 17 (17 becuase we moved everything down one already) as a new starting 
     * point (there is still a gap at row 18). Finally, it would fill this gap.
    *****/
    std::vector<int>::iterator it = rows.end();
    int distance = 0;
    int row;
    while(!WindowShouldClose()) {
        if (distance == 0 && it != rows.begin()) {
            it--;
             while (it != rows.begin() && (*it - *(it-1)) == 1) {
                it = it-1;
                distance++;
            }
            row = (*it)-1;
            distance++;
        }
        else if (distance == 0) {
            WaitTime(0.15);
            return 0;
        }
        for (int i = 0; i < rows.size(); i++) {
            rows[i]+=distance;
        }
            
        BeginDrawing();
            ClearBackground(BLACK);
            for (int i = row; i >= 0; i--) {
                for (int j = 0; j < 10; j++) {
                    grid[i+1][j] = grid[i][j];
                }
            }
            distance--; row++;
        drawGrid(false);
        WaitTime(.15);
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

Grid::Block::Block(int id, int grid[20][10]): id(id), rotation(0) {

    //I-block
    if (id == 1) {
        pos[0] = std::make_pair(3, 2);
        pos[1] = std::make_pair(4, 2);
        pos[2] = std::make_pair(5, 2);
        pos[3] = std::make_pair(6, 2);
    }

    //J-block
    else if (id == 2) {
        pos[0] = std::make_pair(3, 1);
        pos[1] = std::make_pair(3, 2);
        pos[2] = std::make_pair(4, 2);
        pos[3] = std::make_pair(5, 2);

    }

    //L-block
    else if (id == 3) {
        pos[0] = std::make_pair(5, 1);
        pos[1] = std::make_pair(5, 2);
        pos[2] = std::make_pair(4, 2);
        pos[3] = std::make_pair(3, 2);
    }

    //O-block
    else if (id == 4) {
        pos[0] = std::make_pair(4, 1);
        pos[1] = std::make_pair(4, 2);
        pos[2] = std::make_pair(5, 2);
        pos[3] = std::make_pair(5, 1);
    }

    //S-block
    else if (id == 5) {
        pos[0] = std::make_pair(5, 1);
        pos[1] = std::make_pair(4, 1);
        pos[2] = std::make_pair(4, 2);
        pos[3] = std::make_pair(3, 2);
    }

    //T-block
    else if (id == 6) {
        pos[0] = std::make_pair(3, 2);
        pos[1] = std::make_pair(4, 2);
        pos[2] = std::make_pair(4, 1);
        pos[3] = std::make_pair(5, 2);
    }
    
    //Z-block
    else if (id == 7) {
        pos[0] = std::make_pair(4, 1);
        pos[1] = std::make_pair(5, 1);
        pos[2] = std::make_pair(5, 2);
        pos[3] = std::make_pair(6, 2);
    }

    //see if we need to put the block one higher
    if (checkVerticalCollision(grid)) {
        for (int i = 0; i < 4; i++) {
            pos[i].second--;
        }
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
