#include "raylib.h"
#include "raymath.h"
#include "Grid.h"
#include <cmath>

int menu();

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;
    Grid grid = Grid();
    Score* score = new Score();
    grid.setScoreBoard(score);

    int levelcounter = 0;
    int horizontalcounter = 0;
    int downcounter = 0;
    bool start = true;
    int level = 0;

    InitWindow(screenWidth, screenHeight, "TETris");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Main Menu
        //----------------------------------------------------------------------------------
        if (start) {
            int val = menu();
            if (val == -1) {
                break;
            }
            start = false;
        }
        //----------------------------------------------------------------------------------
        // Update
        //----------------------------------------------------------------------------------
        bool checkRows = false;

        //updating our block every so often, speed starts at 120 frames per movement but value of logistic growth function
        //using level as x that approaches 120
        int num = 240/(1+pow(M_E, (-0.05*level)))-120;
        if (levelcounter == 120-(num)) {
            levelcounter = 0;
            grid.moveDown();
            score->addScore(1);
            checkRows = true;
            grid.updatelevel(++level);
        }
        ++levelcounter;

        //checking if keys are pressed and doing the corresponding action
        if (IsKeyPressed(KEY_RIGHT)) {
            grid.moveRight();
            horizontalcounter = -7;
        }

        else if (IsKeyDown(KEY_RIGHT)) {
            if (horizontalcounter == 3) {
                horizontalcounter = 0;
                grid.moveRight();
            }
            else {
                ++horizontalcounter;
            }
        }

        else if (IsKeyPressed(KEY_LEFT)) {
            grid.moveLeft();
            horizontalcounter = -7;
        }

        else if (IsKeyDown(KEY_LEFT)) {
            if (horizontalcounter == 4) {
                horizontalcounter = 0;
                grid.moveLeft();
            }
            else {
                ++horizontalcounter;
            }
        }
    
        if (IsKeyPressed(KEY_DOWN)) {
            grid.moveDown();
            score->addScore(1);
            checkRows = true;
        }

        else if (IsKeyDown(KEY_DOWN)) {
            if (downcounter == 4) {
                downcounter = 0;
                grid.moveDown();
                score->addScore(1);
                checkRows = true;
            }
            else {
                ++downcounter;
            }
        }

        else if (IsKeyPressed(KEY_SPACE)) {
            score->addScore(grid.drop()*1.5);
            checkRows = true;
        }

        if (IsKeyPressed(KEY_UP)) {
            grid.rotate();
        }
        
        //do we need to check if a row is complete?
        if (checkRows) {
            std::vector<int> rowsRemoved = grid.checkRowComplete();
            levelcounter = 0;
            //removes our grid rows and fixes, if -1 is returned then program needs to exit
            if (!rowsRemoved.empty()) {
                if (grid.removeRow(rowsRemoved) == -1 || grid.fixRows(rowsRemoved) == -1) {
                    CloseWindow();
                    return 0;
                }
            }
        }


        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
            BeginDrawing();

                ClearBackground(BLACK);
                grid.drawAll(true);

            EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();
    return 0;
}

int menu() {
    //defining the play button rectangle
    Rectangle playbutton; playbutton.x = 275; playbutton.y = 400; playbutton.width = 240; playbutton.height = 100;

    //conunter for our background cubes 
    const std::vector<Color> colors = {SKYBLUE, DARKBLUE, ORANGE, YELLOW, LIME, PURPLE, RED};
    std::vector<std::pair<Rectangle, Color>> cubes;
    int addCubeCounter = 0;
    int cubeSize = 20;

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(BLACK);

            //cool background cubes falling to make menu a little more intersting
            if (addCubeCounter == 30) {
                Rectangle c; c.x = GetRandomValue(0, 800-cubeSize); c.y = -cubeSize; c.width = cubeSize; c.height = cubeSize;
                cubes.push_back(std::make_pair(c, colors[GetRandomValue(0, 6)]));
                addCubeCounter = 0;
            }
            else {
                addCubeCounter++;
            }

            for (std::vector<std::pair<Rectangle, Color>>::iterator cube = cubes.begin(); cube != cubes.end(); cube++) {
                    cube->first.y+=1;
                DrawRectangleRec((*cube).first, (*cube).second);
                DrawRectangleLinesEx((*cube).first, 4, Fade(BLACK, 0.2));
            }
            if (cubes.size() != 0 && cubes[0].first.y > 800+cubeSize) {
                cubes.assign(cubes.begin()+1, cubes.end());
            }


            //drawing our title and play button
            DrawText("Mista Tet", 150, 150, 100, Fade(GRAY, 0.7));
            DrawRectangleRounded(playbutton, 50, 100, Fade(LIGHTGRAY, 0.7));
            DrawRectangleRoundedLines(playbutton, 50, 100, 10, Fade(GRAY, 0.7));
            DrawText("PLAY", 300, 415, 75, Fade(MAROON, 0.7));

            //checking if mouse is over the play button and if play button is clicked
            int mX = GetMouseX(); int mY = GetMouseY();
            if ( mX >playbutton.x && mX < playbutton.x+playbutton.width && mY > playbutton.y && mY < playbutton.y+playbutton.height) {
                DrawRectangleRounded(playbutton, 50, 100, Fade(WHITE, 0.3));
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    return 0;
                }
            }

        EndDrawing();
    }
    return -1;
}