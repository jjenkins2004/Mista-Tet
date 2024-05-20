#ifndef SCORE_H
#define SCORE_H

#include <vector>
#include <deque>
#include <string>
#include <raylib.h>

struct Text {
    Text(std::string t, int x, int y, int f, Color c): text(t), xpos(x), ypos(y), fsize(f), alpha(1), color(c) {}
    std::string text;
    int xpos;
    int ypos;
    int fsize;
    double alpha;
    Color color;

    //used for animation when score is being added
    void move() {
        xpos+=xVelocity;
        ypos+=yVelocity;
        if (counter == 2) {
            yVelocity+=1;
            counter = 0;
        }
        else {
            counter++;
        }
        alpha-=0.025;
    }

    private:
        int counter = 0;
        int xVelocity = 1;
        int yVelocity = -3;
};

class Score {
    public:
        Score();

        //set level
        void updateLevel(int l) {
            level = l;
        }

        //draws scoreboard and score
        void drawScore();

        //add some numeber to the score
        void addScore(int score);

        //adding multiplier
        void addMultiplier(double i);
        //updating the multipliers
        void updateMultiplier();

        
    private:
        //font we are using for text
        Font allFont = LoadFont("resources/allFont.ttf");

        //the current multipliers that are being applied
        std::deque<std::pair<double, int>> currmultipliers;

        //current level (used for color of text)
        int level;

        //used for animation of scores being added
        std::vector<Text> scores;

        //current multiplier
        double multiplier = 1;

        //the current score
        int score;
        
};

#endif