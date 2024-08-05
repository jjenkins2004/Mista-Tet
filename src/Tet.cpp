#include "tet.h"
#include <iostream>
#include <fstream>

using namespace std;

Tet::Tet(Score* s): scr(s) {
     for (int i = 0; i < 20; i++) {
        vals.push_back(i);
     }
}

Tet::~Tet() {
    UnloadTexture(t);
    UnloadTexture(babyHead);
    UnloadTexture(redBabyHead);
    UnloadFont(tetFont);
}

void Tet::drawTet() {
    
    //tetbobbing
    if (!flying) tetBob();          //don't want tet to bob when he is in this powerup

    //update tet dialogue
    updateDialogue();

    //tet power drawing
    //these functions will do nothing if powerup is not active but will update if it is active
    babyPower();
    flyPower();

    //tet and what he is saying
    DrawTexturePro(t, source, dest, (Vector2){scale*tdim/2, scale*tdim/2}, rotation,  Fade(WHITE, tetFade));
}

void Tet::updateDialogue() {
    //wait time after entire dialogue comes out
    if (wait) {
        if (time == waitTime) {
            time = 0;
            wait = false;
            tetText = "";
        }
        time++;
        SetTextLineSpacing(20);
        drawTetText(tetText);
    }
    //checks if tet should generate and display a new dialogue
    else if (time >= timebetweenText) {
        //adds a singular character to what is displayed and waits for a couple frames before adding another character 
        if (txtCounter == txtCounterWait) {
            spaceWait = false;
            if (!stop) {
                //from which bag and which index should we add another character or use power
                if (tetpowertoggle) {                           //tet power dialogues
                    //score is >90,000, final stage powers
                    if (finalStage) {
                        tetText+= finalPowers[txtIndex].dialogue.substr(txtTracker, 1);
                        if (txtTracker == finalPowers[txtIndex].dialogue.length()-1) {
                            tetpowertoggle = false;
                            stop = true;
                            usePower(finalPowers[txtIndex]);
                            if (finalStart) finalStart = false;
                        }
                    }
                    //score is below 50,000, stages 1 and 2
                    else if (tetStage <=2) {
                        tetText+= tetPowers1[txtIndex].dialogue.substr(txtTracker, 1);
                        if (txtTracker == tetPowers1[txtIndex].dialogue.length()-1) {
                            tetpowertoggle = false;
                            stop = true;
                            usePower(tetPowers1[txtIndex]);
                        }
                    }
                    //score is between 50,000 and 90,000, stages 3 and 4
                    else {
                        tetText += tetPowers2[txtIndex].dialogue.substr(txtTracker, 1);
                        if (txtTracker == tetPowers2[txtIndex].dialogue.length()-1) {
                            tetpowertoggle = false;
                            stop = true;
                            usePower(tetPowers2[txtIndex]);
                        }
                    }
                }
                //dialogue is not a tet power so choose from noEffectText
                else {
                    tetText+= noEffectText[txtIndex].substr(txtTracker, 1);
                    if (txtTracker == noEffectText[txtIndex].length()-1) stop = true;
                }

                //determine if the last letter we just added was a punctuation mark to add an extra delay
                string lastletter = tetText.substr(tetText.length()-1, 1);
                if (lastletter == "." || lastletter == "?" || lastletter == "!") {
                    //make sure tet mouth during punctuation letters
                    if (tetStage <= 2) {
                        spaceWait = true;
                        if (facePhase == 1) source.x-=tdim;
                        tetCounter = 0; tetCounterMax = GetRandomValue(3, 7); facePhase = 0;
                    }
                    txtCounterWait = 15;
                }
                //last letter was not a punctuation so we can see if we should make a tet talking sound
                else {
                    //play a special sound for the inital final stage dialogue
                    if (!finalStart) {
                        //every third letter that comes out which is not a punctuation, play a random tet sound
                        if (talk == 2) {
                            //choosing the random sound
                            std::vector<std::string>::iterator it = tetSounds.begin() + GetRandomValue(0, tetSounds.size()-1);
                            sound().play(*it);

                            //erasing sound from bag so we don't repeat same ones in a row
                            tetSounds.erase(it);
                            if (tetSounds.size() == 3) {                //restoring bag if size is low
                                tetSounds = tetSoundsOriginal;
                            }
                            talk = 0;
                        }
                        else talk++;
                    }
                    txtCounterWait = 3;                                 //wait this many frames before adding a new letter
                }
                txtTracker++;                                           //letter as been added so moving on to next letter
            }
            //dialogue is finished so we should wait some time with full dialogue displaeyd
            else {
                talk = 2;
                time = 0;
                txtTracker = 0;
                wait = true;
                waitTime = 300 + tetText.size()*3;
                timebetweenText = GetRandomValue(5*60, 15*60); //how long should tet wait between dialogues
            }
            txtCounter = 0;
        }
        else ++txtCounter;
        SetTextLineSpacing(20);
        drawTetText(tetText);
        tetTalk(spaceWait);                 //spaceWait just tells tetTalk if we are currently at a punctuation mark and tet mouth should not open at this time
    }
    else {
        ++time;
        //determining whether this should be a tet power and generating a new dialogue
        if (time == timebetweenText) {
            stop = false;
            //choosing which text we should put on screen
            if (vals.size() == 0) for (int i = 0; i < 20; i++) vals.push_back(i);
            int num = 4;
            if (tetStage >= 3) num = 7;                             //later stages of the game make tet powers happen more often
            if (finalStage) num = 10;
            if (GetRandomValue(1, 10) <= num) {                     //determines whether the next dialogue from tet should be a tet power
                tetpowertoggle = true;
                if (finalStage) {
                    if (finalStart) {
                        //initial final stage flurry of powers
                        sound().play("resources/audio/tet_scream.mp3");
                        txtIndex = 0;
                    }
                    else {
                        txtIndex = GetRandomValue(0, 9);                    //final stage powers
                    }
                }
                else if (tetStage <=2) txtIndex = GetRandomValue(0, 7);     //choosing power from our early stage bag
                else txtIndex = GetRandomValue(0, 8);                       //choosing power from our late stage bag
            }
            else {
                std::vector<int>::iterator it = vals.begin()+GetRandomValue(0, vals.size()-1);
                txtIndex = *it;
                vals.erase(it);
            }
        }
    }
}

void Tet::babyPower() {
    if (get<0>(babies) != 0) {
        Texture2D* babyT;
        if (finalStage) babyT = &redBabyHead;
        else babyT = &babyHead;
        if (positions.empty()) {                                                                                    //getting random coordinates for the heads initially
            for (int i = 0; i < get<0>(babies); i++) {
                positions.push_back(Vector2{float(GetRandomValue(20, 780)), float(GetRandomValue(20, 780))});
            }
        }
        for (std::vector<Vector2>::iterator it = positions.begin(); it != positions.end(); it++) {                  //drawing each baby head and checking if player clicked it
            DrawTexturePro(*babyT, {0, 0, 50, 50}, {it->x, it->y, 150, 150}, {75, 75}, 0, WHITE);
            if (CheckCollisionPointCircle(getAdjustedCoordinates(GetMousePosition(), camRotation), *it, 75) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {       //checking if player hit the baby
                if(finalStage) sound().play("resources/audio/baby_sound_final.mp3");
                else sound().play("resources/audio/baby_sound.mp3");
                positions.erase(it);
                it--;
            }
        }
        if (positions.empty()) {                                                                                    //player has clicked all the heads, reward them
            scr->addScoreIgnoreNeg(300);
            babies = make_tuple(0, 0, 0);                                                                           //shows to not draw babies
        }
        else {
            if (--get<2>(babies) == 0) {
                scr->addScoreIgnoreNeg(get<1>(babies)*-1);
                babies = make_tuple(0, 0, 0);
            }
        }
    }
}

void Tet::flyPower() {

    //this entire power is time based and flytime is just used to keep track of the time and when to do a certain action

    if (!flying) return;

    if (flytime == 0) oldDest = dest;
    //fade tet out
    else if (flytime > 30 && flytime <= 90) {
        if (tetFade > 0) tetFade-=0.1;
        else tetFade = 0;
    }
    //fade tet in and teleport tet to the bottom right of the grid
    else if (flytime > 90 && flytime <= 130) {
        if (flytime == 91) {
            scale = 2;
            dest = {575, 575, tdim*scale, tdim*scale};
            look({dest.x-1, dest.y+0.3f});
        }
        if (tetFade < 1) tetFade+=0.1;
        else tetFade = 1;
    }
    //first fly path
    else if (flytime > 130 && flytime <= 136) {
        if (flytime == 131) {
            sound().play("resources/audio/tet_fly_boom.mp3");
            flyShake = true;
            tetVel = {-60, 15};
        }
        else if (flytime == 132 || flytime == 133) {
            tetVel.x-=10;
            tetVel.y+=10;

            //determining which grids were destroyed and subtracting score
            std::vector<Vector2> grids;
            int scoreCount = 0;
            if (flytime == 132) grids = {{17, 7}, {17, 8}, {17, 9}, {18, 7}, {18, 8}, {18, 9}};
            else grids = {{18, 4}, {18, 5}, {18, 6}, {19, 4}, {19, 5}, {19, 6}, {19, 7}};
            for (Vector2 g: grids) {
                if (grid[int(g.x)][int(g.y)] != 0) {
                    grid[int(g.x)][int(g.y)] = 0;
                    ++scoreCount;
                }
            }
            if (scoreCount != 0) scr->addScoreIgnoreNeg(-5*scoreCount);
        }
        else if (flytime == 134 || flytime == 135) {
            tetVel.y-=20;
            std::vector<Vector2> grids;
            int scoreCount = 0;
            if (flytime == 134) grids = {{19, 3}, {19, 4}, {20, 2}, {20, 3}, {20, 4}, {20, 5}};
            else grids = {{19, 0}, {19, 1}, {20, 0}, {20, 1}};
            for (Vector2 g: grids) {
                if (grid[int(g.x)][int(g.y)] != 0) {
                    grid[int(g.x)][int(g.y)] = 0;
                    ++scoreCount;
                }
            }
            if (scoreCount != 0) scr->addScoreIgnoreNeg(-5*scoreCount);
        }
        look({dest.x+tetVel.x, dest.y+tetVel.y});
        tetFade-=0.03;
        dest.x+=tetVel.x;
        dest.y+=tetVel.y;
    }
    //fade out first path tet and simultaneously fade in tet for second path
    else if (flytime > 136 && flytime <= 170) {
        if (tetFade > 0) tetFade-=0.03;
        else tetFade = 0;
        if (flytime >= 150 && tetFade2 < 1) tetFade2+=0.1;
        if (flytime == 170) {
            dest.x = 575;
            dest.y = 575;
            look({dest.x-1, dest.y-0.3f});
            tetFade = 1;
        }
        DrawTexturePro(t, source, {575, 575, tdim*scale, tdim*scale}, {scale*tdim/2, scale*tdim/2}, 10, Fade(WHITE, tetFade2));
    }
    //second path
    else if (flytime > 170 && flytime <= 176) {
        if (flytime == 171) {
            sound().play("resources/audio/tet_fly_boom.mp3");
            flyShake = true;
            tetVel = {-60, -15};
        }
        else if (flytime == 172 || flytime == 173) {
            tetVel.x-=10;
            tetVel.y-=10;
            std::vector<Vector2> grids;
            int scoreCount = 0;
            if (flytime == 172) grids = {{16, 9}, {16, 8}, {15, 9}, {15, 8}, {15, 7}};
            else grids = {{15, 6}, {14, 7}, {14, 6}, {14, 5}, {13, 5}};
            for (Vector2 g: grids) {
                if (grid[int(g.x)][int(g.y)] != 0) {
                    grid[int(g.x)][int(g.y)] = 0;
                    ++scoreCount;
                }
            }
            if (scoreCount != 0) scr->addScoreIgnoreNeg(-5*scoreCount);
            
        }
        else if (flytime == 174 || flytime == 175) {
            tetVel.y+=20;
            std::vector<Vector2> grids;
            int scoreCount = 0;
            if (flytime == 174) grids = {{14, 2}, {14, 3}, {14, 4}, {13, 2}, {13, 3}, {13, 4}};
            else grids = {{14, 0}, {14, 1}, {13, 0}, {13, 1}};
            for (Vector2 g: grids) {
                if (grid[int(g.x)][int(g.y)] != 0) {
                    grid[int(g.x)][int(g.y)] = 0;
                    ++scoreCount;
                }
            }
            if (scoreCount != 0) scr->addScoreIgnoreNeg(-5*scoreCount);
        }
        look({dest.x+tetVel.x, dest.y+tetVel.y});
        tetFade-=0.03;
        dest.x+=tetVel.x;
        dest.y+=tetVel.y;
    }
    //fade out 2nd path tet and fade in new tet for 3rd path
    else if (flytime > 176 && flytime <= 210) {
        if (tetFade > 0) tetFade-=0.03;
        else tetFade = 0;
        if (flytime >= 190 && tetFade2 < 1) tetFade2+=0.1;
        if (flytime == 210) {
            dest.x = 575;
            dest.y = 500;
            look({dest.x-1, dest.y-0.3f});
            tetFade = 1;
        }
        DrawTexturePro(t, source, {575, 500, tdim*scale, tdim*scale}, {scale*tdim/2, scale*tdim/2}, 10, Fade(WHITE, tetFade2));
    }
    //3rd path tet
    else if (flytime > 210 && flytime <= 216) {
        if (flytime == 211) {
            sound().play("resources/audio/tet_fly_boom.mp3");
            flyShake = true;
            tetVel = {-60, -40};
        }
        else if (flytime == 212 || flytime == 213) {
            tetVel.x-=10;
            tetVel.y-=10;
            std::vector<Vector2> grids;
            int scoreCount = 0;
            if (flytime == 212) grids = {{13, 9}, {12, 9}, {12, 8}, {11, 9}, {11, 8}, {11, 7}, {10, 8}};
            else grids = {{11, 6}, {10, 7}, {10, 6}, {9, 6}, {9, 5}, {8, 5}};
            for (Vector2 g: grids) {
                if (grid[int(g.x)][int(g.y)] != 0) {
                    grid[int(g.x)][int(g.y)] = 0;
                    ++scoreCount;
                }
            }
            if (scoreCount != 0) scr->addScoreIgnoreNeg(-5*scoreCount);

        }
        else if (flytime == 214 || flytime == 215) {
            tetVel.y-=5;
            std::vector<Vector2> grids;
            int scoreCount = 0;
            if (flytime == 214) grids = {{9, 4}, {8, 4}, {8, 3}, {7, 4}, {7, 3}, {7, 2}, {6, 3}, {6, 2}};
            else grids = {{7, 1}, {6, 1}, {5, 1}, {5, 0}, {4, 0}};
            for (Vector2 g: grids) {
                if (grid[int(g.x)][int(g.y)] != 0) {
                    grid[int(g.x)][int(g.y)] = 0;
                    ++scoreCount;
                }
            }
            if (scoreCount != 0) scr->addScoreIgnoreNeg(-5*scoreCount);
        }
        look({dest.x+tetVel.x, dest.y+tetVel.y});
        tetFade-=0.03;
        dest.x+=tetVel.x;
        dest.y+=tetVel.y;
    }
    else if (flytime > 210 && flytime <= 260) {
        if (tetFade > 0) tetFade-=0.03;
        else  {
            tetFade = 0;
        }
    }
    //bring tet to original position
    else if (flytime > 260) {
        if (flytime == 261) {
            dest = oldDest;
            scale = 3.5;
            look({-1, -1});
        }
        if (tetFade < 1) tetFade+=0.1;
        else {
            flying = false;
            flytime = 0;
            tetFade2 = 0;
        }
    }
    ++flytime;
}

void Tet::look(Vector2 coor) {
    if (coor.x < 0) {                                                   //reset rotation back to zero and make tet look forward
        rotation = 0;
        if (tetStage == 1 || tetStage == 2) source.x = tdim*facePhase;
        else if (tetStage == 3) {
            UnloadTexture(t);
            t = LoadTexture("resources/tet/tet_3_forward.png");
        }
        else if (tetStage == 4) {
            UnloadTexture(t);
            t = LoadTexture("resources/tet/tet_4_forward.png");
        }
        return;
    }
    if (abs(dest.x-coor.x) < 0.001) {                                   //just in case somehow line is vertical, but this should never occur
        return;
    }
    Vector2 slope = {-1, -1*(dest.y-coor.y)/(dest.x-coor.x)};           //find the vector from center of tet to coor
    float den = sqrt(pow(slope.x, 2) + pow(slope.y, 2));                //denominator for the angle formula using two vectors
    float theta = 180*acos((slope.x*-1)/den)/PI;                        //determine the angle between (-1, 0) which is gaze direction with 0 rotation with the slope vector
    rotation = slope.y < 0 ? theta: theta*-1;                           //determine if we should rotate clockwise or counterclockwise

    //changing texture to side profile
    if ((tetStage == 1 || tetStage == 2) && source.x < tdim*2) source.x+=tdim*2;
    else if (tetStage == 3) {
        UnloadTexture(t);
        t = LoadTexture("resources/tet/tet_3_left.png");
    }
    else if (tetStage == 4) {
        UnloadTexture(t);
        t = LoadTexture("resources/tet/tet_4_left.png");
    }

}

int Tet::tetMonologue() {
    //defining variables
    Rectangle tetSource = (Rectangle) {0, 0, tdim, tdim};
    float scale = 6;
    Rectangle tetDest = (Rectangle) {400, 350, tdim*scale, tdim*scale};
    Vector2 tetOrigin = (Vector2){scale*tdim/2, scale*tdim/2};
    float tBoxWidth = 700, tBoxHeight = 125; Color textBoxColor = (Color) {45, 45, 45, 255};
    Rectangle textBox = (Rectangle) {400-tBoxWidth/2, 700-tBoxHeight/2, tBoxWidth, tBoxHeight};
    Color textColor = (Color){30, 0, 0, 255}; SetTextLineSpacing(25);
    int txtcounter = 0, txtWait = 2, bobCount = 0, soundCount = 0, talkCount = 0, talkCountMax = GetRandomValue(8, 12);
    bool wait = false, checkEnd = false, start = true, end = false, waitBob = false;
    float fade = 0, tetVel = -0.01;

    //opening monologue file
    ifstream mono("resources/tet/tetMonologue.txt");
    string txt;
    char ch;

    //loop for Tet's monologue
    while(!WindowShouldClose()) {
        if (!wait) {
            if (txtcounter == txtWait) {
                if (txtWait == 15) txtWait = 2;
                mono.get(ch);

                //checking characters
                if (ch == '&') {                            //& character marks the end of the text and end of cutscene
                    checkEnd = true;
                    wait = true;
                } 
                else if (ch == '$') {                       //$ marks the end of this section
                    wait = true;
                    mono.get(ch);                           //removing the next line character
                }
                else txt+=ch;

                if (ch == '.' || ch == '?' || ch == '!') {  //pausing for end of sentences
                    txtWait = 15;
                    soundCount = 0;                         //make sure sound doesn't play when there is punctuation
                }

                txtcounter = 0;

                //playing tet talking sounds
                if (!wait && ch != ' ' && txtWait != 15) {  //sound doesn't play when there is a space or if there is punctuation, or if we need to wait
                    if (soundCount == 2) {                  //play sound, wait two letters, play another sound
                        //grabbing sound from our pool
                        std::vector<std::string>::iterator it = tetSounds.begin() + GetRandomValue(0, tetSounds.size()-1);
                        sound().play(*it);
                        tetSounds.erase(it);
                        if (tetSounds.size() == 3) {        //resetting the pool if it gets too small
                            tetSounds = tetSoundsOriginal;
                        }
                        soundCount = 0;
                    }
                    else soundCount++;
                }          
            }
            else ++txtcounter;

            //mouth movements
                if (talkCount == talkCountMax) {
                    if (tetSource.x == 0) {
                        tetSource.x+=tdim;
                        talkCountMax = GetRandomValue(8, 12);
                    }
                    else {
                        tetSource.x-=tdim;
                        talkCountMax = GetRandomValue(3, 7);
                    }
                    talkCount = 0;
                }
                else talkCount++;
                //closing mouth at punctuation or when we are waiting
                if (txtWait == 15 || wait) {
                    tetSource.x = 0;
                }
            
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!checkEnd) {
                txt = "";
                txtcounter = 0;
                wait = false;
            }
            else end = true;
        }
        if (IsKeyPressed(KEY_S)) end = true;

        //for tet bobbing up and down constantly
        double maxBobSpeed = 0.5;                       //defining the maximum speed which tet will accelerate to when bobbing up and down
        if (waitBob) {                                  //waiting at the top and bottom of the bob
                if (bobCount == 50) {
                    bobCount = 0;
                    waitBob = false;
                }
                else {
                    bobCount++;
                }
        }
        else if (scale == 6) {
            tetDest.y += tetVel;
            if (tetDest.y <= 300  && tetVel < 0) {              //decelerating when passing this point
                tetVel+=maxBobSpeed/50;
                if (tetVel > -maxBobSpeed/100) {
                    tetVel = maxBobSpeed/50;
                    waitBob = true;
                }
            }
            else if (tetDest.y >= 337 && tetVel > 0) {          //decelerating when passing this point
                tetVel-=maxBobSpeed/50;
                if (tetVel < maxBobSpeed/100) {
                    tetDest.y = 350;
                    tetVel = -maxBobSpeed/50;
                    waitBob = true;
                }
            }
            else if (tetVel < 0 && tetVel >-maxBobSpeed) {
                tetVel-=maxBobSpeed/50;
            }
            else if (tetVel > 0 && tetVel < maxBobSpeed) {
                tetVel+=maxBobSpeed/50;
            }
        }

        //for tet glowing eye animation
        if (tetStage >= 3) {
            if (tetVel > 0) {
                if (tetDest.y < 286.8) {            //look 5
                    tetSource.x = tdim*4;
                }
                else if (tetDest.y < 304.1) {       //look 4
                    tetSource.x = tdim*3;
                }
                else if (tetDest.y < 333.6) {       //look 3
                    tetSource.x = tdim*2;
                }
                else if (tetDest.y < 350) {         //look 2
                    tetSource.x = tdim;
                }
                else {                              //look 1
                    tetSource.x = 0;
                }
            }
            else {
                if (tetDest.y >= 350 ) {            //look 1
                    tetSource.x = 0;
                }
                else if (tetDest.y >= 333.6) {      //look 2
                    tetSource.x = tdim;
                }
                else if (tetDest.y >= 304.1) {      //look 3
                    tetSource.x = tdim*2;
                }
                else if (tetDest.y >= 286.769) {    //look 4
                    tetSource.x = tdim*3;
                }
                else {                              //look 5
                    tetSource.x = tdim*4;
                }
            }
        }   


        BeginDrawing();
            MusicPlayer().updateMusic();
            ClearBackground(BLACK);
            DrawTexturePro(t, tetSource, tetDest, tetOrigin, 0, WHITE);
            DrawRectangleRounded(textBox, 0.2, 500, textBoxColor);
            DrawRectangleRoundedLines(textBox, 0.2, 500, 5, (Color){30, 30, 30, 255});
            DrawTextPro(tetFont, txt.c_str(), (Vector2){textBox.x+15, textBox.y+10}, (Vector2){0,0}, 0, 30, 0, textColor);
            DrawTextEx(tetFont, "S to skip, CLICK to proceed", (Vector2){800-MeasureTextEx(tetFont, "S to skip, CLICK to proceed", 15, 0).x-5, 800-18}, 15, 0, WHITE);
            DrawRectangle(0, 0, 800, 800, Fade(BLACK, fade));
        EndDrawing();
        if (end) {
            fade+=0.01;
            if (fade >= 1.5) {
                return 0;
            }
        }
    }
    return -1;
}

void Tet::usePower(tetPower p) {
    if (p.power == "less") {
        scr->addMultiplier(0.7);
        sound().play("resources/audio/debuff.mp3");
    }
    else if (p.power == "half") {
        scr->addMultiplier(0.5);
        sound().play("resources/audio/debuff.mp3");
    }
    else if (p.power == "negative") {
        scr->addMultiplier(-1);
        sound().play("resources/audio/debuff.mp3");
    }
    else if (p.power == "babies1") {
        babies = std::make_tuple(1, 200, 200);
        sound().play("resources/audio/baby_laugh.mp3");
    }
    else if (p.power == "babies2"){
        babies = std::make_tuple(2, 300, 230);
        sound().play("resources/audio/baby_laugh.mp3");
    }
    else if (p.power == "babies3") {
        babies = std::make_tuple(3, 400, 260);
        sound().play("resources/audio/baby_laugh_final.mp3");
    }
    else if (p.power == "fly") {
        flying = true;
    }
    else currPower = p.power;
}

void Tet::tetTalk(bool wait) {
    if (tetStage <= 2 && !wait) {
        if (tetCounter == tetCounterMax) {
            //opening tet mouth
            if (facePhase == 0) {
                source.x+=tdim;
                facePhase = 1;
                tetCounterMax = GetRandomValue(8, 12);
            }
            //closing tet mouth
            else {
                source.x-=tdim;
                facePhase = 0;
                tetCounterMax = GetRandomValue(3, 7);
            }
            tetCounter = 0;
        }
        else tetCounter++;
    }
}

void Tet::tetBob() {
    //for tet bobbing up and down constantly
    double maxBobSpeed = 0.25;                      //defining the maximum speed which tet will accelerate to when bobbing up and down
    if (bobWait) {                                  //waiting at the top and bottom of the bob
            if (bobCounter == 50) {
                bobCounter = 0;
                bobWait = false;
            }
            else {
                bobCounter++;
            }
    }
    else {
        dest.y += yvel;
        if (dest.y <= 335 && yvel < 0) {            //decelerating when passing this point
            yvel+=maxBobSpeed/50;
            if (yvel > -maxBobSpeed/100) {
                yvel = maxBobSpeed/50;
                bobWait = true;
            }
        }
        else if (dest.y >= 343.75 && yvel > 0) {    //decelerating when passing this point
            yvel-=maxBobSpeed/50;
            if (yvel < maxBobSpeed/100) {
                dest.y = 350;
                yvel = -maxBobSpeed/50;
                bobWait = true;
            }
        }
        else if (yvel < 0 && yvel >-maxBobSpeed) {
            yvel-=maxBobSpeed/50;
        }
        else if (yvel > 0 && yvel < maxBobSpeed) {
            yvel+=maxBobSpeed/50;
        }
    }

    //for tete glowing eye animation
    if (tetStage >= 3) {
        if (yvel > 0) {
            if (dest.y < 329) {             //look 5
                source.x = tdim*4;
            }
            else if (dest.y < 334) {        //look 4
                source.x = tdim*3;
            }
            else if (dest.y < 345) {        //look 3
                source.x = tdim*2;
            }
            else if (dest.y < 350) {        //look 2
                source.x = tdim;
            }
            else {                          //look 1
                source.x = 0;
            }
        }
        else {
            if (dest.y >= 350) {            //look 1
                source.x = 0;
            }
            else if (dest.y >= 345) {       //look 2
                source.x = tdim;
            }
            else if (dest.y >= 334) {       //look 3
                source.x = tdim*2;
            }
            else if (dest.y >= 329) {       //look 4
                source.x = tdim*3;
            }
            else {                          //look 5
                source.x = tdim*4;
            }
        }
    }   
}

std::string Tet::checkTetPower() {
    std::string temp = currPower;
    currPower = "none";
    return temp;
}

void Tet::drawTetText(std::string& s) {
    //adds each line in s, seperated by new line characters into seperate strings and puts them in a vector
    int start = 0;
    std::vector<std::string> lines;
    for (int i = 0; i < s.length(); i++){
        if (s[i] == '\n') {                                                                                 //i index represents new line character
            lines.push_back(s.substr(start, i-start));                                                      //add section of string starting at start index to right before new line character
            start = i+1;                                                                                    //new start index is character after new line character
        }
    }
    lines.push_back(s.substr(start));                                                                       //push back the remaining text
    Vector2 center = {650, float(160-(lines.size()/2.f)*20+10)};                                            //figure out where first line should start based on number of lines
                                                                                                            //center of entire text will be at (650, 160)
    for (string i: lines) {
        Vector2 textMeasure = MeasureTextEx(tetFont, i.c_str(), 20, 0);
        DrawTextPro(tetFont, i.c_str(), center, {textMeasure.x/2, textMeasure.y/2}, 0, 20, 0, WHITE);       //drawing each individual line
        center.y+=20;                                                                                       //next line will be 20 pixels below current line
    }
}

void Tet::checkStage() {
    if (scr->getScore() >= 25000 && tetStage < 2) {
        tetStage = 2;
        UnloadTexture(t);
        t = LoadTexture("resources/tet/tet_2.png");
        MusicPlayer().fade(300, "resources/music/tet_stage_2.mp3");
    }
    else if (scr->getScore() >= 50000 && tetStage < 3) {
        tetStage = 3;
        UnloadTexture(t);
        if (rotation == 0) t = LoadTexture("resources/tet/tet_3_forward.png");
        else t = LoadTexture("resources/tet/tet_3_left.png");
        MusicPlayer().fade(300, "resources/music/tet_stage_3.mp3");
    }
    else if (scr->getScore() >= 75000 && tetStage < 4) {
        tetStage = 4;
        UnloadTexture(t);
        if (rotation == 0) t = LoadTexture("resources/tet/tet_4_forward.png");
        else t = LoadTexture("resources/tet/tet_4_left.png");
        MusicPlayer().fade(300, "resources/music/tet_stage_4.mp3");
    }

    //handled in game.cpp

    //else if (scr->getScore() >= 90000) {
        //cutscene and tet goes crazy with his powers
    //}
    //else if (scr->getScore() >= 100000) {
        //you win
    //}
}

int Tet::tetCutscene() {
    //defining variables
    Texture2D tetFace = LoadTexture("resources/tet/tet_4_forward.png");
    Rectangle tetSource = (Rectangle) {0, 0, tdim, tdim};
    float scale = 0.01;
    Rectangle tetDest = (Rectangle) {400, 350, tdim*scale, tdim*scale};
    Vector2 tetOrigin = (Vector2){scale*tdim/2, scale*tdim/2};
    float tBoxWidth = 700, tBoxHeight = 125; Color textBoxColor = (Color) {45, 45, 45, 255};
    Rectangle textBox = (Rectangle) {400-tBoxWidth/2, 700-tBoxHeight/2, tBoxWidth, tBoxHeight};
    Color textColor = (Color){30, 0, 0, 255}; SetTextLineSpacing(25);
    int txtcounter = 0, txtWait = 2, begCount = 0, bobCount = 0, soundCount = 0;
    bool wait = false, checkEnd = false, start = true, end = false, waitBob = false;
    float fade = 0, tetVel = -0.01;

    //opening monologue file
    ifstream mono("resources/tet/tetCutscene.txt");
    string txt;
    char ch;

    //playing music
    MusicPlayer().play("resources/music/tet_theme.mp3");

    //tet flying towards player sound
    sound().play("resources/audio/tet_fly_cutscene.mp3");

    //loop for Tet's monologue
    while(!WindowShouldClose()) {

        MusicPlayer().updateMusic();                        //updating music stream

        //tet texture enlarges and waits some time before speaking
        if (start) {
            scale*=1.03;
            if (scale >= 6) {                               //tet is of size so wait some time before starting to speak
                bobCounter = 0;
                scale = 6;
                start = false;
            }
            //rescaling drawing values to match current scale
            tetDest = (Rectangle) {400, 350, tdim*scale, tdim*scale};
            tetOrigin = (Vector2){scale*tdim/2, scale*tdim/2};
        }
        else if (begCount < 120) begCount++;                //waiting some time in after appearing before speaking
        else if (!wait) {
            if (txtcounter == txtWait) {

                if (txtWait == 15) txtWait = 2;
                mono.get(ch);

                //checking characters
                if (ch == '&') {                            //& character marks the end of the text and end of cutscene
                    checkEnd = true;
                    wait = true;
                } 
                else if (ch == '$') {                       //$ marks the end of this section
                    wait = true;
                    mono.get(ch);                           //removing the next line character
                }
                else txt+=ch;
                if (ch == '.' || ch == '?' || ch == '!') {  //pausing for end of sentences
                    txtWait = 15;
                    soundCount = 0;                         //make sure sound doesn't play when there is punctuation
                }

                txtcounter = 0;

                //playing tet talking sounds
                if (ch != ' ' && txtWait != 15) {           //sound doesn't play when there is a space or if there is punctuation
                    if (soundCount == 2) {                  //play sound, wait two letters, play another sound
                        //grabbing sound from our pool
                        std::vector<std::string>::iterator it = tetSounds.begin() + GetRandomValue(0, tetSounds.size()-1);
                        sound().play(*it);
                        tetSounds.erase(it);
                        if (tetSounds.size() == 3) {        //resetting the pool if it gets too small
                            tetSounds = tetSoundsOriginal;
                        }
                        soundCount = 0;
                    }
                    else soundCount++;
                }          
            }
            else ++txtcounter;
            
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!checkEnd) {
                txt = "";
                txtcounter = 0;
                wait = false;
            }
            else end = true;
        }
        if (IsKeyPressed(KEY_S)) end = true;

        //for tet bobbing up and down constantly
        double maxBobSpeed = 0.5;                       //defining the maximum speed which tet will accelerate to when bobbing up and down
        if (waitBob) {                                  //waiting at the top and bottom of the bob
                if (bobCount == 50) {
                    bobCount = 0;
                    waitBob = false;
                }
                else {
                    bobCount++;
                }
        }
        else if (scale == 6) {
            tetDest.y += tetVel;
            if (tetDest.y <= 300  && tetVel < 0) {              //decelerating when passing this point
                tetVel+=maxBobSpeed/50;
                if (tetVel > -maxBobSpeed/100) {
                    tetVel = maxBobSpeed/50;
                    waitBob = true;
                }
            }
            else if (tetDest.y >= 337 && tetVel > 0) {          //decelerating when passing this point
                tetVel-=maxBobSpeed/50;
                if (tetVel < maxBobSpeed/100) {
                    tetDest.y = 350;
                    tetVel = -maxBobSpeed/50;
                    waitBob = true;
                }
            }
            else if (tetVel < 0 && tetVel >-maxBobSpeed) {
                tetVel-=maxBobSpeed/50;
            }
            else if (tetVel > 0 && tetVel < maxBobSpeed) {
                tetVel+=maxBobSpeed/50;
            }
        }

        //for tet glowing eye animation
        if (tetStage >= 3) {
            if (tetVel > 0) {
                if (tetDest.y < 286.8) {             //look 5
                    tetSource.x = tdim*4;
                }
                else if (tetDest.y < 304.1) {        //look 4
                    tetSource.x = tdim*3;
                }
                else if (tetDest.y < 333.6) {        //look 3
                    tetSource.x = tdim*2;
                }
                else if (tetDest.y < 350) {        //look 2
                    tetSource.x = tdim;
                }
                else {                          //look 1
                    tetSource.x = 0;
                }
            }
            else {
                if (tetDest.y >= 350 ) {            //look 1
                    tetSource.x = 0;
                }
                else if (tetDest.y >= 333.6) {       //look 2
                    tetSource.x = tdim;
                }
                else if (tetDest.y >= 304.1) {       //look 3
                    tetSource.x = tdim*2;
                }
                else if (tetDest.y >= 286.769) {       //look 4
                    tetSource.x = tdim*3;
                }
                else {                          //look 5
                    tetSource.x = tdim*4;
                }
            }
        }   


        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(t, tetSource, tetDest, tetOrigin, 0, WHITE);
            DrawRectangleRounded(textBox, 0.2, 500, textBoxColor);
            DrawRectangleRoundedLines(textBox, 0.2, 500, 5, (Color){30, 30, 30, 255});
            DrawTextPro(tetFont, txt.c_str(), (Vector2){textBox.x+15, textBox.y+10}, (Vector2){0,0}, 0, 30, 0, textColor);
            DrawTextEx(tetFont, "S to skip, CLICK to proceed", (Vector2){800-MeasureTextEx(tetFont, "S to skip, CLICK to proceed", 15, 0).x-5, 800-18}, 15, 0, WHITE);
            DrawRectangle(0, 0, 800, 800, Fade(BLACK, fade));
        EndDrawing();
        if (end) {
            if (fade == 0) MusicPlayer().fade(140);
            fade+=0.01;
            if (fade >= 1.5) {
                UnloadTexture(tetFace);
                return 0;
            }
        }
    }
    return -1;
}

void Tet::tetFinalStage() {
    tetText = "";
    txtTracker = 0;
    time = 0;
    wait = false;
    txtCounter = 0;
    talk = 0;
    finalStage = true;
    finalStart = true;
    timebetweenText = 120;
}