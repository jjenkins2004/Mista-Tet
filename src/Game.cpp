#include "raylib.h"
#include "raymath.h"
#include "Grid.h"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;
    Grid grid = Grid();
    int counter = 0;

    InitWindow(screenWidth, screenHeight, "TETris");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
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