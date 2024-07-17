#include "Tet.h"
#include <iostream>
#include <fstream>

using namespace std;

Tet::Tet(Score* s): src(s) {
     for (int i = 0; i < 20; i++) {
        vals.push_back(i);
     }
}

void Tet::drawTet() {
    
    //tet and what he is saying
    DrawTexturePro(t, source, dest, (Vector2){3.5*tdim/2, 3.5*tdim/2}, rotation,  WHITE);

    //tetbobbing
    tetBob();

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
    else if (time == timebetweenText) { //checks if we should make tet say something
        if (txtCounter == txtCounterWait) {
            spaceWait = false;
            if (!stop) {
                if (tetpowertoggle) {
                    if (tetStage <=2) {
                        tetText+= tetPowers1[txtIndex].dialogue.substr(txtTracker, 1);
                        if (txtTracker == tetPowers1[txtIndex].dialogue.length()-1) {
                            tetpowertoggle = false;
                            stop = true;
                            usePower(tetPowers1[txtIndex]);
                        }
                    }
                    else {
                        tetText += tetPowers2[txtIndex].dialogue.substr(txtTracker, 1);
                        if (txtTracker == tetPowers2[txtIndex].dialogue.length()-1) {
                            tetpowertoggle = false;
                            stop = true;
                            usePower(tetPowers2[txtIndex]);
                        }
                    }
                }
                else {
                    tetText+= noEffectText[txtIndex].substr(txtTracker, 1);
                    if (txtTracker == noEffectText[txtIndex].length()-1) stop = true;
                }
                string lastletter = tetText.substr(tetText.length()-1, 1);
                if (lastletter == "." || lastletter == "?" || lastletter == "!") {
                    if (tetStage <= 2) {
                        spaceWait = true;
                        if (facePhase == 1) source.x-=tdim;
                        tetCounter = 0; tetCounterMax = GetRandomValue(3, 7); facePhase = 0;
                    }
                    txtCounterWait = 15;
                }
                else {
                    if (talk == 2) {
                        std::vector<std::string>::iterator it = tetSounds.begin() + GetRandomValue(0, tetSounds.size()-1);
                        sound().play(*it);
                        tetSounds.erase(it);
                        if (tetSounds.size() == 3) {
                            tetSounds = tetSoundsOriginal;
                        }
                        talk = 0;
                    }
                    else talk++;

                    txtCounterWait = 3;
                }
                txtTracker++;
            }
            else {
                talk = 2;
                time = 0;
                txtTracker = 0;
                wait = true;
                waitTime = 300 + tetText.size()*3;
                timebetweenText = GetRandomValue(1*60, 3*60); //how long should tet wait between dialogues
            }
            txtCounter = 0;
        }
        else ++txtCounter;
        SetTextLineSpacing(20);
        drawTetText(tetText);
        tetTalk(spaceWait);
    }
    else {
        time++;
        if (time == timebetweenText) {
            stop = false;
            //choosing which text we should put on screen
            if (vals.size() == 0) for (int i = 0; i < 20; i++) vals.push_back(i);
            int num = 4;
            if (tetStage >= 3) num = 7;                             //later stages of the game make tet powers happen more often
            if (GetRandomValue(1, 10) <= num) {                     //determines whether the next dialogue from tet should be a tet power
                tetpowertoggle = true;
                if (tetStage <=2) txtIndex = GetRandomValue(0, 5);  //choosing power from our early stage bag
                else txtIndex = GetRandomValue(0, 6);               //choosing power from our late stage bag
            }
            else {
                std::vector<int>::iterator it = vals.begin()+GetRandomValue(0, vals.size()-1);
                txtIndex = *it;
                vals.erase(it);
            }
        }
    }
}

void Tet::look(Vector2 coor) {
    if (coor.x < 0) {                                                   //reset rotation back to zero and make tet look forward
        rotation = 0;
        if (tetStage == 1 || tetStage == 2) source.x = tdim*facePhase;
        else if (tetStage == 3) t = LoadTexture("resources/tet/mistaTet3Forward.png");
        else if (tetStage == 4) t = LoadTexture("resources/tet/mistaTet4Forward.png");
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
    else if (tetStage == 3) t = LoadTexture("resources/tet/mistaTet3Left.png");
    else if (tetStage == 4) t = LoadTexture("resources/tet/mistaTet4Left.png");

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
    if (p.power == "less") src->addMultiplier(0.7);
    else if (p.power == "half") src->addMultiplier(0.5);
    else if (p.power == "negative") src->addMultiplier(-1);
    else currPower = p.power;
}

void Tet::tetTalk(bool wait) {
    if (tetStage <= 2 && !wait) {
        if (tetCounter == tetCounterMax) {
            if (facePhase == 0) {
                source.x+=tdim;
                facePhase = 1;
                tetCounterMax = GetRandomValue(8, 12);
            }
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
    if (src->getScore() >= 25000 && tetStage < 2) {
        tetStage = 2;
        UnloadTexture(t);
        t = LoadTexture("resources/tet/mistaTet2.png");
        MusicPlayer().fade(300, "resources/music/tetStage2Music.wav");
    }
    else if (src->getScore() >= 50000 && tetStage < 3) {
        tetStage = 3;
        UnloadTexture(t);
        if (rotation == 0) t = LoadTexture("resources/tet/mistaTet3Forward.png");
        else t = LoadTexture("resources/tet/mistaTet3Left.png");
        MusicPlayer().fade(300, "resources/music/tetStage3Music.wav");
    }
    else if (src->getScore() >= 75000 && tetStage < 4) {
        tetStage = 4;
        UnloadTexture(t);
        if (rotation == 0) t = LoadTexture("resources/tet/mistaTet4Forward.png");
        else t = LoadTexture("resources/tet/mistaTet4Left.png");
        MusicPlayer().fade(300, "resources/music/tetStage4Music.wav");
    }

    //handled in game.cpp

    //else if (src->getScore() >= 90000) {
        //cutscene and tet goes crazy with his powers
    //}
    //else if (src->getScore() >= 100000) {
        //you win
    //}
}

int Tet::tetCutscene() {
    //defining variables
    Texture2D tetFace = LoadTexture("resources/tet/mistaTet4Forward.png");
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
    MusicPlayer().play("resources/music/TetTheme.wav");

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