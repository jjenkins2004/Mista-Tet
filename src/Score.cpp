#include "Score.h"
#include "string"
#include "iostream"

Score::Score(): score(0) {}

void Score::drawScore() {
    Rectangle rec = Rectangle(); rec.x = 569; rec.y = 585; rec.height = 110; rec.width = 153;
    DrawRectangleLinesEx(rec, 5, GRAY);
    DrawText("Score", 600, 600, 30, BLUE);
    std::string text;
    for (int i = 0; i < 6-std::to_string(score).length(); i++) {
        text+='0';
    }
    std::string multipliertxt = std::to_string(multiplier).substr(0, 3);
    DrawText(multipliertxt.c_str(), 650, 710, 40, WHITE);
    DrawText("X", 600, 710, 40, WHITE);
    text+=std::to_string(score);
    DrawText(text.c_str(), 595, 650, 30, DARKBLUE);
    std::cout << "size is currently " << scores.size() << std::endl;
    for (int i = 0; i < scores.size(); i++) {
        if (scores[i].alpha <= 0) {
            scores.assign(scores.begin()+i, scores.end());
        }
        DrawText(scores[i].text.c_str(), scores[i].xpos, scores[i].ypos, scores[i].fsize, Fade(scores[i].color, scores[i].alpha));
        scores[i].move();
    }
}

void Score::addScore(int score) {
    this->score+=score*multiplier;
    std::string s = "+" + std::to_string((int)(score*multiplier));
    int font = 12 + log2(score*multiplier)*2;
    Color c = WHITE;
    if (score*multiplier >= 50) {
        c = GOLD;
    }
    scores.push_back(Text(s.c_str(), 690, 637, font, c));
}