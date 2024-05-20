#include "Score.h"
#include "string"
#include "iostream"

Score::Score(): score(0) {}

void Score::drawScore() {
    Color DarkRed = {static_cast<unsigned char>(255-level*1.4), 0, 0, 255};

    std::string multipliertxt = "x";
    int lngth = std::to_string(int(multiplier)).length();
    if (multiplier < 0 && multiplier> -1) {
        lngth = 2;
    }
    multipliertxt+=std::to_string(multiplier).substr(0,2+lngth);
        
    DrawRectangleRoundedLines((Rectangle){600, 720, 100, 60}, 0.2, 100, 5, (Color){80, 60, 60, 255});
    DrawTextPro(allFont, multipliertxt.c_str(), (Vector2){650, 750}, (Vector2){MeasureTextEx(allFont, multipliertxt.c_str(), 25, 3).x/2, MeasureTextEx(GetFontDefault(), multipliertxt.c_str(), 25, 3).y/2}, 0, 25, 3, DarkRed);
    DrawTextPro(allFont, multipliertxt.c_str(), (Vector2){650, 750}, (Vector2){MeasureTextEx(allFont, multipliertxt.c_str(), 25, 3).x/2, MeasureTextEx(GetFontDefault(), multipliertxt.c_str(), 25, 3).y/2}, 0, 25, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));

    std::string text;
    for (int i = 0; i < 6-std::to_string(score).length(); i++) {
        text+='0';
    }
    text+=std::to_string(score);
    DrawRectangleRoundedLines((Rectangle){570, 625, 160, 90}, 0.2, 100, 5, (Color){80, 60, 60, 255});
    DrawTextPro(allFont, "score", (Vector2){650, 645}, (Vector2){MeasureTextEx(allFont, "score", 35, 3).x/2, MeasureTextEx(allFont, "score", 35, 3).y/2}, 0, 35, 3, DarkRed);
    DrawTextPro(allFont, "score", (Vector2){650, 645}, (Vector2){MeasureTextEx(allFont, "score", 35, 3).x/2, MeasureTextEx(allFont, "score", 35, 3).y/2}, 0, 35, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    DrawTextPro(allFont, text.c_str(), (Vector2){650, 690}, (Vector2){MeasureTextEx(allFont, text.c_str(), 30, 3).x/2, MeasureTextEx(allFont, text.c_str(), 30, 3).y/2}, 0, 30, 3, DarkRed);
    DrawTextPro(allFont, text.c_str(), (Vector2){650, 690}, (Vector2){MeasureTextEx(allFont, text.c_str(), 30, 3).x/2, MeasureTextEx(allFont, text.c_str(), 30, 3).y/2}, 0, 30, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    for (int i = 0; i < scores.size(); i++) {
        if (scores[i].alpha <= 0) {
            scores.assign(scores.begin()+i, scores.end());
        }
        DrawTextEx(allFont, scores[i].text.c_str(), (Vector2){static_cast<float>(scores[i].xpos), static_cast<float>(scores[i].ypos)}, scores[i].fsize, 1, Fade(scores[i].color, scores[i].alpha));
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

void Score::updateMultiplier() {
    multiplier = 1;
    for (std::deque<std::pair<double, int>>::iterator it = currmultipliers.begin(); it != currmultipliers.end(); it++) {
        if (it == currmultipliers.begin() && it->second <= 0) {
            currmultipliers.pop_front();
        }
        multiplier*=(it->first);
        it->second--;
    }
}

void Score::addMultiplier(double i) {
    currmultipliers.push_back(std::make_pair(i, 1800));
}