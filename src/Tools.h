#ifndef RANDOMBLOCK_H
#define RANDOMBLOCK_H

#include <raylib.h>
#include <vector>
#include <math.h>
#include <iostream>

//same as Vector2 type but uses more precise types
typedef struct v2 {
    long double x;
    long double y;
} v2;

//to get a random block from a bag
struct RandomBlock {
    public:
        //chooses from bag of 7 blocks
        int getBlock();

    private:
        //resets our bag and adds blocks back in when empty
        void resetBag();
        std::vector<int> blocks;
};

//handles playing music
struct MusicPlayer {
    public:
        //continously plays this file on repeat
        static void play(std::string file);
        //used when pause button is hit
        static void pause();
        //resuming after leaving pause menu
        static void resume();
        //fades out current song and plays new song immediately after
        static void fade(int frames, std::string file = "");
        //updating funciton
        static void updateMusic();
        //updating for pause menu music
        static void updatePauseMusic();
    private:
        static float vol;               //volume of the music
        static float decrease;          //how much we decrease the volume each frame
        static bool playing;            //are we currently playing a song
        static std::string nextSong;    //the next song that will be played after fading
        static Music pauseSong;         //seperate variable for pause menu music
        static Music song;              //current music playing
};

//handles sound playing and unloading
//plays the sound using the play() function and adds it to our soundlist. Every so often, goes through sound list and unloads sounds done playing
struct sound {
    public:
        //play sound
        static void play(std::string file);
        //updating function
        static void updateSound();
    private:
        struct item {
            Sound curr;                                             //current item
            item* next;                                             //next item
            item* prev;                                             //previous item
        };

        //list
        struct soundlist {
            void push_back(Sound i);                                //adds new sound to the back of the list
            void remove(item* i);                                   //deletes the given item
            item* head = nullptr;                                   //head of the list
            item* tail = nullptr;                                   //tail of the list
        };

        static int resetCounter;                                    //counter for when we go through list and unload sounds
        static soundlist sounds;                                    //list of played sounds
};

//to get accurate coordinates relative to the screen when there is a camera rotation
Vector2 getAdjustedCoordinates(Vector2 coor, double rotation);

//take derivative of distance between center of circle and line. If closest distance is less than radius of circle then there is a collision
bool checkCollisionLineCircle(Vector2 p1, Vector2 p2, Vector2 c, float r);

//return Vector2 of the closest point of line to center of circle
Vector2 closestPointLineCircle(Vector2 p1, Vector2 p2, Vector2 c);

#endif