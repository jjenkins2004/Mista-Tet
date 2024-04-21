#include "raylib.h"
#include "raymath.h"
#include "Grid.h"

int menu();

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;
    Grid grid = Grid();
    int counter = 0;
    bool start = true;

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

        //updating our block every so often
        if (counter == 60) {
            grid.moveDown();
            counter = 0;
            checkRows = true;
        }
        counter++;
        
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            //checking if keys are pressed and doing the corresponding action
            if (IsKeyPressed(KEY_RIGHT)) {
                grid.moveRight();
            }

            else if (IsKeyPressed(KEY_LEFT)) {
                grid.moveLeft();
            }

            else if (IsKeyPressed(KEY_UP)) {
                grid.rotate();
            }

            else if (IsKeyPressed(KEY_DOWN)) {
                grid.moveDown();
                checkRows = true;
            }

            else if (IsKeyPressed(KEY_SPACE)) {
                grid.drop();
                checkRows = true;
            }
            
            //do we need to check if a row is complete?
            if (checkRows) {
                std::vector<int> rowsRemoved = grid.checkRowComplete();
                if (!rowsRemoved.empty()) {
                    counter = 0;
                    //removes our grid rows and fixes, if -1 is returned then program needs to exit
                    if (grid.removeRow(rowsRemoved) == -1 || grid.fixRows(rowsRemoved) == -1) {
                        CloseWindow();
                        return 0;
                    }
                }
            }
            grid.drawGrid(true);

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
                DrawRectangleLinesEx((*cube).first, 3, Fade(BLACK, 0.2));
            }
            if (cubes.size() != 0 && cubes[0].first.y > 800+cubeSize) {
                cubes.assign(cubes.begin()+1, cubes.end());
            }


            //drawing our title and play button
            DrawText("Mista Tet", 150, 150, 100, GRAY);
            DrawRectangleRounded(playbutton, 50, 100, LIGHTGRAY);
            DrawRectangleRoundedLines(playbutton, 50, 100, 10, GRAY);
            DrawText("PLAY", 300, 415, 75, MAROON);

            //checking if mouse is over the play button and if play button is clicked
            int mX = GetMouseX(); int mY = GetMouseY();
            if ( mX >250 && mX < 490 && mY > 385 && mY < 485) {
                DrawRectangleRounded(playbutton, 50, 100, Fade(WHITE, 0.3));
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    return 0;
                }
            }

        EndDrawing();
    }
    return -1;
}