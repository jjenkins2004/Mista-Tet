#include "score.h"
#include "string"

void Text::move() {
        xpos+=xVelocity;
        ypos+=yVelocity;
        if (counter == 2) {
            yVelocity+=1;
            counter = 0;
        }
        else counter++;
        alpha-=0.025;
    }




Score::Score(): score(0) {}

void Score::drawScore() {
    Color DarkRed = {static_cast<unsigned char>(255-level*1.4), 0, 0, 255};

    std::string multipliertxt = "x";
    int lngth = std::to_string(int(multiplier)).length();
    if (multiplier < 0 && multiplier> -1) lngth = 2;
    multipliertxt+=std::to_string(multiplier).substr(0,2+lngth);
        
    DrawRectangleRoundedLines((Rectangle){600, 720, 100, 60}, 0.2, 100, 5, (Color){80, 60, 60, 255});
    DrawTextPro(allFont, multipliertxt.c_str(), (Vector2){650, 750}, (Vector2){MeasureTextEx(allFont, multipliertxt.c_str(), 25, 3).x/2, MeasureTextEx(GetFontDefault(), multipliertxt.c_str(), 25, 3).y/2}, 0, 25, 3, DarkRed);
    DrawTextPro(allFont, multipliertxt.c_str(), (Vector2){650, 750}, (Vector2){MeasureTextEx(allFont, multipliertxt.c_str(), 25, 3).x/2, MeasureTextEx(GetFontDefault(), multipliertxt.c_str(), 25, 3).y/2}, 0, 25, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));

    std::string text;
    for (int i = 0; i < 6-std::to_string(int(score)).length(); i++) text+='0';
    text+=std::to_string(int(score));
    DrawRectangleRoundedLines((Rectangle){570, 625, 160, 90}, 0.2, 100, 5, (Color){80, 60, 60, 255});
    DrawTextPro(allFont, "score", (Vector2){650, 645}, (Vector2){MeasureTextEx(allFont, "score", 35, 3).x/2, MeasureTextEx(allFont, "score", 35, 3).y/2}, 0, 35, 3, DarkRed);
    DrawTextPro(allFont, "score", (Vector2){650, 645}, (Vector2){MeasureTextEx(allFont, "score", 35, 3).x/2, MeasureTextEx(allFont, "score", 35, 3).y/2}, 0, 35, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    DrawTextPro(allFont, text.c_str(), (Vector2){650, 690}, (Vector2){MeasureTextEx(allFont, text.c_str(), 30, 3).x/2, MeasureTextEx(allFont, text.c_str(), 30, 3).y/2}, 0, 30, 3, DarkRed);
    DrawTextPro(allFont, text.c_str(), (Vector2){650, 690}, (Vector2){MeasureTextEx(allFont, text.c_str(), 30, 3).x/2, MeasureTextEx(allFont, text.c_str(), 30, 3).y/2}, 0, 30, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    for (int i = 0; i < scores.size(); i++) {
        if (scores[i].alpha <= 0) scores.assign(scores.begin()+i, scores.end());
        DrawTextEx(allFont, scores[i].text.c_str(), (Vector2){static_cast<float>(scores[i].xpos), static_cast<float>(scores[i].ypos)}, scores[i].fsize, 1, Fade(scores[i].color, scores[i].alpha));
        scores[i].move();
    }
}

void Score::addScore(int score) {
    float scr = score*multiplier;
    if (this->score+scr > 0) this->score+=scr;
    else this->score = 0;
    std::string s;
    if (scr > 0) s = "+" + convertDecimal(scr);
    else s = convertDecimal(scr);
    int font = 12 + log2(abs(scr))*2;
    Color c = WHITE;
    if (scr >= 100) c = GOLD;
    else if (scr < 0) c = RED;
    scores.push_back(Text(s.c_str(), 690, 637, font, c));
}

void Score::addScoreIgnoreNeg(int score) {
    float scr = score*(multiplier > 0 ? multiplier: multiplier*-1);
    if (this->score+scr > 0) this->score+=scr;
    else this->score = 0;
    std::string s;
    if (scr > 0) s = "+" + convertDecimal(scr);
    else s = convertDecimal(scr);
    int font = 12 + log2(abs(scr))*2;
    Color c = WHITE;
    if (scr >= 100) c = GOLD;
    else if (scr < 0) c = RED;
    scores.push_back(Text(s.c_str(), 690, 637, font, c));
}

void Score::updateMultiplier() {
    multiplier = baseMultiplier;
    for (std::deque<std::pair<double, int>>::iterator it = currmultipliers.begin(); it != currmultipliers.end(); it++) {
        if (it == currmultipliers.begin() && it->second <= 0) currmultipliers.pop_front();
        multiplier*=(it->first);
        it->second--;
    }
}

void Score::addMultiplier(double i) {
    currmultipliers.push_back(std::make_pair(i, 1800));
}

std::string Score::convertDecimal(float i) {
    if (i < 0) i = float(int(i*10-.5))/10;
    else i = float(int(i*10+.5))/10;
    std::string s = std::to_string(i);
    for (int i = 0; i < s.length(); i++) {
        if (s.substr(i, 1) == ".") {
            if (i < 2) return s.substr(0, i+2);
            else return s.substr(0, i);
        } 
    }
    return s;
}

void Score::addPermanentMultiplier(double m) {
    baseMultiplier+= m;
}

void Score::halfMultiplier() {
    baseMultiplier/=2;
}