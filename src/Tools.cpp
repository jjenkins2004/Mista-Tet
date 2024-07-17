#include "Tools.h"

//--------------------------------------------------------------------------
// random block bag
//--------------------------------------------------------------------------

int RandomBlock::getBlock() {
    if (blocks.empty()) resetBag();
    int rand = GetRandomValue(0, blocks.size()-1);
    int num = blocks[rand];
    std::vector<int>::iterator it = blocks.begin()+rand;
    blocks.erase(it);
    return num;
}

void RandomBlock::resetBag() {
    for (int i = 1; i < 8; i++) blocks.push_back(i);
}

//--------------------------------------------------------------------------
// Powerup Item collision and math
//--------------------------------------------------------------------------

Vector2 getAdjustedCoordinates(Vector2 coor, double rotation) {
    double x = coor.x-400; double y = coor.y-400;       //finding vectors of the coordinates with (400, 400) as the center
                                                        //(400,400) acts as (0,0) because it is in the center of the screen
    double r = sqrt(pow(x, 2) + pow(y, 2));             //compute the radius
    double theta = atan2(y, x) - rotation*PI/180;       //find angle of current point in radians and add rotation
    return (Vector2){static_cast<float>(r*cos(theta)+400), static_cast<float>(r*sin(theta)+400)};       //returning new point
}

bool checkCollisionLineCircle(Vector2 point1, Vector2 point2, Vector2 center, float r) {

    //using more precise types
    v2 perpVec;
    v2 p1 = {point1.x, point1.y};
    v2 p2 = {point2.x, point2.y};
    v2 c = {center.x, center.y};

    //finding unit perpendicular vector
    if (p1.y-p2.y < 0.01 && p1.y-p2.y > -0.01) {            //check in case slope is 0
        perpVec = {0, 1};
    }
    else perpVec = {1, -(p1.x-p2.x)/(p1.y-p2.y)};           //the perpendicular vector of the line
    
    float scale = 1/sqrt(pow(perpVec.x, 2) + pow(perpVec.y, 2));    //coverting perpVector into unit vector
    perpVec.x*=scale;
    perpVec.y*=scale;

    //making sure perpendicular vector points away from center
    double dist1 = sqrt(pow(p1.x+perpVec.x-400 , 2) + pow(p1.y+perpVec.y-400, 2));
    double dist2 = sqrt(pow(p1.x-perpVec.x-400 , 2) + pow(p1.y-perpVec.y-400, 2));
    if (dist2 > dist1) {             
        perpVec.x*=-1;                  
        perpVec.y*=-1;                  
    }

    //finding magnitude of parameter for a vector line from item center to border in perpVec direction
    //in other words finding magnitude of line from center of item to closest point on border
    v2 slope;
    if (abs(p1.x-p2.x) < 0.01) {
        slope = {0, 1};
    }
    else slope = {1, (p1.y-p2.y)/(p1.x-p2.x)};
    double t = (slope.x*(c.y-p1.y)-slope.y*(c.x-p1.x))/(slope.y*perpVec.x-slope.x*perpVec.y); 

    return t < r;
}

Vector2 closestPointLineCircle(Vector2 point1, Vector2 point2, Vector2 center) {
    //using more precise types
    v2 perpVec;
    v2 p1 = {point1.x, point1.y};
    v2 p2 = {point2.x, point2.y};
    v2 c = {center.x, center.y};

    //finding unit perpendicular vector
    if (p1.y-p2.y < 0.01 && p1.y-p2.y > -0.01) {            //check in case slope is 0
        perpVec = {0, 1};
    }
    else perpVec = {1, -(p1.x-p2.x)/(p1.y-p2.y)};           //the perpendicular vector of the line
    
    //making sure perpendicular vector points away from center
    double dist1 = sqrt(pow(p1.x+perpVec.x-400 , 2) + pow(p1.y+perpVec.y-400, 2));
    double dist2 = sqrt(pow(p1.x-perpVec.x-400 , 2) + pow(p1.y-perpVec.y-400, 2));
    if (dist2 > dist1) {             
        perpVec.x*=-1;                  
        perpVec.y*=-1;                  
    }

    //finding slope of line
    v2 s;
    if (abs(p1.x-p2.x) < 0.01) {
        s = {0, 1};
    }
    else {
        s = {1, (p1.y-p2.y)/(p1.x-p2.x)};
    }

    //finding closest point by intersection of two parametric lines, one being the border and the other being a
    //line starting at center with direction of perpVec
    double t = (s.x*(c.y-p1.y)+s.y*(p1.x-c.x))/(s.y*perpVec.x-s.x*perpVec.y);
    return {float(c.x + t*perpVec.x), float(c.y+t*perpVec.y)};

}

//--------------------------------------------------------------------------
// MusicPlayer
//--------------------------------------------------------------------------

void MusicPlayer::play(std::string file) {
    UnloadMusicStream(song);
    song = LoadMusicStream(file.c_str());
    PlayMusicStream(song);
    playing = true;
}

void MusicPlayer::pause() {
    pauseSong = LoadMusicStream("resources/music/TetTheme.wav");
    PlayMusicStream(pauseSong);
}

void MusicPlayer::resume() {
    StopMusicStream(pauseSong);
    UnloadMusicStream(pauseSong);
}

void MusicPlayer::fade(int frames, std::string file) {
    decrease = 1.f/frames;
    nextSong = file;
}

void MusicPlayer::updateMusic() {
    if (playing) {
        //checking if we need to repeat the current song
        if (!IsMusicStreamPlaying(song)) {
             PlayMusicStream(song);
        }
        else UpdateMusicStream(song);

        //applying fade
        vol -= decrease;
        SetMusicVolume(song, vol);
        //unloading music when volume hits 0 from the fade
        if (vol <= 0) {
            StopMusicStream(song);
            vol = 1;
            decrease = 0;
            if (nextSong != "") {
                play(nextSong);
                nextSong = "";
            }
            else playing = false;
        }
    }
}

void MusicPlayer::updatePauseMusic() {
    if (!IsMusicStreamPlaying(pauseSong)) {
        PlayMusicStream(pauseSong);
    }
    else UpdateMusicStream(pauseSong);
}

Music MusicPlayer::song;
Music MusicPlayer::pauseSong;
std::string MusicPlayer::nextSong = "";
float MusicPlayer::vol = 1;
float MusicPlayer::decrease = 0;
bool MusicPlayer::playing = false;

//--------------------------------------------------------------------------
// sound and soundlist
//--------------------------------------------------------------------------

void sound::soundlist::push_back(Sound i) {
    if (tail != nullptr) {
        tail->next = new item();
        tail->next->next = nullptr; tail->next->curr = i; tail->next->prev = tail;
        tail = tail->next;
    }
    else {
        head = new item();
        tail = head;
        head->curr = i; head->next = nullptr; head->prev = nullptr;
    }
}

void sound::soundlist::remove(item* i) {
    if (i == head) head = i->next;
    if (i == tail) tail = i->prev;
    if (i->prev != nullptr) i->prev->next = i->next;
    if (i->next != nullptr) i->next->prev = i->prev;
    UnloadSound(i->curr);
    delete i;
}

void sound::play(std::string file) {
    sounds.push_back(LoadSound(file.c_str()));
    PlaySound(sounds.tail->curr);
}

void sound::updateSound() {
    if (resetCounter == 60*20) {
        //unload not currently playing sounds
        item* it = sounds.head;
        while(it != nullptr) {
            if (!IsSoundPlaying(it->curr)) {
                item* temp = it;
                it = it->next;
                sounds.remove(temp);
            }
            else {
                it = it->next;
            }
        }
        resetCounter = 0;
    }
    else ++resetCounter;

}

int sound::resetCounter = 0;
sound::soundlist sound::sounds = sound::soundlist();