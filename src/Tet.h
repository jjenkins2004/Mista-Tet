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


    private:

    //during the game for when tet is talking
    std::string tetText;
    int txtCounter = 0;
    int txtTracker = 0;
    int noEffect; //which index in noEffectText are we using right now
    std::vector<int> vals; //which dialougues tet can say (to make sure two dont repeat back to back)
    int timebetweenText = GetRandomValue(2*60, 3*60);
    int time = 0; int waitTime; bool wait;


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

    //defining our tetPowers
    tetPower lessMultiplier = {"Wow, you're score is so\nhigh. Let's slow down.", "less"};
    tetPower halfMultiplier = {"You're progressing too\nquickly for my liking.\nHalf multiplier!", "half"};
    tetPower negativeMultiplier = {"Mhhhh... That score is\ngetting too high for\nmy liking.", "negative"};
    tetPower increaseLevel1 = {"How about we spice\nthings up a little?", "level1"};
    tetPower increaseLevel2 = {"Okay, let's spice\nthings up a lot!", "level2"};
    tetPower topblind1 = {"Top fourth of the\nboard is now blocked.", "topblind1"};
    tetPower topblind2 = {"Top half of the\nboard is now blocked.", "topblind2"};
    tetPower bottomblind1 = {"Bottom fourth of the\nboard is now blocked.", "bottomblind1"};
    tetPower bottomblind2 = {"Bottom half of the\nboard is now blocked", "bottomblind2"};
    tetPower flip = {"TURN THE SCREEN!", "turn"};
    tetPower Zblock = {"Here are some amazing\nblocks to help you.", "zblock"};
    tetPower Sblock = {"Here are some amazing\nblocks to help you.", "sblock"};



    //2 vector<tetPower> inside a vector which represents the two stages, progressively gets better powers
    const std::vector<std::vector<tetPower>> tetPowers1 = { 
                                                        {lessMultiplier, halfMultiplier, increaseLevel1, topblind1, bottomblind1, Zblock, Sblock}, 
                                                        {halfMultiplier, negativeMultiplier, increaseLevel2, topblind2, bottomblind2, flip, Zblock, Sblock}};

    //for tet's face
    Rectangle source = (Rectangle) {0, 0, tdim, tdim};
    Rectangle dest = (Rectangle) {650, 350, tdim*3.5, tdim*3.5};
    Texture2D t = LoadTexture("resources/mistaTet1.png");
    int tetStage = 1;


    Score* src;
    Font tetFont = LoadFont("resources/allFont.ttf");
    int level;

};

#endif