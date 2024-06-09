#ifndef TET_H
#define TET_H

#define tdim 70

#include "Score.h"
/*****
 * handles all actions related to tet; animations, dialogue, changes, etc...
*****/

class Tet {
    public:
        Tet(Score* s);

        //set level
        void updateLevel(int l) {
            level = l;
        }

        //draw mista tet
        void drawTet();
        //tet monologue
        int tetMonologue();
        //check if a tet power is activated
        std::string checkTetPower();


    private:

    //during the game for when tet is talking
    std::string tetText;
    int txtCounter = 0;
    int txtCounterWait = 3;
    int txtTracker = 0;
    int txtIndex; //which index are we using right now
    std::vector<int> vals; //which dialougues tet can say (to make sure two dont repeat back to back)
    int timebetweenText = GetRandomValue(1*60, 3*60);
    int time = 0; int waitTime; bool wait;
    bool stop;


    //possible tet text that has no effect
    const std::vector<std::string> noEffectText = {"You're like a cloud.\nWhen you disappear,\nit's a beautiful day.", 
                                            "Roses are red.\nViolents are blue.\nNobody likes you.",
                                            "Goodness you are\nhorrible at this.",
                                            "I repeat my lines\nbecause you're stupid.",
                                            "There's no hope for\nan idiot like you.",
                                            "Just another idiot\ntrying to challenge me...\nJust give up, for\nI am the ALMIGHTY Tet!",
                                            "My dog wouldn't let\nyou pet it. Why? Because\nyou smell and are ugly.",
                                            "You're face had to\nhave been someone's\nthirteenth reason.",
                                            "Shrek would beat you\nin a beauty pagent.",
                                            "Oh, you definitely\nshould not have done\nthat.",
                                            "I am the greatest!\nla, la, la.\nI am the best!\nla, la, la.",
                                            "When I looked up the\nword unsightly in the\ndictionary, they gave a\npicture of your face\nas an example.",
                                            "If you didn't know,\nI love watching\nothers fail, so thank you\nfor coming!",
                                            "If you go to the\ngym, quit... It's\nclearly not working.",
                                            "That was definitely\nthe wrong move, but\nwhat more could I\nexpect?",
                                            "People say I'm the\nmost handsome person\nalive. Such modest\ncompliments.",
                                            "I thought you should\nknow that I have an IQ of\n378. I think the test\nunderestimated my true\nintelligence.",
                                            "They say Rome wasn't\nbuilt in a day.\nI could do it.",
                                            "The ladies all want\nme, as you can clearly\ntell.",
                                            "Albert Einstein said\nthis to me, \"You're\nintellect is endless in\ndepth and breadth\""};

    //struct that holds the dialogue that tet says when he does this and what he specifically does
    struct tetPower {
        std::string dialogue;
        std::string power;
    };

    void usePower(tetPower p); //function we call to realize the effect of the tetPower

    //defining our tetPowers
    tetPower lessMultiplier = {"Wow, you're score is so\nhigh. Let's slow down.", "less"};
    tetPower halfMultiplier = {"You're progressing too\nquickly for my liking.\nHalf multiplier!", "half"};
    tetPower negativeMultiplier = {"Mhhhh... That score is\ngetting too high for\nmy liking.", "negative"};
    tetPower increaseLevel1 = {"How about we spice\nthings up a little?", "level1"};
    tetPower increaseLevel2 = {"Okay, let's spice\nthings up a lot!", "level2"};
    tetPower blind1 = {"Would it be easier\nif you couldn't see?!", "blind1"};
    tetPower blind2 = {"You don't need to\nsee the blocks anyways\nright?", "blind2"};
    tetPower flip = {"TURN THE SCREEN!", "turn"};
    tetPower Zblock = {"Here are some amazing\nblocks to help you.", "zblock"};
    tetPower Sblock = {"Here are some amazing\nblocks to help you.", "sblock"};

    bool tetpowertoggle = false; //to determine if next dialogue should be a tetPower

    Sound debuff = LoadSound("resources/audio/tetDeBuff.wav"); //play this when player is debuffed by tet

    //2 vector<tetPower> inside a vector which represents the two stages, progressively gets better powers
    std::string currPower = "null";
    const std::vector<tetPower> tetPowers1 = {lessMultiplier, halfMultiplier, increaseLevel1, blind1, Zblock, Sblock};
    const std::vector<tetPower> tetPowers2 = {halfMultiplier, negativeMultiplier, increaseLevel2, blind2, flip, Zblock, Sblock};

    //for tet's face
    void tetBob(); int bobCounter = 0; double yvel = -0.25; bool bobWait = false;
    void tetTalk(bool wait); int tetCounter = 0; int tetCounterMax = GetRandomValue(3, 7); int facePhase = 0;
    bool spaceWait = false; //make sure tet's mouth is closed when there is a period.
    Rectangle source = (Rectangle) {0, 0, tdim, tdim};
    Rectangle dest = (Rectangle) {650, 350, tdim*3.5, tdim*3.5};
    Texture2D t = LoadTexture("resources/tet/mistaTet1.png");
    int tetStage = 1;

    //for tet talking
    std::vector<std::string> tetSounds = {"resources/audio/tetTalk1.wav", "resources/audio/tetTalk2.wav", "resources/audio/tetTalk3.wav", "resources/audio/tetTalk4.wav", "resources/audio/tetTalk5.wav", "resources/audio/tetTalk6.wav", "resources/audio/tetTalk7.wav"};
    int talk = 2;

    Score* src;
    Font tetFont = LoadFont("resources/allFont.ttf");
    int level;

};

#endif