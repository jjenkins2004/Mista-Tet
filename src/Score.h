#ifndef SCORE_H
#define SCORE_H

#include <vector>
#include <deque>
#include <string>
#include <raylib.h>
#include <iostream>
#include "math.h"

//a singular score item, which flies out of score board when score is added and disappears
struct Text {

    Text(std::string t, int x, int y, int f, Color c): text(t), xpos(x), ypos(y), fsize(f), alpha(1), color(c) {}       //constructor

    std::string text;                       //the score that is displayed
    int xpos, ypos;                         //position of the text
    int fsize;                              //font size of the text
    double alpha;                           //fade of the text
    Color color;                            //color of the text

    //used for animation when score is being added
    void move();

    private:
        int counter = 0;
        int xVelocity = 1, yVelocity = -3;  //x and y velocity
};

class Score {

    public:
        //updating, getting, drawing
        Score();                                        //constructor
        ~Score();                                       //destructor
        void updateLevel(int l) {level = l;}            //update the current level
        void updateMultiplier();                        //updating the time on multipliers
        float getScore() {return score;}                //get score
        void drawScore();                               //draws scoreboard and score

        //adding score and multipliers
        void addScore(int score);                       //add some number to the score, taking into account the multiplier
        void addScoreIgnoreNeg(int score);              //add score but ignore negative multiplier
        void addMultiplier(double i);                   //add some multiplier for 30 seconds
        void addPermanentMultiplier(double m);          //add a permanent multiplier
        void halfMultiplier();                          //half base multiplier (only used initially for final stage)

        
    private:
        //convert a decimal to a string with only one decimal place or none
        std::string convertDecimal(float x);

        Font allFont = LoadFont("resources/all_font.ttf");                  //font we are using for text
        std::deque<std::pair<double, int>> currmultipliers;                 //the current multipliers that are being applied
        int level;                                                          //current level (used for color of text)
        std::vector<Text> scores;                                           //used for animation of scores being added
        float multiplier = 1;                                               //current multiplier
        float baseMultiplier = 1;                                           //base multiplier which only includes permanent multipliers
        float score;                                                        //the current score
        
};

#endif