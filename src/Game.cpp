//--------------------------------------------------------------------------
// Includes, constants, pre-definitions
//--------------------------------------------------------------------------

#include "raylib.h"
#include "grid.h"
#include "tools.h"
#include <cmath>

#ifndef M_E
#define M_E 2.718281828459045
#endif
#define screenWidth 800
#define screenHeight 800

int menu();
int pause();
int win();
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
    int rotatecounter = 0;                                  //cooldown for rotate
    int powerupcounter = 0;                                 //keeps track of when to spawn new powerup
    bool start = true;                                      //bool to bring us to the start menu
    int spawnpower = 60*5;                                  //how often we should spawn powerup
    int blockInitialWait = 0;                               //initial wait counter for when the block has just been generated

    //for powers
    std::vector<std::pair<double, int>> speedChange;        //for our speedchange powerup, double represents how changed and int is time left of effect
    std::pair<bool, int> slow = std::make_pair(false, 0);   //for input slowing, bool is if its active, int is time left
    int keysPressed = 0;                                    //need to press key 2 times for one movement
    std::pair<bool, int> delay = std::make_pair(false, 0);  //for input delay, bool is if active, int is time left
    std::deque<std::pair<std::string, int>> moveQueue;      //movement queue for when inputs are delayed

    //other
    Font titleFont = LoadFont("resources/title_font.ttf");  //title font
    int level = 0;                                          //the current level, determines how fast the blocks fall
    bool scenePlayed = false;                               //has tet cutscene already played?


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
            scenePlayed = false;

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
            MusicPlayer().play("resources/music/tet_stage_1.mp3");          //playing stage 1 music

        }


        //----------------------------------------------------------------------------------
        // Checking and calling updates from other classes
        //----------------------------------------------------------------------------------


        bool checkRows = false;                             //true if we should check if rows on the board are completed and need to be removed

        //updating functions
        grid->updateAll();

        //for delaying and slowing power
        if (delay.first) {
            --delay.second;
            if (delay.second == 0) delay.first = false;
        }

        if (slow.first) {
            --slow.second;
            if (slow.second == 0) slow.first = false;
        }
        for (std::deque<std::pair<std::string, int>>::iterator it = moveQueue.begin(); it != moveQueue.end(); it++) {
            --it->second;
            if (it->second == 0) {
                if (it->first == "right") grid->moveRight();
                else if (it->first == "left") grid->moveLeft();
                else if (it->first == "rotate") grid->rotate();
                else if (it->first == "down") {
                    grid->moveDown();
                    checkRows = true;
                }
                else if (it->first == "drop") {
                    grid->drop();
                    checkRows = true;
                }
                moveQueue.erase(it);
                --it;
            }
        }

        //checking if we should go to tet cutscene
        if (!scenePlayed && score->getScore() >= 90000) {
            if (tet->tetCutscene() == -1) break;
            scenePlayed = true;
            if (grid->finalStage() == -1) break;
        }
        //player wins
        else if (score->getScore() >= 100000) {
            bool quit = true; start = true;
            int endcounter = 0; double fade = 0;
            sound().reset();
            MusicPlayer().play("resources/music/victory_song.mp3");
            while (!WindowShouldClose()) {
                MusicPlayer().updateMusic();
                fade +=0.007;
                if (fade > 4 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    quit = false;
                    break;
                }
                BeginDrawing();
                    ClearBackground((Color){static_cast<unsigned char>(0+level/2), 0, 0});
                    if (fade < 1) {
                        grid->drawAll(true, true);
                        sound().reset();
                    }
                    //White Screen fade in and text that says you have defeated tet and how long it took player
                    DrawRectangle(0, 0, 800, 800, Fade(WHITE, fade));
                    std::string txt = "DEFEATED TET IN";
                    DrawTextPro(titleFont, txt.c_str(), {400, 300}, {MeasureTextEx(titleFont, txt.c_str(), 50, 0).x/2, MeasureTextEx(titleFont, txt.c_str(), 50, 0).y/2}, 0, 50, 0, Fade(RED, fade));
                    txt = grid->getFinalTime();
                    DrawTextPro(titleFont, txt.c_str(), {400, 400}, {MeasureTextEx(titleFont, txt.c_str(), 60, 4).x/2, MeasureTextEx(titleFont, txt.c_str(), 60, 4).y/2}, 0, 60, 4, Fade(RED, fade-1.5));
                    txt = "thank you";
                    DrawTextPro(titleFont, txt.c_str(), {400, 500}, {MeasureTextEx(titleFont, txt.c_str(), 30, 0).x/2, MeasureTextEx(titleFont, txt.c_str(), 30, 0).y/2}, 0, 30, 0, Fade(RED, fade-3));
                    txt = "click to proceed";
                    DrawTextPro(titleFont, txt.c_str(), {725, 775}, {MeasureTextEx(titleFont, txt.c_str(), 10, 0).x/2, MeasureTextEx(titleFont, txt.c_str(), 10, 0).y/2}, 0, 10, 0, Fade(RED, fade));
                EndDrawing();
            }
            if (quit) break;
            else continue;
        }

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

        //speed cap
        if (num > 0 && num < 5) num = 5;

        //give a slight initial delay when the block has just spawned
        if (grid->blockSpawned()) { 
            blockInitialWait = 15;
        }

        if (blockInitialWait != 0) {
            blockInitialWait--;
        }
        else {
            //check if we should move block down
            if (!(num < 0) && levelcounter >= num) {
                levelcounter = 0;
                grid->moveDown();
                checkRows = true;
            }
            levelcounter++;
        }


        //----------------------------------------------------------------------------------
        // Powerups from Tet and Player
        //----------------------------------------------------------------------------------


        //check if we should spawn a new powerup
        if (powerupcounter == spawnpower) {
            powerUp->spawnPowerup(true);
            powerupcounter = 0;
            if (score->getScore() < 25000) {
                spawnpower = GetRandomValue(15*60, 24*60);
            }
            else if (score->getScore() < 50000) {
                spawnpower = GetRandomValue(11*60, 20*60);
            }
            else if (score->getScore() < 75000) {
                spawnpower = GetRandomValue(9*60, 16*60);
            }
            else if (score->getScore() < 90000) {
                spawnpower = GetRandomValue(8*60, 13*60);
            }
            else if (score->getScore() < 100000) {
                spawnpower = GetRandomValue(6*60, 10*60);
            }
        }
        powerupcounter++;

        //checking if tet used a new power and acting accordingly
        std::vector<std::string> powers = tet->checkTetPower();
        for (std::string power: powers) {
            if (power == "level1") grid->increaseLevel(15);
            else if (power == "level2") grid->increaseLevel(25);
            else if (power == "blind1") grid->blind(GetRandomValue(5, 7));
            else if (power == "blind2") grid->blind(GetRandomValue(8, 12));
            else if (power == "zblock") grid->changeNext(5);
            else if (power == "sblock") grid->changeNext(7);
            else if (power == "turn") grid->randomRotate();
            else if (power == "inputslow") slow = std::make_pair(true, GetRandomValue(60*20, 60*35));
            else if (power == "inputdelay") delay = std::make_pair(true, GetRandomValue(60*20, 60*35)); 
            else if (power == "slowanddelay") {
                int rand = GetRandomValue(60*20, 60*35);
                slow = std::make_pair(true, rand);
                delay = std::make_pair(true, rand); 
            }
            else if (power == "flurry") {
                grid->extremeRandomRotate();
                score->addMultiplier(-1);
                score->halfMultiplier();
            }
        }

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
            sound().play("resources/audio/slow_motion.mp3");
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
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            if (slow.first) {
                if (keysPressed >= 1) {
                    if (delay.first) {
                        moveQueue.push_back(std::make_pair("right", 20));
                    }
                    else grid->moveRight();
                    keysPressed = 0;
                }
                else ++keysPressed;
            }
            else if (delay.first) {
                moveQueue.push_back(std::make_pair("right", 20));
            }
            else grid->moveRight();
            horizontalcounter = slow.first? -14: -7;
        }

        //right arrow key held down
        else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            if (horizontalcounter >= (slow.first ? 20: 3)) {
                if (delay.first) moveQueue.push_back(std::make_pair("right", 20));
                else grid->moveRight();
                horizontalcounter = 0;
            }
            else ++horizontalcounter;
        }

        //left arrow key to move left
        else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            if (slow.first) {
                if (keysPressed >= 1) {
                    if (delay.first) moveQueue.push_back(std::make_pair("left", 20));
                    else grid->moveLeft();
                    keysPressed = 0;
                }
                else ++keysPressed;
            }
            else if (delay.first) {
                moveQueue.push_back(std::make_pair("left", 20));
            }
            else grid->moveLeft();
            horizontalcounter = slow.first? -14: -7;
        }

        //left arrow key held down
        else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
             if (horizontalcounter >= (slow.first ? 20: 3)) {
                if (delay.first) moveQueue.push_back(std::make_pair("left", 20));
                else grid->moveLeft();
                horizontalcounter = 0;
            }
            else ++horizontalcounter;
        }

        //down arrow key to move down
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            if (slow.first) {
                if (keysPressed >= 1) {
                    if (delay.first) moveQueue.push_back(std::make_pair("down", 20));
                    else {
                        grid->moveDown();
                        checkRows = true;
                    }
                    keysPressed = 0;
                }
                else ++keysPressed;
            }
            else if (delay.first) {
                moveQueue.push_back(std::make_pair("down", 20));
            }
            else {
                grid->moveDown();
                checkRows = true;
            }
            horizontalcounter = slow.first? -14: -7;
        }

        //down arrow key held down 
        else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            if (downcounter >= (slow.first ? 20: 3)) {
                downcounter = 0;
                if (delay.first) moveQueue.push_back(std::make_pair("down", 20));
                else {
                    grid->moveDown();
                    checkRows = true;
                }
            }
            else ++downcounter;
        }

        //space bar to drop block
        else if (IsKeyPressed(KEY_SPACE)) {
            if (delay.first) {
                moveQueue.push_back(std::make_pair("drop", 20));
            }
            else {
                grid->drop();
                checkRows = true;
            }
        }

        //up arrow key to rotate block
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            if (slow.first) {
                if (keysPressed >= 1) {
                    keysPressed = 0;
                    if (delay.first) {
                        moveQueue.push_back(std::make_pair("rotate", 20));
                    }
                    else grid->rotate();
                }
                else ++keysPressed;
            }
            else if (delay.first) {
                moveQueue.push_back(std::make_pair("rotate", 20));
            }
            else grid->rotate();
            rotatecounter = slow.first? -14: -7;
        }

        else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            if (rotatecounter >= (slow.first ? 30: 6)) {
                rotatecounter = 0;
                if (delay.first) moveQueue.push_back(std::make_pair("rotate", 20));
                else {
                    grid->rotate();
                }
            }
            else ++rotatecounter;
        }

        //C key to hold current block
        if (IsKeyPressed(KEY_LEFT_SHIFT)) grid->hold();

        //pause menu
        if (IsKeyPressed(KEY_P)) {
            int p = pause();
            if (p == -1) break; //quit game
            else if (p == 0) {
                //quit to start
                start = true;
                continue;
            }
            MusicPlayer().resume();
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
                if (grid->removeRow(rowsRemoved) == -1 || grid->fixRows(rowsRemoved, false, true) == -1) {
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
                grid->updateAll();
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
                    std::string txt = "YOU LOSE";
                    DrawRectanglePro({400, 400, 800, MeasureTextEx(titleFont, txt.c_str(), 100, 0).y+20}, {400, (MeasureTextEx(titleFont, txt.c_str(), 100, 0).y+20)/2}, 0, Fade(BLACK, fade));
                    DrawTextPro(titleFont, txt.c_str(), {400, 400}, {MeasureTextEx(titleFont, txt.c_str(), 100, 0).x/2, MeasureTextEx(titleFont, txt.c_str(), 100, 0).y/2}, 0, 100, 0, Fade(WHITE, fade));
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
// Functions: menu(), pause(), usePower(), win()
//----------------------------------------------------------------------------------

int menu() {
    Font titleFont = LoadFont("resources/title_font.ttf");                      //loading font
    Rectangle playbutton {275, 500, 240, 100};                                  //start menu play button
    const std::vector<Color> colors = {SKYBLUE, DARKBLUE, ORANGE, 
                                        YELLOW, LIME, PURPLE, RED};             //colors for background cubes
    std::vector<std::pair<Rectangle, Color>> cubes;                             //vector that stores individual cubes each cube will
                                                                                //be removed when it goes out the bounds of screen
    int addCubeCounter = 0;                                                     //counter for how often we should spawn new cube
    int cubeSize = 20;                                                          //dimension of each cube
    bool fade = false;                                                          //bool to fade into Tet Monologue when play button is pressed
    double fadeTracker = 0;                                                     //for fade of screen
    bool mouseOn = false;                                                       //if mouse has been on the button

    MusicPlayer().play("resources/music/tet_theme.mp3");                        //playing tet theme song


    while (!WindowShouldClose()) {

        MusicPlayer().updateMusic();            //updating the music player
        sound().updateSound();

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
            DrawTextEx(titleFont, "MISTA TET", (Vector2){400-MeasureTextEx(titleFont, "MISTA TET", 100, 0).x/2, 200}, 100, 0, WHITE);
            DrawRectangleRounded(playbutton, 0.2, 100, LIGHTGRAY);
            DrawRectangleRoundedLines(playbutton, 0.2, 100, 10, GRAY);
            DrawTextEx(titleFont, "START", Vector2{playbutton.x+playbutton.width/2-MeasureTextEx(titleFont, "START", 40, 0).x/2, playbutton.y+playbutton.height/2-MeasureTextEx(titleFont, "START", 40, 0).y/2}, 40, 0, WHITE);

            //checking if mouse is over the play button and if play button is clicked
            float mX = GetMouseX(); float mY = GetMouseY();
            if (CheckCollisionPointRec((Vector2){mX, mY}, playbutton)) {
                if (!mouseOn) {
                    sound().play("resources/audio/button_down.mp3");
                    mouseOn = true;
                }
                DrawRectangleRounded(playbutton, 0.2, 100, Fade(WHITE, 0.3));
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) fade = true;
            }
            else {
                if (mouseOn) sound().play("resources/audio/button_up.mp3");
                mouseOn = false;
            }

            //once play button is hit then fade to black for tet monologue
            if (fade) {
                DrawRectangle(0, 0, 800, 800, Fade(BLACK, fadeTracker));
                fadeTracker+=0.01;
            }
            if (fadeTracker >= 1.5) {
                UnloadFont(titleFont);
                return 0;
            }
        EndDrawing();

    }
    return -1;                                  //quit program
}


int pause() {
    Texture2D tetHead = LoadTexture("resources/tet/tet_4_forward.png");                     //tet's head in pause menu
    Font allFont = LoadFont("resources/all_font.ttf");                                      //loading font
    Rectangle src = (Rectangle){0, 0, 70, 70}, dest = (Rectangle) {400, 275, 70*6, 70*6};   //source and dest rectangle of tet head texture
    int bobCounter = 0, bobSpeed = 10, yvel = -1, startWait = 60, counter = 0, add = 1;     //variables for bobbing of the tet head
                                                                                            //much better implementation using velocity was done in the tet class but I'm too lazy to change this
    double fade = 0;                                                                        //for fading
    bool wait, quit = false;                                                                //wait for head bobbing and quit for returning to menu

    MusicPlayer().pause();                                                                  //loading pause menu music

    while (!WindowShouldClose()) {
        
        MusicPlayer().updatePauseMusic();               //updating music stream

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
            if (IsKeyPressed(KEY_P)) {                  //continue game
                UnloadTexture(tetHead);
                UnloadFont(allFont);
                return 1;    
            }
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
            }
        EndDrawing();

        if (fade >= 1) {
            UnloadTexture(tetHead);
            UnloadFont(allFont);
            return 0;
        }

        ++counter;
        ++bobCounter;
    }
    return -1;
}


int usePower(PowerupItem* p, Score* scr, Grid* grid) {
    if (p->id == "multiplier") {                                    //multiplier powerup
        sound().play("resources/audio/use_powerup.mp3");
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
        sound().play("resources/audio/use_powerup.mp3");
        ThreeBlock* t = dynamic_cast<ThreeBlock*>(p);
        grid->changeNext(t->blockID);
        delete t;
    }   
    else if (p->id == "plusmultiplier") {                           //permanent multiplier power
        sound().play("resources/audio/use_powerup.mp3");
        PlusMultiplier* m = dynamic_cast<PlusMultiplier*>(p);
        scr->addPermanentMultiplier(m->multiplier);
    }
    return 0;
}
