#ifndef TET_H
#define TET_H

#define tdim 70

#include "score.h"
#include "tools.h"


//--------------------------------------------------------------------------
// handles all actions related to tet; animations, dialogue, changes, etc...
//--------------------------------------------------------------------------


class Tet {
    public:

        Tet(Score* s);

        //set level
        void updateLevel(int l) { level = l;}
        
        //set camera rotation
        void updateCamRotate(float r) {camRotation = r;}

        //draw mista tet
        void drawTet();

        //tet monologue
        int tetMonologue();

        //cutscene when you reach 90k score
        int tetCutscene();

        //check if a tet power is activated
        std::string checkTetPower();

        //tet will turn to the side and look at the given corrdinates, if x coordinate is negative, will reset face back to facing forward
        void look(Vector2 coor);

        //check if we should increase the tet stage
        void checkStage();

    private:

    //during the game for when tet is talking
    std::string tetText;                                    //current text that is being displayed
    int txtCounter = 0;                                     
    int txtCounterWait = 3;
    int txtTracker = 0;                                 
    int txtIndex;                                           //which index are we using right now
    std::vector<int> vals;                                  //which dialougues tet can say (to make sure two dont repeat back to back)
    int timebetweenText = GetRandomValue(1*60, 3*60);       //how long before each time tet talks
    int time = 0; int waitTime; bool wait = false;
    bool stop;
    bool finalStage = false, hugeFlip = false;


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
                                            "Albert Einstein said\nthis to me, \"You're\nintellect is endless in\ndepth and breadth\"."};

    //struct that holds the dialogue that tet says when he does this and what he specifically does
    struct tetPower {
        std::string dialogue;
        std::string power;
    };

    void usePower(tetPower p); //function we call to realize the effect of the tetPower

    //defining our tetPowers
    tetPower lessMultiplier = {"Wow, you're score is so\nhigh. Let's slow down.", "less"};                      //multiplies current multiplier by 0.7
    tetPower halfMultiplier = {"You're progressing too\nquickly for my liking.\nHalf multiplier!", "half"};     //multiplies current multiplier by 0.5
    tetPower negativeMultiplier = {"Mhhhh... That score is\ngetting too high for\nmy liking.", "negative"};     //multiplies current multiplier by -1
    tetPower increaseLevel1 = {"How about we spice\nthings up a little?", "level1"};                            //increases current level by 15
    tetPower increaseLevel2 = {"Okay, let's spice\nthings up a lot!", "level2"};                                //increases current level by 25
    tetPower blind1 = {"Would it be easier\nif you couldn't see?!", "blind1"};                                  //blinds random rows
    tetPower blind2 = {"You don't need to\nsee the blocks anyways\nright?", "blind2"};                          //blinds more random rows
    tetPower flip = {"FLIP THE SCREEN!", "turn"};                                                               //turns the screen 90, 180, or 270 degrees
    tetPower Zblock = {"Here are some amazing\nblocks to help you.", "zblock"};                                 //replaces next three blocks with z blocks
    tetPower Sblock = {"Here are some amazing\nblocks to help you.", "sblock"};                                 //replaces next three blocks with s blocks
    tetPower babies1 = {"PUNCH the baby, you\nevil degenerate!", "babies1"};
    tetPower babies2 = {"PUNCH the babies, you\nevil degenerate!", "babies2"};
    tetPower babies3 = {"PUNCH the babies, you\nevil degenerate!", "babies3"};
    tetPower flurry = {"AAAAAAAAAAAAAEEEEEHHH\nHHHHHHHHHHHHHHHHHH!", "flurry"};                                 //power at the start of final stage: extreme flip, giant block, and negative multiplier all at once

    bool tetpowertoggle = false;                                                                                //to determine if next dialogue should be a tetPower

    //for the babie head powerup
    std::tuple<int, int, int> babies = std::make_tuple(0, 0, 0);                    //first int is num of babies, second is score lost if no succeed, third is frames to do it, last is texture of babie
    std::vector<Vector2> positions;                                                 //positions of our baby heads
    Texture2D babyHead = LoadTexture("resources/tet/baby_head.png");                //normal baby head
    Texture2D redBabyHead = LoadTexture("resources/tet/baby_head_final.png");       //final stage baby head

    //2 vector<tetPower> inside a vector which represents the two stages, progressively gets better powers
    std::string currPower = "null";                                                                                                                 //current chosen power
    const std::vector<tetPower> tetPowers1 = {lessMultiplier, halfMultiplier, increaseLevel1, blind1, Zblock, Sblock, babies1};                     //stage 1 powers, starts at beginning
    const std::vector<tetPower> tetPowers2 = {halfMultiplier, negativeMultiplier, increaseLevel2, blind2, flip, Zblock, Sblock, babies2};           //stage 2 powers, when score is greater than 50,000
    const std::vector<tetPower> finalPowers = {flurry, halfMultiplier, babies3};

    //for tet's face
    void tetBob();                                                                      //for the constant up and down motion of the tet head
    int bobCounter = 0; double yvel = -0.25; bool bobWait = false;           
    int count = 0;         
    void tetTalk(bool wait);                                                            //for tet's mouth moving when talking
    int tetCounter = 0, tetCounterMax = GetRandomValue(3, 7), facePhase = 0;
    bool spaceWait = false;                                                             //make sure tet's mouth is closed when there is a period.
    Rectangle source = (Rectangle) {0, 0, tdim, tdim};                                  //source rectangle for mista tet structure
    Rectangle dest = (Rectangle) {650, 350, tdim*3.5, tdim*3.5};                        //destination rectabgle for mista tet texture
    Texture2D t = LoadTexture("resources/tet/tet_1.png");                               //mista tet texture
    float rotation = 0;                                                                 //current rotation of Tet's face
    int tetStage = 1;                                                                   //current stage out of 4, each stage has a different texture

    //resources for tet talking
    std::vector<std::string> tetSounds =                                                                //current pool of sounds we can use when tet is talking
                            {"resources/audio/tet_talk_1.mp3", "resources/audio/tet_talk_2.mp3", 
                            "resources/audio/tet_talk_3.mp3", "resources/audio/tet_talk_4.mp3", 
                            "resources/audio/tet_talk_5.mp3", "resources/audio/tet_talk_6.mp3", 
                            "resources/audio/tet_talk_7.mp3"};
    std::vector<std::string> tetSoundsOriginal =                                                        //original pool of sounds for refrence when we want to reset the pool
                            {"resources/audio/tet_talk_1.mp3", "resources/audio/tet_talk_2.mp3", 
                            "resources/audio/tet_talk_3.mp3", "resources/audio/tet_talk_4.mp3", 
                            "resources/audio/tet_talk_5.mp3", "resources/audio/tet_talk_6.mp3", 
                            "resources/audio/tet_talk_7.mp3"};
    int talk = 2;                                                                                       //wait counter for tet talking sound
    void drawTetText(std::string& s);                                                                   //function that puts tet's dialogue on correct position
    void tetFinalStage();                                                                               //brings tet into final stage, called after cutscene

    float camRotation = 0;                                          //current camera rotation
    Score* src;                                                     //score object pointer
    Font tetFont = LoadFont("resources/all_font.ttf");              //font used for text
    int level;                                                      //current level

};

#endif