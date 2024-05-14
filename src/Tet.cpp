#include "Tet.h"
#include <iostream>
#include <fstream>

using namespace std;

Tet::Tet(Score* s): src(s) {
     tet1 = LoadTexture("resources/mistaTet1.png");
     tetFont = LoadFont("resources/allFont.ttf");
}

void Tet::drawTet() {
    Rectangle source = (Rectangle) {0, 0, tdim, tdim};
    Rectangle dest = (Rectangle) {650, 400, tdim*3.5, tdim*3.5};
    DrawTexturePro(tet1, source, dest, (Vector2){3.5*tdim/2, 3.5*tdim/2}, 0,  WHITE);
    DrawTextPro(tetFont, "hello i like poo", (Vector2){550, 50}, (Vector2){0, 0}, 0, 20, 0, WHITE);
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
            DrawTexturePro(tet1, tetSource, tetDest, tetOrigin, 0, WHITE);
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