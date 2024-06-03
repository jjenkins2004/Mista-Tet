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
    DrawTexturePro(t, source, dest, (Vector2){3.5*tdim/2, 3.5*tdim/2}, 0,  WHITE);

    if (wait) {
        if (time == waitTime) {
            time = 0;
            wait = false;
            tetText = "";
        }
        time++;
        SetTextLineSpacing(20);
        DrawTextPro(tetFont, tetText.c_str(), (Vector2){550, 100}, (Vector2){0, 0}, 0, 20, 0, WHITE);
    }
    else if (time == timebetweenText) {
        if (txtCounter == txtCounterWait) {
            if (!stop) {
                if (tetpowertoggle) {
                    if (tetStage <=2) {
                        tetText+= tetPowers1[txtIndex].dialogue.substr(txtTracker, 1);
                        if (txtTracker == tetPowers1[txtIndex].dialogue.length()-1) {
                            std::cout << "stopping" << std::endl;
                            tetpowertoggle = false;
                            stop = true;
                        }
                    }
                    else {
                        tetText += tetPowers2[txtIndex].dialogue.substr(txtTracker, 1);
                        if (txtTracker == tetPowers2[txtIndex].dialogue.length()-1) {
                            tetpowertoggle = false;
                            stop = true;
                        }
                    }
                }
                else {
                    tetText+= noEffectText[txtIndex].substr(txtTracker, 1);
                    if (txtTracker == noEffectText[txtIndex].length()-1) {
                        stop = true;
                    }
                }
                string lastletter = tetText.substr(tetText.length()-1, 1);
                if (lastletter == "." || lastletter == "?" || lastletter == "!") {
                    txtCounterWait = 15;
                }
                else {
                    if (talk == 2) {
                        PlaySound(LoadSound(tetSounds[GetRandomValue(0, 6)].c_str()));
                        talk = 0;
                    }
                    else {
                        talk++;
                    }
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
                timebetweenText = GetRandomValue(1*60, 3*60);
            }
            txtCounter = 0;
        }
        else {
            ++txtCounter;
        }
        SetTextLineSpacing(20);
        DrawTextPro(tetFont, tetText.c_str(), (Vector2){550, 100}, (Vector2){0, 0}, 0, 20, 0, WHITE);
    }
    else {
        time++;
        if (time == timebetweenText) {
            stop = false;
            //choosing which text we should put on screen
            if (vals.size() == 0) {
                for (int i = 0; i < 20; i++) {
                    vals.push_back(i);
                }
            }
            int num = 3;
            if (tetStage >= 3) {
                num = 5;
            }
            if (GetRandomValue(1, 10) <= num) {
                tetpowertoggle = true;
                if (tetStage <=2) {
                    txtIndex = /*GetRandomValue(0, 6)*/0;
                }
                else {
                    txtIndex = GetRandomValue(0, 7);
                }
            }
            else {
                std::vector<int>::iterator it = vals.begin()+GetRandomValue(0, vals.size()-1);
                txtIndex = *it;
                vals.erase(it);
            }
        }
    }
}

int Tet::tetMonologue() {
    //defining variables
    Rectangle tetSource = (Rectangle) {0, 0, tdim, tdim};
    Rectangle tetDest = (Rectangle) {400, 300, 6*tdim, 6*tdim};
    Vector2 tetOrigin = (Vector2){6*tdim/2, 6*tdim/2};
    float tBoxWidth = 700, tBoxHeight = 125; Color textBoxColor = (Color) {45, 45, 45, 255};
    Rectangle textBox = (Rectangle) {400-tBoxWidth/2, 700-tBoxHeight/2, tBoxWidth, tBoxHeight};
    Color textColor = (Color){30, 0, 0, 255}; SetTextLineSpacing(25);
    int txtcounter = 0;
    bool wait = false;
    bool checkEnd = false;
    bool end = false;
    float facePhase = 0;
    int faceCounter = 0; 
    int faceCounterChange = GetRandomValue(3, 7);
    double fade = 0;

    //opening monologue file
    ifstream mono("resources/tetMonologue.txt");
    string txt;
    char ch;

    //loop for Tet's monologue
    while(!WindowShouldClose()) {
        tetSource = (Rectangle) {facePhase*tdim, 0, tdim, tdim};
        if (!wait && facePhase == 0 && faceCounter == faceCounterChange) {
            facePhase = 1;
            faceCounterChange = GetRandomValue(8, 12);
            faceCounter = 0;
        }
        if (!wait && facePhase == 1 && faceCounter == faceCounterChange) {
            facePhase = 0;
            faceCounterChange = GetRandomValue(3, 7);
            faceCounter = 0;
        }
        if (!wait) {
            if (txtcounter == 2) {
                mono.get(ch);

                //& character marks the end of the text
                if (ch == '&') {
                    checkEnd = true;
                    wait = true;
                } else {
                     txt+=ch;
                }
                //stopping everytime there are two next lines in a row
                if (txt.length() > 2 && *(txt.end()-1) == '\n' && *(txt.end()-2) == '\n') {
                    wait = true;
                }
                txtcounter = 0;
            }
            
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!checkEnd) {
                txt = "";
                txtcounter = 0;
                faceCounter = 0;
                wait = false;
            }
            else {
                end = true;
            }
        }
        if (IsKeyPressed(KEY_S)) {
            end = true;
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
        ++txtcounter;
        ++faceCounter;
        if (end) {
            fade+=0.01;
            if (fade >= 1.5) {
                return 0;
            }
        }
    }
    return -1;
}