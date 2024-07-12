//--------------------------------------------------------------------------
// Includes, constants, pre-definitions
//--------------------------------------------------------------------------

#include "raylib.h"
#include "Grid.h"
#include <cmath>

#define screenWidth 800
#define screenHeight 800

int menu();
int pause();
int usePower(PowerupItem* p, Score* src, Grid* grid);

int main() {

    //--------------------------------------------------------------------------
    // Initialization
    //--------------------------------------------------------------------------


    SetTargetFPS(60);                                       //setting game FPS to 60, game loop runs 60 times per second
    InitWindow(screenWidth, screenHeight, "TETris");        //initializing our window
    InitAudioDevice();                                      //initializing audio device
    ChangeDirectory(GetApplicationDirectory());             //changing default directory to the bin folder where build is located

    //defining our other classes
    Grid* grid = nullptr;
    Score* score;
    Tet* tet;
    Powerup* powerUp;

    //counters and such to only execute a line of code after a certain amount of time
    int levelcounter = 0;                                   //counter to keep track of when we should move block down
    int horizontalcounter = 0;                              //cooldown for moving block horizontally
    int downcounter = 0;                                    //cooldown for moving block down
    int powerupcounter = 0;                                 //keeps track of when to spawn new powerup
    bool start = true;                                      //bool to bring us to the start menu
    int spawnpower = -1;         //how often we should spawn powerup

    //other
    Font titleFont = LoadFont("resources/titleFont.ttf");   //title font
    std::vector<std::pair<double, int>> speedChange;        //for our speedchange powerup, double represents how changed and int is time left of effect
    int level = 0;                                          //the current level, determines how fast the blocks fall


    //--------------------------------------------------------------------------
    // Main Game Loop
    //--------------------------------------------------------------------------


    while (!WindowShouldClose()) {                      // returns false when window is closed or ESC key is pressed, thus ending the game

        //--------------------------------------------------------------------------
        // Main Manu into Tet Monologue
        //--------------------------------------------------------------------------


        if (start) {                                    //if start is true, will reset everything and bring up start menu
            
            //deleting prevous data for replayability
            if (grid != nullptr) {
                delete grid;
                delete score;
                delete tet;
                delete powerUp;
            }

            //creating fresh classes and setting appropriate variables
            grid = new Grid();
            score = new Score();
            tet = new Tet(score);
            powerUp = new Powerup();
            grid->setScoreBoard(score);
            grid->setTet(tet);
            grid->setPowerup(powerUp);
            powerUp->setScore(score);
            level = 0;
            levelcounter = 0;
            horizontalcounter = 0;
            downcounter = 0;

            //temp
            //grid->randomRotate();
            score->addScore(74500);
            powerUp->spawnPowerup(true);
            start = false; continue;

            //bringing up our start menu
            if (menu() == -1) break;
            start = false;

            //bringing up our tet Monologue
            if (tet->tetMonologue() == -1) break;

            //to fade in the scene of game from the Tet Monologue
            double fade = 1;
            bool quit = true;
            while (!WindowShouldClose()) {
                BeginDrawing();
                    ClearBackground(BLACK);
                    grid->drawAll(true, true);
                    DrawRectangle(0, 0, 800, 800, Fade(BLACK, fade));       //covering the whole screen with a black rectangle and decreasing transparency for a fade from black
                EndDrawing();
                fade-=0.01;
                if (fade <= 0) {
                    quit = false;
                    break;
                }
            }
            if (quit) break;                                                //check if game should quit

        }


        //----------------------------------------------------------------------------------
        // Checking and calling updates from other classes
        //----------------------------------------------------------------------------------


        bool checkRows = false;                             //true if we should check if rows on the board are completed and need to be removed

        //updating functions
        grid->updateAll();

        //updating our block every so often, speed starts at 120 frames per movement but value of logistic growth function
        //using level as x that approaches 120
        level = grid->getLevel();                               //get updated level
        int num = 120 - (240/(1+pow(M_E, (-0.05*level)))-120);  //using logistic growth function to find how many frames should pass before block is moved automatically down  
                                                                //num of frames that should pass exponentially decreases with each increase of level
        //for the speed change powerup, which modifies how fast block automatically moves
        for (std::vector<std::pair<double, int>>::iterator it = speedChange.begin(); it != speedChange.end(); it++) {
            if (!(num < 0 && it->first < 0)) num*=it->first;
            if (num < 1 && num > 0) num = 1;
            it->second--;
            if (it->second == 0) {
                if (it->first < 0) levelcounter = 0;
                speedChange.erase(it);
                it--;
            }
        }

        //check if we should move block down
        if (levelcounter >= num) {
            levelcounter = 0;
            grid->moveDown();
            score->addScore(1);
            checkRows = true;
        }
        levelcounter++;


        //----------------------------------------------------------------------------------
        // Powerups from Tet and Player
        //----------------------------------------------------------------------------------


        //check if we should spawn a new powerup
        if (powerupcounter == spawnpower) {
            powerUp->spawnPowerup(true);
            powerupcounter = 0;
            //spawnpower = GetRandomValue(3*60, 5*60);
        }
        powerupcounter++;

        //checking if tet used a new power and acting accordingly
        std::string power = tet->checkTetPower();
        if (power == "level1") grid->increaseLevel(15);
        else if (power == "level2") grid->increaseLevel(25);
        else if (power == "blind1") grid->blind(GetRandomValue(5, 7));
        else if (power == "blind2") grid->blind(GetRandomValue(8, 12));
        else if (power == "zblock") grid->changeNext(5);
        else if (power == "sblock") grid->changeNext(7);
        else if (power == "turn") grid->randomRotate();

        //ask powerup class if there are any fast speedchange
        //need a seperate function because we don't want to store this powerup for later use and the powerup affects
        //game.cpp class
        std::vector<std::pair<double, int>> temp = powerUp->checkFastSpeed();
        speedChange.insert(speedChange.end(), temp.begin(), temp.end());

        //check if any stored powerups were used
        PowerupItem* p = nullptr;
        if (IsKeyPressed(KEY_ONE)) p = powerUp->usePowerup(1);
        else if (IsKeyPressed(KEY_TWO)) p = powerUp->usePowerup(2);
        else if (IsKeyPressed(KEY_THREE)) p = powerUp->usePowerup(3);
        if (p != nullptr && p->id == "speedchange") {
            SpeedChange* s = dynamic_cast<SpeedChange*>(p);
            if (s->variant == 0) speedChange.push_back(std::make_pair(-1, 1800));
            else if (s->variant == -1) speedChange.push_back(std::make_pair(1.3, 1800));
            else if (s->variant == -2) speedChange.push_back(std::make_pair(1.5, 1800));
        }
        else if (p != nullptr && usePower(p, score, grid) == -1) break;


        //----------------------------------------------------------------------------------
        // User Input
        //----------------------------------------------------------------------------------


        //right arrow key to move right
        if (IsKeyPressed(KEY_RIGHT)) {
            grid->moveRight();
            horizontalcounter = -7;
        }

        //right arrow key held down
        else if (IsKeyDown(KEY_RIGHT)) {
            if (horizontalcounter == 3) {
                horizontalcounter = 0;
                grid->moveRight();
            }
            else ++horizontalcounter;
        }

        //left arrow key to move left
        else if (IsKeyPressed(KEY_LEFT)) {
            grid->moveLeft();
            horizontalcounter = -7;
        }

        //left arrow key held down
        else if (IsKeyDown(KEY_LEFT)) {
            if (horizontalcounter == 3) {
                horizontalcounter = 0;
                grid->moveLeft();
            }
            else ++horizontalcounter;
        }

        //down arrow key to move down
        if (IsKeyPressed(KEY_DOWN)) {
            grid->moveDown();
            score->addScore(1);
            checkRows = true;
        }

        //down arrow key held down 
        else if (IsKeyDown(KEY_DOWN)) {
            if (downcounter == 3) {
                downcounter = 0;
                grid->moveDown();
                score->addScore(1);
                checkRows = true;
            }
            else ++downcounter;
        }

        //space bar to drop block
        else if (IsKeyPressed(KEY_SPACE)) {
            Sound drop = LoadSound("resources/audio/blockdropping.wav");
            PlaySound(drop);
            score->addScore(grid->drop()*2);
            checkRows = true;
        }

        //up arrow key to rotate block
        if (IsKeyPressed(KEY_UP)) grid->rotate();

        //C key to hold current block
        if (IsKeyPressed(KEY_C)) grid->hold();

        //pause menu
        if (IsKeyPressed(KEY_P)) {
            int p = pause();
            if (p == -1) break; //quit game
            else if (p == 0) {
                //quit to start
                start = true;
                continue;
            }
        }


        //----------------------------------------------------------------------------------
        // Final Checks: Row Completion and Game Over
        //----------------------------------------------------------------------------------

        
        //do we need to check if a row is complete?
        if (checkRows) {
            std::vector<int> rowsRemoved = grid->checkRowComplete();
            levelcounter = 0;
            //removes our grid rows and fixes, if -1 is returned then program needs to exit
            if (!rowsRemoved.empty()) {
                if (grid->removeRow(rowsRemoved) == -1 || grid->fixRows(rowsRemoved) == -1) {
                    CloseWindow();
                    return 0;
                }
            }
        }


        //checking if the game should be over
        if (grid->checkGameOver()) {
            start = true; bool quit = true;
            int endcounter = 0; double fade = 0;
            while (!WindowShouldClose()) {
                ++endcounter;
                fade +=0.01;
                if (endcounter == 450) {
                    quit = false;
                    break;
                }
                BeginDrawing();
                    ClearBackground((Color){static_cast<unsigned char>(0+level/2), 0, 0});
                    grid->drawAll(true, true);
                    //Fade in YOU LOSE text into center of screen with a black bar background, like in Dark Souls
                    DrawRectangle(0, 400-MeasureTextEx(titleFont, "YOU LOSE", 150, 5).y/2-10, 800, MeasureTextEx(titleFont, "YOU LOSE", 150, 5).y+20, Fade(BLACK, fade));
                    DrawTextEx(titleFont, "YOU LOSE", (Vector2){400-MeasureTextEx(titleFont, "YOU LOSE", 150, 5).x/2, 400-MeasureTextEx(titleFont, "YOU LOSE", 150, 5).y/2}, 150, 5, Fade(WHITE, fade));
                EndDrawing();
            }
            if (quit) break;
            else continue;
        }


        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------

            BeginDrawing();
                grid->drawAll(true, true);              //handles all drawing and camera related functions when game is active
            EndDrawing();

    }


        //----------------------------------------------------------------------------------
        // Deinitialization
        //----------------------------------------------------------------------------------


    if (grid != nullptr) {
        delete grid;
        delete score;
        delete tet;
        delete powerUp;
    }
    CloseWindow();
    CloseAudioDevice();
    return 0;
}

//----------------------------------------------------------------------------------
// Functions: menu(), pause(), usePower()
//----------------------------------------------------------------------------------

int menu() {
    Font titleFont = LoadFont("resources/titleFont.ttf");                       //loading font
    Rectangle playbutton {275, 400, 240, 100};                                  //start menu play button
    const std::vector<Color> colors = {SKYBLUE, DARKBLUE, ORANGE, 
                                        YELLOW, LIME, PURPLE, RED};             //colors for background cubes
    std::vector<std::pair<Rectangle, Color>> cubes;                             //vector that stores individual cubes each cube will
                                                                                //be removed when it goes out the bounds of screen
    int addCubeCounter = 0;                                                     //counter for how often we should spawn new cube
    int cubeSize = 20;                                                          //dimension of each cube
    bool fade = false;                                                          //bool to fade into Tet Monologue when play button is pressed
    double fadeTracker = 0;                                                     //for fade of screen


    while (!WindowShouldClose()) {

        BeginDrawing();
            ClearBackground(BLACK);

            //adding random cube with random position to the vector
            if (addCubeCounter == 30) {
                Rectangle c; c.x = GetRandomValue(0, 800-cubeSize); c.y = -cubeSize; c.width = cubeSize; c.height = cubeSize;
                cubes.push_back(std::make_pair(c, colors[GetRandomValue(0, 6)]));
                addCubeCounter = 0;
            }
            else addCubeCounter++;

            //drawing the cubes and removing ones that are out of the screen
            for (std::vector<std::pair<Rectangle, Color>>::iterator cube = cubes.begin(); cube != cubes.end(); cube++) {
                    cube->first.y+=1;
                DrawRectangleRec((*cube).first, (*cube).second);
                DrawRectangleLinesEx((*cube).first, 4, Fade(BLACK, 0.2));
            }
            if (cubes.size() != 0 && cubes[0].first.y > 800+cubeSize) {
                cubes.assign(cubes.begin()+1, cubes.end());
            }

            //drawing our title and play button
            DrawTextEx(titleFont, "MistaTet", (Vector2){400-MeasureTextEx(titleFont, "MistaTet", 150, 5).x/2, 200}, 150, 5, WHITE);
            DrawRectangleRounded(playbutton, 0.2, 100, LIGHTGRAY);
            DrawRectangleRoundedLines(playbutton, 0.2, 100, 10, GRAY);
            DrawTextEx(titleFont, "start", Vector2{playbutton.x+playbutton.width/2-MeasureTextEx(titleFont, "start", 60, 0).x/2, playbutton.y+playbutton.height/2-MeasureTextEx(titleFont, "start", 60, 0).y/2}, 60, 0, WHITE);

            //checking if mouse is over the play button and if play button is clicked
            float mX = GetMouseX(); float mY = GetMouseY();
            if (CheckCollisionPointRec((Vector2){mX, mY}, playbutton)) {
                DrawRectangleRounded(playbutton, 0.2, 100, Fade(WHITE, 0.3));
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) fade = true;
            }

            //once play button is hit then fade to black for tet monologue
            if (fade) {
                DrawRectangle(0, 0, 800, 800, Fade(BLACK, fadeTracker));
                fadeTracker+=0.01;
            }
            if (fadeTracker >= 1.5) return 0;
        EndDrawing();

    }
    return -1;                                  //quit program
}


int pause() {
    Texture2D tetHead = LoadTexture("resources/mistaTet4Forward.png");                      //tet's head in pause menu
    Font allFont = LoadFont("resources/allFont.ttf");                                       //loading font
    Rectangle src = (Rectangle){0, 0, 70, 70}, dest = (Rectangle) {400, 275, 70*6, 70*6};   //source and dest rectangle of tet head texture
    int bobCounter = 0, bobSpeed = 10, yvel = -1, startWait = 60, counter = 0, add = 1;     //variables for bobbing of the tet head
                                                                                            //much better implementation using velocity was done in the tet class but I'm too lazy to change this
    double fade = 0;                                                                        //for fading
    bool wait, quit = false;                                                                //wait for head bobbing and quit for returning to menu

    while (!WindowShouldClose()) {

        //for bobbing of giant Tet Head
        if (counter == 15) {
                src.x+=70*add;
            if (src.x == 280) add = -1;
            if (src.x == 0) add = 1;
            counter = 0;
        }
        if (wait) {
            if (bobCounter == 50) {
                bobCounter = 0;
                wait = false;
            }
        }
        else if (bobCounter == bobSpeed) {
            dest.y += yvel;
            if ((dest.y <= 255 && yvel < 0) || (dest.y >= 270 && yvel > 0)) bobSpeed++;
            else if (bobSpeed != 5) bobSpeed--;
            if (dest.y == 250) {
                wait = true;
                yvel = 1;
            }
            if (dest.y == 275) {
                yvel = -1;
                wait = true;
            }
            bobCounter = 0;
        }

        //checking user input
        if (startWait == 0) {                           //startWait is used to give a little delay before program recognizes user inputs
            if (IsKeyPressed(KEY_P)) return 1;          //continue game
            else if (IsKeyPressed(KEY_M)) quit = true;  //go to main menu
            else if (IsKeyDown(KEY_Q)) return -1;       //quit program
        }
        else --startWait;
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(tetHead, src, dest, (Vector2){35*6, 35*6}, 0, WHITE);
            DrawTextEx(allFont, "P to resume", (Vector2){400-MeasureTextEx(allFont, "P to resume", 25, 1).x/2, 500}, 25, 1, WHITE);
            DrawTextEx(allFont, "M to quit", (Vector2){400-MeasureTextEx(allFont, "M to quit", 25, 1).x/2, 550}, 25, 1, WHITE);
            DrawTextEx(allFont, "Q to exit game", (Vector2){400-MeasureTextEx(allFont, "Q to exit game", 25, 1).x/2, 600}, 25, 1, WHITE);
            if (quit) {
                DrawRectangle(0, 0, 800, 800, Fade(WHITE, fade));
                fade+=0.01;
                if (fade >= 1) return 0;
            }
        EndDrawing();

        ++counter;
        ++bobCounter;
    }
    return -1;
}


int usePower(PowerupItem* p, Score* scr, Grid* grid) {
    if (p->id == "multiplier") {                                    //multiplier powerup
        PlaySound(LoadSound("resources/audio/usepowerup.wav"));
        Multiplier* m = dynamic_cast<Multiplier*>(p);
        scr->addMultiplier(m->multiplier);
        delete m;
    }
    else if (p->id == "laser") {                                    //laser powerup
        delete p;
        return grid->lasers();
    }
    else if (p->id == "bomb") {                                     //bomb powerup
        delete p;
        return grid->bomb();
    }
    else if (p->id == "nuke") {                                     //nuke powerup
        delete p;
        return grid->nuke();
    }
    else if (p->id == "threeblock") {                               //change next three blocks power
        PlaySound(LoadSound("resources/audio/usepowerup.wav"));
        ThreeBlock* t = dynamic_cast<ThreeBlock*>(p);
        grid->changeNext(t->blockID);
        delete t;
    }   
    else if (p->id == "plusmultiplier") {                           //permanent multiplier power
        PlaySound(LoadSound("resources/audio/usepowerup.wav"));
        PlusMultiplier* m = dynamic_cast<PlusMultiplier*>(p);
        scr->addPermanentMultiplier(m->multiplier);
    }
    return 0;
}