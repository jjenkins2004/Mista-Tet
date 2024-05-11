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

        //draw mista tet
        void drawTet();
        //tet monologue
        int tetMonologue();


    private:
    Score* src;
    Texture2D tet1;
    Font tetFont;

};

#endif