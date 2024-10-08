#include "grid.h"



/*********************************************************************************************
***** START OF GRID CLASS 
*********************************************************************************************/


        //--------------------------------------------------------------------------
        // Constructor
        //--------------------------------------------------------------------------


Grid::Grid() {
    srand(time(NULL));
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = 0;
        }
    }

    //setting up our next array for upcoming blocks and giving a starting block
    for (int i = 0; i < 3; i++) next.push_back(randBlock.getBlock());
    block = Block(randBlock.getBlock(), grid);

    //initialize 2d camera
    cameraMain.target = (Vector2){400, 400};
    cameraMain.offset = (Vector2){400, 400};
    cameraMain.rotation = 0;
    cameraMain.zoom = 1;

    rotationIt = null.end();
}

Grid::~Grid() {
    MusicPlayer().reset();
    sound().reset();
    //unloading textures
    UnloadTexture(fog);
    UnloadFont(allFont);
}


        //--------------------------------------------------------------------------
        // Drawing Functions
        //--------------------------------------------------------------------------


void Grid::drawAll(bool drawBlock, bool useCamera) {
    if (useCamera) BeginMode2D(cameraMain);
        drawGrid(drawBlock);
        scr->drawScore();
        pow->drawPowerBoard();
        tet->drawTet();
        pow->drawPowerupItems();
    if (useCamera) EndMode2D();
}

void Grid::drawGrid(bool drawBlock) {
    Color DarkRed = {static_cast<unsigned char>(255-level*1.4), 0, 0, 255};
    float borderWidth = 10;
    Color borderColor = (Color){80, 60, 60, 255};

    ClearBackground((Color){static_cast<unsigned char>(0+level/2), 0, 0});

    //filling in 10x20 grid based on the 2d array
    for (int i = 1; i < 21; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] != 0) {
                Rectangle rec; rec.x = xpos+j*gridsize; rec.y = ypos+(i-1)*gridsize; rec.width = gridsize; rec.height = gridsize;
                DrawRectangleRec(rec, colors[grid[i][j]]);
                DrawRectangleLinesEx(rec, 2, Fade(BLACK, 0.2));
            }
        }
    }

    //drawing our next blocks board
    float height = 250; float width = 120;
    DrawRectangleRoundedLines((Rectangle){30, 90, width+10, height+10}, 0.2, 100, 5, borderColor);
    DrawTextPro(allFont, "next", (Vector2){35+width/2, 100+MeasureTextEx(allFont, "next", 35, 3).y/2}, (Vector2){MeasureTextEx(allFont, "next", 35, 3).x/2, MeasureTextEx(allFont, "next", 35, 3).y/2}, 0, 35, 3, DarkRed);
    DrawTextPro(allFont, "next", (Vector2){35+width/2, 100+MeasureTextEx(allFont, "next", 35, 3).y/2}, (Vector2){MeasureTextEx(allFont, "next", 35, 3).x/2, MeasureTextEx(allFont, "next", 35, 3).y/2}, 0, 35, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    
    for (int i = 0; i < 3; i++) {
        int side = 20;
        if (next[i] == 1) {
            Rectangle rec; rec.x = 55; rec.y = 155+(i*65), rec.width = side; rec.height = side;
            DrawRectangleRec(rec, colors[1]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            for (int i = 0; i < 3; i++) {
                rec.x+= side;
                DrawRectangleRec(rec, colors[1]); 
                DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            }
        }
        else if (next[i] == 2) {
            Rectangle rec; rec.x = 65; rec.y = 150+(i*65); rec.width = side; rec.height = side;
            DrawRectangleRec(rec, colors[2]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.y+=side; DrawRectangleRec(rec, colors[2]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[2]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[2]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        }
        else if (next[i] == 3) {
            Rectangle rec; rec.x = 65; rec.y = 170+(i*65); rec.width = side; rec.height = side;
            DrawRectangleRec(rec, colors[3]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[3]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[3]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.y-=side; DrawRectangleRec(rec, colors[3]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));

        }
        else if (next[i] == 4) {
            Rectangle rec; rec.x = 75; rec.y = 150+(i*65); rec.width = side; rec.height = side;
            DrawRectangleRec(rec, colors[4]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[4]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x-=side; rec.y+=side; DrawRectangleRec(rec, colors[4]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[4]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));

        }
        else if (next[i] == 5) {
            Rectangle rec; rec.x = 65; rec.y = 170+(i*65); rec.width = side; rec.height = side;
            DrawRectangleRec(rec, colors[5]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[5]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.y-=side; DrawRectangleRec(rec, colors[5]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[5]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        }
        else if (next[i] == 6) {
            Rectangle rec; rec.x = 65; rec.y = 170+(i*65); rec.width = side; rec.height = side;
            DrawRectangleRec(rec, colors[6]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[6]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.y-=side; DrawRectangleRec(rec, colors[6]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.y+=side; rec.x+=side; DrawRectangleRec(rec, colors[6]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        }
        else {
            Rectangle rec; rec.x = 65; rec.y = 150+(i*65); rec.width = side; rec.height = side;
            DrawRectangleRec(rec, colors[7]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[7]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.y+=side; DrawRectangleRec(rec, colors[7]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
            rec.x+=side; DrawRectangleRec(rec, colors[7]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        }
    }

    //drawing level board
    height = 100;
    std::string l = "";
    if (std::to_string(level).length() == 1) l+='0';
    DrawRectangleRoundedLines((Rectangle){30, 625, width+10, 90}, 0.2, 100, 5, borderColor);
    l+= std::to_string(level);
    DrawTextPro(allFont, "level", (Vector2){35+width/2, 650}, (Vector2){MeasureTextEx(allFont, "level", 35, 3).x/2, MeasureTextEx(allFont, "level", 35, 3).y/2}, 0, 35, 3, DarkRed);
    DrawTextPro(allFont, "level", (Vector2){35+width/2, 650}, (Vector2){MeasureTextEx(allFont, "level", 35, 3).x/2, MeasureTextEx(allFont, "level", 35, 3).y/2}, 0, 35, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    DrawTextPro(allFont, l.c_str(), (Vector2){35+width/2, 690}, (Vector2){MeasureTextEx(allFont, l.c_str(), 30, 3).x/2, MeasureTextEx(allFont, l.c_str(), 30, 3).y/2}, 0, 30, 3, DarkRed);
    DrawTextPro(allFont, l.c_str(), (Vector2){35+width/2, 690}, (Vector2){MeasureTextEx(allFont, l.c_str(), 30, 3).x/2, MeasureTextEx(allFont, l.c_str(), 30, 3).y/2}, 0, 30, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));

    //drawing hold board
    int side = 30;
    int y = 480;
    int x = 50;

    DrawRectangleRoundedLines((Rectangle){20, 400, width+30, height+70}, 0.2, 100, 5, borderColor);
    DrawTextPro(allFont, "hold", (Vector2){35+width/2, 410+MeasureTextEx(allFont, "hold", 35, 3).y/2}, (Vector2){MeasureTextEx(allFont, "next", 35, 3).x/2, MeasureTextEx(allFont, "next", 35, 3).y/2}, 0, 35, 3, DarkRed);
    DrawTextPro(allFont, "hold", (Vector2){35+width/2, 410+MeasureTextEx(allFont, "hold", 35, 3).y/2}, (Vector2){MeasureTextEx(allFont, "next", 35, 3).x/2, MeasureTextEx(allFont, "next", 35, 3).y/2}, 0, 35, 3, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    
    if (held == 1) {
        Rectangle rec; rec.x = x-15; rec.y = y+5; rec.width = side; rec.height = side;
        DrawRectangleRec(rec, colors[1]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        for (int i = 0; i < 3; i++) {
            rec.x+= side;
            DrawRectangleRec(rec, colors[1]); 
            DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        }
    }
    else if (held == 2) {
        Rectangle rec; rec.x = x; rec.y = y; rec.width = side; rec.height = side;
        DrawRectangleRec(rec, colors[2]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.y+=side; DrawRectangleRec(rec, colors[2]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[2]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[2]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
    }
    else if (held == 3) {
        Rectangle rec; rec.x = x; rec.y = y+20; rec.width = side; rec.height = side;
        DrawRectangleRec(rec, colors[3]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[3]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[3]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.y-=side; DrawRectangleRec(rec, colors[3]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));

    }
    else if (held == 4) {
        Rectangle rec; rec.x = x+15; rec.y = y; rec.width = side; rec.height = side;
        DrawRectangleRec(rec, colors[4]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[4]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x-=side; rec.y+=side; DrawRectangleRec(rec, colors[4]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[4]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));

    }
    else if (held == 5) {
        Rectangle rec; rec.x = x; rec.y = y+20; rec.width = side; rec.height = side;
        DrawRectangleRec(rec, colors[5]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[5]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.y-=side; DrawRectangleRec(rec, colors[5]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[5]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
    }
    else if (held == 6) {
        Rectangle rec; rec.x = x; rec.y = y+20; rec.width = side; rec.height = side;
        DrawRectangleRec(rec, colors[6]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[6]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.y-=side; DrawRectangleRec(rec, colors[6]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.y+=side; rec.x+=side; DrawRectangleRec(rec, colors[6]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
    }
    else if (held == 7) {
        Rectangle rec; rec.x = x; rec.y = y; rec.width = side; rec.height = side;
        DrawRectangleRec(rec, colors[7]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[7]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.y+=side; DrawRectangleRec(rec, colors[7]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
        rec.x+=side; DrawRectangleRec(rec, colors[7]); DrawRectangleLinesEx(rec, 3, Fade(BLACK, 0.2));
    }


    //drawing our block
    if (drawBlock) {
        for (int i = 0; i < 4; i++) {
            if (block.getPos()[i].second != 0) {
                Rectangle rec; rec.x = xpos+block.getPos()[i].first*gridsize; rec.y = ypos+(block.getPos()[i].second-1)*gridsize;
                rec.height = gridsize; rec.width = gridsize; 
                DrawRectangleRec(rec, colors[block.getId()]);
                DrawRectangleLinesEx(rec, 2, Fade(BLACK, 0.2));
            }
        }
        int dist = 0;

        //finds out how much distance between block and the ground or another block below it to figure out where to draw the
        //the ghost block that shows where block is going
        while (true) {
            bool leave = false;
            for (int i = 0; i < 4; i++) {
                if (block.getPos()[i].second+dist == 20 || grid[block.getPos()[i].second+dist+1][block.getPos()[i].first] != 0) {
                    leave = true;
                    break;
                }
            }
            if (leave) break;
            else dist++;
        }
        for (int i = 0; i < 4; i++) {
            if (block.getPos()[i].second+dist != 0) {
                Rectangle rec; rec.x = xpos+block.getPos()[i].first*gridsize; rec.y = ypos+(block.getPos()[i].second+dist-1)*gridsize;
                rec.height = gridsize; rec.width = gridsize;
                DrawRectangleRec(rec, Fade(colors[block.getId()], 0.5));
                DrawRectangleLinesEx(rec, 2, Fade(BLACK, 0.1));
            }
        }
    }

    //blind tet power
    for (std::vector<std::tuple<int, int, double>>::iterator it = blindRows.begin(); it != blindRows.end(); it++) {
        for (int i = 0; i < 10; i++) {
            DrawTexturePro(fog, {0, 0, 30, 30}, {float(xpos) + 30*i, float(ypos) + 30*(std::get<0>(*it)-1), 30, 30}, {0, 0}, 0, Fade(WHITE, std::get<2>(*it)));
        }
        //checking if time is up otherwise decrement time
        if (std::get<1>(*it) == 0) {
            std::get<2>(*it)-=0.015;
            //if time is up and fade is less than 0 then this row's blind is over
            if (std::get<2>(*it) < 0) {
                blindRows.erase(it);
                it--;
            }
        }
        //time is not up so need to check if we need to fade in the texture
        else {
            std::get<1>(*it)--;
            if (std::get<2>(*it) < 1) {
                std::get<2>(*it)+=0.015;
            }
        }
    }
    //grid border
    DrawRectangleRoundedLines((Rectangle){static_cast<float>(xpos), ypos-borderWidth, 10*gridsize, 20*gridsize+borderWidth}, 0.1, 100, borderWidth, borderColor);

    //outer border
    float bordwidth = 5;
    DrawRectangleLinesEx({0-bordwidth, 0-bordwidth, 800 + 2*bordwidth, 800 + 2*bordwidth}, bordwidth, borderColor);
}


        //--------------------------------------------------------------------------
        // Updating Functions
        //--------------------------------------------------------------------------


void Grid::updateAll() {
    updatelevel();
    updateCamera();
    scr->updateMultiplier();
    pow->updateRotation(cameraMain.rotation, angVel, subtract);
    tet->updateCamRotate(cameraMain.rotation);
    scr->updateLevel(level);
    pow->updateLevel(level);
    tet->checkStage();
    PowerupItem::cameraRotation = cameraMain.rotation;
    PowerupItem::rotationVel = subtract ? -angVel: angVel;
    MusicPlayer().updateMusic();
    sound().updateSound();
    timer++;
}

void Grid::updateCamera() {
    if (tet->flyShake) tetFlyCamUpdate();

    if (rotationIt == null.end()) {
        bool cont = false;
        for (rotationIt = rotations.begin(); rotationIt != rotations.end(); rotationIt++) {
            if (std::get<2>(*rotationIt) || std::get<1>(*rotationIt) == 0) {
                cont = true;
                break;
            }
            else --std::get<1>(*rotationIt);
        }
        if (!cont) {
            rotationIt = null.end();
            return;
        }
    }
    if (std::get<1>(*rotationIt) == 0) {
        subtract = true;
    }
    if (!subtract && int(cameraMain.rotation)%90 > 88 && int(cameraMain.rotation+angVel)%90 < 2 ) {
        if (cameraMain.rotation < 90) cameraMain.rotation = 90;
        else if (cameraMain.rotation < 180) cameraMain.rotation = 180;
        else if (cameraMain.rotation < 270) cameraMain.rotation = 270;
        else cameraMain.rotation = 0;
        shake = true;
        angVel = 0;
        maxOffsetRotation = 15;
        numRotations++;
    }
    else if (subtract && int(cameraMain.rotation)%90 < 2 && cameraMain.rotation != 0 && cameraMain.rotation != 90 && cameraMain.rotation != 180 && cameraMain.rotation != 270 && int(cameraMain.rotation+90-angVel)%90 > 88) {
        if (cameraMain.rotation > 270) cameraMain.rotation = 270;
        else if (cameraMain.rotation > 180) cameraMain.rotation = 180;
        else if (cameraMain.rotation > 90) cameraMain.rotation = 90;
        else cameraMain.rotation = 0;
        shake = true;
        angVel = 0;
        maxOffsetRotation = 15;
        numRotations++;
    }
    else if (shake) {
        if (maxOffsetRotation == 15) sound().play("resources/audio/rotation_rumble.mp3");
        cameraMain.offset.x = 400 + maxOffsetRotation*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
        cameraMain.offset.y = 400 + maxOffsetRotation*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
        maxOffsetRotation-=0.1875;
        if (maxOffsetRotation <= 0) {
            shake = false;
            cameraMain.offset.x = 400;
            cameraMain.offset.y = 400;
             if (std::get<0>(*rotationIt) == numRotations) {
                numRotations = 0;
                std::get<2>(*rotationIt) = false;
                if (subtract) {
                    subtract = false;
                    rotations.erase(rotationIt);
                }
                rotationIt = null.end();
                return;
            }
        }
    }
    else {
        if (cameraMain.rotation == 0 || cameraMain.rotation == 90 || cameraMain.rotation == 180 || cameraMain.rotation == 270) sound().play("resources/audio/screen_turn.mp3");
        if (subtract) {
            cameraMain.rotation-=angVel;
            if (cameraMain.rotation < 0) cameraMain.rotation+=360;
        }
        else {
            cameraMain.rotation+= angVel;
        }
        cameraMain.offset.x = 400 + 3*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
        cameraMain.offset.y = 400 + 3*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
        if (angVel < 1) angVel+=angAcc;
        else angVel = 1;
    }
}

void Grid::tetFlyCamUpdate() {
    cameraMain.offset.x = 400 + maxOffsetFly*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
    cameraMain.offset.y = 400 + maxOffsetFly*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
    if (maxOffsetFly > 0) maxOffsetFly-=0.15;
    else {
        maxOffsetFly = 5;
        tet->flyShake = false;
    }
}

int Grid::finalStage() {
    //resetting rotation and rotation powerups
    cameraMain.rotation = 0;
    subtract = false;
    shake = false;
    angVel = 0;
    numRotations = 0;
    rotations.clear();
    rotationIt = null.end();

    //resetting camera
    cameraMain.offset.x = 400;
    cameraMain.offset.y = 400;

    //resetting grid
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = 0;
        }
    }

    //resetting block
    block = Block(block.getId(), grid);

    //calling final stage for tet class
    tet->tetFinalStage();

    //variables
    int counter = 0;
    bool wait = true;

    //camera
    Camera2D camera;
    float maxAngle = 0.25;
    float maxOffset = 2;

    //initialize 2d camera
    camera.target = (Vector2){400, 400};
    camera.offset = (Vector2){400, 400};
    camera.rotation = 0;
    camera.zoom = 1;

    //giant gross block comes out
    while(!WindowShouldClose()) {
        updateAll();

        if (!wait) {            //making the block start from center and grow
            if (counter == 0) {
                grid[17][4] = GetRandomValue(1, 7);
                grid[17][5] = GetRandomValue(1, 7);
                grid[16][4] = GetRandomValue(1, 7);
                grid[16][5] = GetRandomValue(1, 7);
            }
            else if (counter == 30) {
                MusicPlayer().play("resources/music/final_battle_music.mp3");
                for (int i = 3; i < 7; i++) {
                    grid[18][i] = GetRandomValue(1, 7);
                    grid[15][i] = GetRandomValue(1, 7);
                }
                grid[17][3] = GetRandomValue(1, 7);
                grid[17][6] = GetRandomValue(1, 7);
                grid[16][3] = GetRandomValue(1, 7);
                grid[16][6] = GetRandomValue(1, 7);
            }
            else if (counter == 60) {
                grid[14][3] = GetRandomValue(1, 7);
                grid[14][5] = GetRandomValue(1, 7);
                grid[14][6] = GetRandomValue(1, 7);
                grid[18][2] = GetRandomValue(1, 7);
                grid[16][2] = GetRandomValue(1, 7);
                grid[19][4] = GetRandomValue(1, 7);
                grid[19][6] = GetRandomValue(1, 7);
                grid[18][7] = GetRandomValue(1, 7);
                grid[16][7] = GetRandomValue(1, 7);
            }
            else if (counter == 120) {
                grid[20][4] = GetRandomValue(1, 7);
                grid[20][6] = GetRandomValue(1, 7);
                grid[19][2] = GetRandomValue(1, 7);
                grid[19][7] = GetRandomValue(1, 7);
                grid[16][1] = GetRandomValue(1, 7);
                grid[16][8] = GetRandomValue(1, 7);
                grid[14][2] = GetRandomValue(1, 7);
                grid[14][7] = GetRandomValue(1, 7);
                grid[13][3] = GetRandomValue(1, 7);
                grid[13][6] = GetRandomValue(1, 7);
            }
            else if (counter == 170) {
                grid[20][3] = GetRandomValue(1, 7);
                grid[19][1] = GetRandomValue(1, 7);
                grid[19][8] = GetRandomValue(1, 7);
                grid[17][1] = GetRandomValue(1, 7);
                grid[17][8] = GetRandomValue(1, 7);
                grid[14][1] = GetRandomValue(1, 7);
                grid[14][8] = GetRandomValue(1, 7);
                grid[12][6] = GetRandomValue(1, 7);
            }
            else if (counter == 220) {
                grid[20][1] = GetRandomValue(1, 7);
                grid[20][8] = GetRandomValue(1, 7);
                grid[17][0] = GetRandomValue(1, 7);
                grid[17][9] = GetRandomValue(1, 7);
                grid[13][1] = GetRandomValue(1, 7);
                grid[12][3] = GetRandomValue(1, 7);
                grid[12][7] = GetRandomValue(1, 7);
            }
            else if (counter == 270) {
                grid[20][0] = GetRandomValue(1, 7);
                grid[20][9] = GetRandomValue(1, 7);
                grid[18][0] = GetRandomValue(1, 7);
                grid[13][0] = GetRandomValue(1, 7);
                grid[12][1] = GetRandomValue(1, 7);
                grid[12][8] = GetRandomValue(1, 7);
                grid[11][3] = GetRandomValue(1, 7);
            }
            else if (counter == 320) {
                grid[14][9] = GetRandomValue(1, 7);
                grid[12][9] = GetRandomValue(1, 7);
                grid[11][1] = GetRandomValue(1, 7);
                grid[11][4] = GetRandomValue(1, 7);
                grid[11][8] = GetRandomValue(1, 7);
                grid[10][3] = GetRandomValue(1, 7);
            }

            else if (counter == 360) return 0;
            counter++;
            //camera shake, build it up then dies out
            if (counter <= 200) {
                if (maxOffset >= 20) maxOffset = 12;
                else maxOffset+=0.05;
                if (maxAngle >= 2) maxAngle = 1.5;
                else maxAngle += 0.007;
            }
            else {
                if (maxOffset <= 3) maxOffset = 3;
                else maxOffset-=0.1;
                if (maxAngle <= 0.1) maxAngle-= 0.002;
            }
            camera.offset.x = 400 + maxOffset*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
            camera.offset.y = 400 + maxOffset*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
            camera.rotation = maxAngle*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
        }
        else {                  //waiting a little in the beginning
            counter++;
            if (counter == 150) {
                wait = false;
                counter = 0;
            }
        }
        BeginDrawing();
            BeginMode2D(camera);
                drawAll(false, false);
            EndMode2D();
        EndDrawing();
    }
    return -1;
}

void Grid::updatelevel() {
     if (rawLevel < int(scr->getScore()/2000)+1) rawLevel = int(scr->getScore()/2000)+1;
    level = rawLevel;
    for (std::vector<std::pair<int, int>>::iterator it = changeLevel.begin(); it != changeLevel.end(); it++) {
        level+=it->first;
        if (level > 99) level = 99;
        it->second--;
        if (it->second == 0) {
            changeLevel.erase(it);
            it--;
        }
    }
}

std::string Grid::getFinalTime() {
    int time = timer/60;
    std::string minutes = std::to_string(time/60);
    if (minutes.length() == 1) minutes = "0" + minutes;
    std::string seconds = std::to_string(time%60);
    if (seconds.length() == 1) seconds = "0" + seconds;
    return minutes + ":" + seconds;
}


        //--------------------------------------------------------------------------
        // Block Related Functions
        //--------------------------------------------------------------------------


void Grid::moveDown() {
    scr->addScore(2);
    if (!block.moveDown(grid)) {
        this->placeBlock();
        this->generateBlock();
    }
}

void Grid::moveRight() {
    block.moveRight(grid);
}

void Grid::moveLeft() {
    block.moveLeft(grid);
}

void Grid::rotate() {
    block.rotate(grid);
}

void Grid::drop() {
    sound().play("resources/audio/block_dropping.mp3");
    scr->addScore(4*block.drop(grid)); 
    this->placeBlock();
    this->generateBlock();
}

void Grid::hold() {
    if (ableToHold) {
        int temp = block.getId();
        if (held != 0) {
            block = Block(held, grid);
        }else {
            block = Block(next.front(), grid);
            next.pop_front();
            next.push_back(randBlock.getBlock());
        }
        held = temp;
        ableToHold = false;
    }
}

void Grid::placeBlock() {
    const std::pair<int, int>* pos = block.getPos();
    for (int i = 0; i < 4; i++) grid[pos[i].second][pos[i].first] = block.getId();
}

void Grid::generateBlock() {
    block = Block(next.front(), grid);
    const std::pair<int, int>* pos = block.getPos();
    for (int i = 0; i < 4; i++) if (grid[pos[i].second][pos[i].first] != 0) gameover = true;
    next.pop_front();
    next.push_back(randBlock.getBlock());
    ableToHold = true;
    justGenerated = true;
}

bool Grid::blockSpawned() {
    if (justGenerated) {
        justGenerated = false;
        return true;
    }
    return false;
}

const std::vector<int> Grid::checkRowComplete() const{
    std::vector<int> rows;
    for (int i = 1; i < 21; i++) {
        bool addRow = true;
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == 0) {
                addRow = false;
                break;
            }
        }
        if (addRow) rows.push_back(i);
    }
    return rows;
}


        //--------------------------------------------------------------------------
        // Row Related Functions
        //--------------------------------------------------------------------------


int Grid::removeRow(std::vector<int>& rows) {
    std::vector<int*> cells;
    bool first = true;
    //adds every cell that we need to remove as an int* in the correct order
    for (int r: rows) {
        for (int i = 0; i < 10; i++) {
            grid[r][i] = 8;
            cells.push_back(&grid[r][i]);
        }
    }
    tet->look({xpos+5*gridsize, ypos+rows[0]*gridsize-15});
    std::vector<int*>::iterator it = cells.begin();
    int counter = 0;
    while(!WindowShouldClose()) {
        updateAll();
        //this is removing each square and delaying it a little bit
         if (counter == 1 && !first && it != cells.end()) {
            sound().play("resources/audio/row_completion.mp3");
            int indx = it-cells.begin();
            tet->look({xpos+gridsize*5, ypos+rows[indx/10]*gridsize-gridsize/2});
            **it = 0;
            it++;
            counter = 0;
        } else if (!first) {
            counter++;
        }  
        
        //checking if we have removed everything
        if (it == cells.end()) {
            counter = 0;
            while (!WindowShouldClose()) {
                updateAll();
                if (counter == 10) return 0;
                else counter++;
                BeginDrawing();
                    drawAll(false, true);
                EndDrawing();
            }
            return -1;
        }

        //adding a little delay in the very beginning before any squares are removed
        if (first && counter == 10) {
            first = false;
            counter = 0;
        }
        else if (first) {
            counter++;
        }

        BeginDrawing();
            drawAll(false, true);
        EndDrawing();
    }
    return -1;
}

int Grid::fixRows(std::vector<int> rows, bool showBlock, bool addScore) {

    /*****
     * The logic is that we start from the lowest row removed (which is the last element of the removed rows vector), then 
     * iterate through the removed rows vector, in order to find from which row we need to move all our tetris pieces down.
     * However, this program also takes into account if the removed rows are not all together. For example, if rows 19 and 17
     * were removed, then the program would first find row 18 as the start and move everything above row 18 down a distance of one.
     * Then it would find row 17 (17 becuase we moved everything down one already) as a new starting 
     * point (there is still a gap at row 18). Finally, it would fill this gap.
    *****/

    std::queue<int> removedRows;
    for (int i = rows.size()-1; i >= 0; i--) removedRows.push(rows[i]);

    int distance = 0;
    int row;
    int counter = 0;

    while(!WindowShouldClose()) {
        updateAll();
        //if our distance is 0 then either we are done or we need to move on to the next row in removedRows
        if (counter == 10) {
            if (distance == 0) {
                //initial check to see if any fixing is actually required
                bool leave = true;
                for (int i = 0; i < removedRows.size(); i++) {
                    for (int j = 0; j < 10; j++) {
                        if (grid[removedRows.front()-1][j] != 0) {
                            leave = false;
                            break;
                        }
                    }
                    removedRows.push(removedRows.front());
                    removedRows.pop();
                }
                if (leave) {
                    counter = 0;
                    if (addScore) {
                        if (rows.size() == 1) scr->addScore(200);
                        else if (rows.size() == 2) scr->addScore(350);
                        else if (rows.size() == 3) scr->addScore(700);
                        else scr->addScore(1500);
                    }
                    while (!WindowShouldClose()) {
                        updateAll();
                        if (counter == 15) {
                            tet->look({-1, 0});
                            return 0;
                        }
                        else counter++;
                        BeginDrawing();
                            drawAll(showBlock, true);
                        EndDrawing();
                    }
                    return -1;
                }

                //moving on to the next row and seeing if there are consecutive removed rows with said row
                row = removedRows.front();
                while (!removedRows.empty() && row == removedRows.front()) {
                    row = removedRows.front()-1;
                    removedRows.pop();
                    distance++;
                }
            }


            //removedRows needs the distance moved to be added to it
            for (int i = 0; i < removedRows.size(); i++) {
                removedRows.push(removedRows.front()+1);
                removedRows.pop();
            }

            //moving the grid
            for (int i = row; i >= 0; i--) for (int j = 0; j < 10; j++) grid[i+1][j] = grid[i][j];
            sound().play("resources/audio/block_dropping.mp3");
            distance--; row++; counter = 0;
            
        }
        else {
            counter++;
        }

        //drawing
        BeginDrawing();
            drawAll(showBlock, true);
        EndDrawing();

    }
    return -1;
}


        //--------------------------------------------------------------------------
        // Player and Tet Powerups
        //--------------------------------------------------------------------------


int Grid::lasers() {
    Rectangle laserHeadSource = {0, 60, 50, 50};
    Vector2 laserHeadOrigin = {25, 25};
    Rectangle laserBodySource = {0, 0, 50, 70};
    Vector2 laserBodyOrigin = {25, 35};
    Texture2D laser = LoadTexture("resources/powerup/laser.png");
    float fade = 0;
    int wait = 0;
    int counter = 0;

    //camera for screen shake
    Camera2D camera;
    float maxAngle = 3;
    float maxOffset = 30;

    //choosing three random rows
    std::vector<int> row;
    std::vector<int> nums;
    for (int i = 1; i < 21; i++) {
        nums.push_back(i);
    }
    for (int i = 0; i < 3; i++) {
        int randnum = GetRandomValue(0, nums.size()-1);
        std::vector<int>::iterator it = nums.begin() + randnum;
        row.push_back(nums[randnum]);
        nums.erase(it);
    }
    
    //finding number of blocks that were hit by laser
    int blocks = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[row[i]][j] != 0) {
                blocks++;
            }
        }
    }
    
    sound().play("resources/audio/laser_audio.mp3");
    while(!WindowShouldClose()) {
        updateAll();
        camera = cameraMain;
            if (wait < 90) {
                wait++;
                if (wait == 15) tet->look({xpos+5*gridsize, ypos+10*gridsize});
                BeginDrawing();
                    this->drawAll(true, true);
                EndDrawing();
                continue;
            }

            //stream of score being added
            if (counter%7 == 0) {
                scr->addScore(blocks);
            }
            counter++;

            camera.offset.x = 400 + maxOffset*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
            camera.offset.y = 400 + maxOffset*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
            camera.rotation = cameraMain.rotation + maxAngle*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
            fade+=0.008;
            if (fade >= 1.2) {

                //the more blocks you break, the higher score and greater multiplier buff
                this->scr->addMultiplier(1+blocks*0.04);

                //removing affected blocks
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 10; j++) {
                        if (grid[row[i]][j] != 0) {
                            grid[row[i]][j] = 0;
                        }
                    }
                }
                
                tet->look({-1, 0});
                UnloadTexture(laser);

                for (int i = 0; i < 3; i++) {
                    for (int j = i; j < 3; j++) {
                        if (row[j] < row[i]) {
                            int temp = row[i];
                            row[i] = row[j];
                            row[j] = temp;
                        }
                    }
                }
                fixRows(row, true, false);
                return 0;
            }

            if (maxOffset > 0.5) maxOffset-=0.3;
            if (maxAngle > 0.1) maxAngle-=0.05;

        BeginDrawing();
            BeginMode2D(camera);
                this->drawAll(true, false);
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 10; j++) {
                        if (grid[row[i]][j] != 0) DrawRectangle(200+30*j, 100+30*(row[i]-1), 30, 30, Fade(BLACK, fade));
                    }
                    DrawTexturePro(laser, laserBodySource, {490.f-25, 85.f+row[i]*30, 50, 70}, laserBodyOrigin, 270, WHITE);
                    for (int j = 1; j < 15; j++) DrawTexturePro(laser, laserBodySource, {490.f-j*60, 85.f+row[i]*30, 50, 70}, laserBodyOrigin, 270, WHITE);
                    DrawTexturePro(laser, laserHeadSource, {490, 85.f+row[i]*30, 50, 50}, laserHeadOrigin, 270, WHITE);
                }
            EndMode2D();
        EndDrawing();
    }
    return -1;
}

int Grid::bomb() {
    Texture2D target = LoadTexture("resources/powerup/target.png");
    Texture2D explosion = LoadTexture("resources/powerup/explosion.png");
    int counter1 = 0;
    int counter2 = 0;
    Rectangle pos = {350, 400, 100, 100};
    Rectangle explosionSource = {0, 0, 250, 275};
    double fade = 1;
    int explosionStage = 0;

    //camera for screen shake
    Camera2D camera;
    float maxAngle = 1.5;
    float maxOffset = 15;

    sound().play("resources/audio/ticking.mp3");
    while(!WindowShouldClose()) {
        updateAll();
        camera = cameraMain;
        if (counter1 <= 150) {
            if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && pos.x + 5 < 500) pos.x+=5;
            if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && pos.x - 5 > 200) pos.x-=5;
            if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && pos.y + 5 < 700) pos.y+=5;
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && pos.y - 5 > 100) pos.y-=5;
            if (counter1 >= 20) tet->look({pos.x, pos.y});
            BeginDrawing();
                BeginMode2D(camera);
                    this->drawAll(true, false);
                    DrawTexturePro(target, {0, 0, 1200, 1200}, pos, {50, 50}, 0, WHITE);
                EndMode2D();
            EndDrawing();
            counter1++;
            continue;
        }
        if (fade >= -0.2) {
            fade-=0.015;
            BeginDrawing();
                BeginMode2D(camera);
                    this->drawAll(true, false);
                    DrawTexturePro(target, {0, 0, 1200, 1200}, pos, {50, 50}, 0, Fade(WHITE, fade));
                EndMode2D();
            EndDrawing();
            continue;
        }
        if (counter2 < 160) {
            if (counter2 > 20) {
                camera.offset.x = 400 + maxOffset*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
                camera.offset.y = 400 + maxOffset*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
                camera.rotation = cameraMain.rotation + maxAngle*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
            }
            if (counter2 == 20) {
                sound().play("resources/audio/explosion.mp3");
            }
            if (counter2 % 20 == 0 && counter2 != 0) {
                if (counter2 == 80) {
                    explosionSource.x = 0;
                    explosionSource.y = 275;
                    int counter = 0;
                    //deleting the blocks that were touched by explosion and adding score
                    for (int i = 0; i < 21; i++) {
                        for (int j = 0; j < 10; j++) {
                            if (CheckCollisionPointCircle({200+ 15.0f + j*30, 100+ 15.0f + i*30}, {pos.x, pos.y}, 100)) {
                                if (grid[i][j] != 0) {
                                    grid[i][j] = 0;
                                    counter++;
                                }
                            }
                        }
                    }
                scr->addScore(counter*35);
                }
                else {
                    explosionSource.x+=250;
                }
            }
            if (counter2 > 100) {
                if (maxOffset > 1) maxOffset-=0.3;
                if (maxAngle > 0.5) maxAngle-=0.05;
            }
            BeginDrawing();
                BeginMode2D(camera);
                    this->drawAll(true, false);
                    DrawTexturePro(explosion, explosionSource, {pos.x, pos.y, 250, 225}, {125, 122}, 0, WHITE);
                EndMode2D();
            EndDrawing();
            counter2++;
            continue;
        }
        tet->look({-1, 0});
        UnloadTexture(target);
        UnloadTexture(explosion);
        return 0;

    }
    return -1;
}

int Grid::nuke() {
    Texture2D explosion = LoadTexture("resources/powerup/nuclear_explosion.png");
    int counter1 = 0;
    int counter2 = 0;
    Rectangle source = {0, 0, 150, 120};
    double fade1 = 1;
    double fade2 = 0;

    //camera for screen shake
    Camera2D camera;
    float maxAngle = 2;
    float maxOffset = 25;

    //for tet look
    float y = ypos+20*gridsize;

    while (!WindowShouldClose()) {
        updateAll();
        camera = cameraMain;
        if (counter1 < 180) {
            counter1++;
            if (counter1 == 30) sound().play("resources/audio/alarm.mp3");
            BeginDrawing();
                this->drawAll(true, true);
            EndDrawing();
            continue;
        }
        if (counter2 < 400) {
            if (counter2 == 0) tet->look({xpos+5*gridsize, y});
            if (counter2 == 60) sound().play("resources/audio/nuke_sound.mp3");
            if (counter2 % 20 == 0 && counter2 != 0 && counter2 < 180) {
                y-=30;
                tet->look({xpos+5*gridsize, y});
                source.x += 150;
            }
            if (counter2 > 200) fade1-=0.005;
            if (counter2 >= 60) {
                camera.offset.x = 400 + maxOffset*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
                camera.offset.y = 400 + maxOffset*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
                camera.rotation = cameraMain.rotation + maxAngle*(double(rand())/RAND_MAX)*(1-GetRandomValue(0, 1)*2);
            }
            if (counter2 >= 300) {
                if (maxOffset > 1) maxOffset-=0.4;
                if (maxAngle > 0.25) maxAngle-=0.02;
            }
            fade2+=0.005;
            BeginDrawing();
                BeginMode2D(camera);
                    this->drawAll(true, false);
                    for (int i = 1; i < 21; i++) for (int j = 0; j < 10; j++) DrawRectangle(200 + 30*j, 100 + 30 * (i-1), 30, 30, Fade(BLACK, fade2));
                    DrawTexturePro(explosion, source, {350, 700, 300, 500}, {150, 500}, 0, Fade(WHITE, fade1));
                EndMode2D();
            EndDrawing();
            counter2++;
            continue;
        }
        blindRows.clear();
        int gridCounter = 0;
        for (int i = 1; i < 21; i++) {
            for (int j = 0; j < 10; j++) {
                if (grid[i][j] != 0) gridCounter++;
                grid[i][j] = 0;
            }
        }
        tet->look({-1, 0});
        scr->addScore(gridCounter*15);
        UnloadTexture(explosion);
        return 0;
    }
    return -1;
}

void Grid::changeNext(int i) {
    for (int j = 0; j < 3; j++) {
        next.pop_front();
        next.push_back(i);
    }
}

void Grid::increaseLevel(int x) {
    changeLevel.push_back(std::make_pair(x, 1800));
}

void Grid::blind(int num) {
    std::vector<int> rows;
    for (int i = 1; i <= 20; i++) rows.push_back(i);
    for (int i = 0; i < num; i++) {
        std::vector<int>::iterator it = rows.begin()+GetRandomValue(0, rows.size()-1);
        blindRows.push_back(std::make_tuple(*it, 900, 0));
        rows.erase(it);
    }
}

void Grid::randomRotate() {
    rotations.push_back(std::make_tuple(GetRandomValue(3, 3), GetRandomValue(20*60, 40*60), true));
}

void Grid::extremeRandomRotate() {
    rotations.push_back(std::make_tuple(GetRandomValue(1, 1), GetRandomValue(20*60, 40*60), true));
}


/*********************************************************************************************
***** END OF GRID CLASS 
*********************************************************************************************/


/*********************************************************************************************
***** START OF BLOCK CLASS 
*********************************************************************************************/


        //--------------------------------------------------------------------------
        // Constructor
        //--------------------------------------------------------------------------

Grid::Block::Block(int id, int grid[21][10]): id(id), rotation(0) {

    //I-block
    if (id == 1) {
        pos[0] = std::make_pair(3, 2);
        pos[1] = std::make_pair(4, 2);
        pos[2] = std::make_pair(5, 2);
        pos[3] = std::make_pair(6, 2);
    }

    //J-block
    else if (id == 2) {
        pos[0] = std::make_pair(3, 1);
        pos[1] = std::make_pair(3, 2);
        pos[2] = std::make_pair(4, 2);
        pos[3] = std::make_pair(5, 2);

    }

    //L-block
    else if (id == 3) {
        pos[0] = std::make_pair(5, 1);
        pos[1] = std::make_pair(5, 2);
        pos[2] = std::make_pair(4, 2);
        pos[3] = std::make_pair(3, 2);
    }

    //O-block
    else if (id == 4) {
        pos[0] = std::make_pair(4, 1);
        pos[1] = std::make_pair(4, 2);
        pos[2] = std::make_pair(5, 2);
        pos[3] = std::make_pair(5, 1);
    }

    //S-block
    else if (id == 5) {
        pos[0] = std::make_pair(5, 1);
        pos[1] = std::make_pair(4, 1);
        pos[2] = std::make_pair(4, 2);
        pos[3] = std::make_pair(3, 2);
    }

    //T-block
    else if (id == 6) {
        pos[0] = std::make_pair(3, 2);
        pos[1] = std::make_pair(4, 2);
        pos[2] = std::make_pair(4, 1);
        pos[3] = std::make_pair(5, 2);
    }
    
    //Z-block
    else if (id == 7) {
        pos[0] = std::make_pair(4, 1);
        pos[1] = std::make_pair(5, 1);
        pos[2] = std::make_pair(5, 2);
        pos[3] = std::make_pair(6, 2);
    }

    //see if we need to put the block one higher
    if (checkVerticalCollision(grid)) for (int i = 0; i < 4; i++) pos[i].second--;
}


        //--------------------------------------------------------------------------
        // Block Movement Functions
        //--------------------------------------------------------------------------


bool Grid::Block::moveDown(const int grid[21][10]) {
    if (checkVerticalCollision(grid)) return false;
    sound().play("resources/audio/block_move.mp3");
    for (int i = 0; i < 4; i++) pos[i].second++;
    return true;
}

void Grid::Block::moveRight(const int grid[21][10]) {
    if (checkHorizontalCollision(1, grid)) return;
    sound().play("resources/audio/block_move.mp3");
    for (int i = 0; i < 4; i++) pos[i].first++;
}

void Grid::Block::moveLeft(const int grid[21][10]) {
    if (checkHorizontalCollision(0, grid)) return;
    sound().play("resources/audio/block_move.mp3");
    for (int i = 0; i < 4; i++) pos[i].first--;
}

int Grid::Block::drop(const int grid[21][10]) {
    int counter = 0;
    while (!checkVerticalCollision(grid)) {
        for (int i = 0; i < 4; i++) pos[i].second++;
        counter++;
    }
    return counter;
}

void Grid::Block::rotate(const int grid[21][10]) {
    std::pair<int, int> backUpPos[4];
    for (int i = 0; i < 4; i++) {
        backUpPos[i] = pos[i];
    }

    //I-block rotation
    if (id == 1) {
        if (rotation == 0) {
            pos[0].first+=2;
            pos[0].second--;
            pos[1].first++;
            pos[2].second++;
            pos[3].first--;
            pos[3].second+=2;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].first-=2;
            pos[0].second+=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].first++;
            pos[0].second-=2;
            pos[1].second--;
            pos[2].first--;
            pos[3].first-=2;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }

        else if (rotation == 3) {
            pos[0].first--;
            pos[0].second++;
            pos[2].first++;
            pos[2].second--;
            pos[3].first+=2;
            pos[3].second-=2;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation = 0;
                return;
            }
        }
    }
    //J-block rotation
    else if (id == 2) {
        if (rotation == 0) {
            pos[0].first+=2;
            pos[1].first++;
            pos[1].second--;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].second+=2;
            pos[1].first++;
            pos[1].second++;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].first-=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].second-=2;
            pos[1].first--;
            pos[1].second--;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation = 0;
                return;
            }
        }
    }
    else if (id == 3) {
        if (rotation == 0) {
            pos[0].second+=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].first-=2;
            pos[1].first--;
            pos[1].second--;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].second-=2;
            pos[1].first++;
            pos[1].second--;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].first+=2;
            pos[1].first++;
            pos[1].second++;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation = 0;
                return;
            }
        }
    }
    else if (id == 5) {
        if (rotation == 0) {
            pos[0].second+=2;
            pos[1].first++;
            pos[1].second++;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].first-=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].second-=2;
            pos[1].first--;
            pos[1].second--;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].first+=2;
            pos[1].first++;
            pos[1].second--;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation = 0;
                return;
            }
        }
    }
    else if (id == 6) {
        if (rotation == 0) {
            pos[0].first++;
            pos[0].second--;
            pos[2].first++;
            pos[2].second++;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].first++;
            pos[0].second++;
            pos[2].first--;
            pos[2].second++;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].first--;
            pos[0].second++;
            pos[2].first--;
            pos[2].second--;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].first--;
            pos[0].second--;
            pos[2].first++;
            pos[2].second--;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation = 0;
                return;
            }
        }
    }
    else if (id == 7) {
        if (rotation == 0) {
            pos[0].first+=2;
            pos[1].first++;
            pos[1].second++;
            pos[3].first--;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 1) {
            pos[0].second+=2;
            pos[1].first--;
            pos[1].second++;
            pos[3].first--;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 2) {
            pos[0].first-=2;
            pos[1].first--;
            pos[1].second--;
            pos[3].first++;
            pos[3].second--;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation++;
                return;
            }
        }
        else if (rotation == 3) {
            pos[0].second-=2;
            pos[1].first++;
            pos[1].second--;
            pos[3].first++;
            pos[3].second++;
            if (checkRotationalCollision(pos, grid)) {
                sound().play("resources/audio/block_move.mp3");
                rotation = 0;
                return;
            }
        }
    }
    for (int i = 0; i < 4; i++) pos[i] = backUpPos[i];
}


        //--------------------------------------------------------------------------
        // Collision Checks
        //--------------------------------------------------------------------------


bool Grid::Block::checkHorizontalCollision(int side, const int grid[21][10]) const{
    if (side == 0) {
        for (int i = 0; i < 4; i++) if (pos[i].first == 0 || grid[pos[i].second][pos[i].first-1] != 0) return true;
        return false;
    }
    if (side == 1) {
        for (int i = 0; i < 4; i++) if (pos[i].first == 9 || grid[pos[i].second][pos[i].first+1] != 0) return true;
        return false;
    }
    return false;
}

bool Grid::Block::checkVerticalCollision(const int grid[20][10]) const {
    for (int i = 0; i < 4; i++) if (pos[i].second == 20 || grid[pos[i].second+1][pos[i].first] != 0) return true;
    return false;
}

bool Grid::Block::
checkRotationalCollision(std::pair<int, int>* position, const int grid[21][10]) {
    //translations that the function will try for a valid rotation
    std::vector<int> xVals = {1, 2, -1, -2};
    std::vector<int> yVals = {0, 0, 0, 0};

    std::pair<int, int> positionCheck[4];
    for (int i = 0; i < 4; i++) positionCheck[i] = position[i];

    //if there is a collision then proceed
    if (isRotationalCollision(position, grid)) {
        //go through xVals and yVals for different translations
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                positionCheck[j].first+=xVals[i];
                positionCheck[j].second+=yVals[i];
            }
            if (isRotationalCollision(positionCheck, grid)) {
                //revert back because this translation doesn't work
                for (int i = 0; i < 4; i++) positionCheck[i] = position[i];
            }
            else {
                //the first translation that works will be the one that we use
                for (int i = 0; i < 4; i++) position[i] = positionCheck[i];
                return true;
            }
        }
        return false;
    } 
    else return true;
}

bool Grid::Block::isRotationalCollision(const std::pair<int, int>* position, const int grid[21][10]) {
    for (int i = 0; i < 4; i++) {
        //checking for out of bounds and collision with another block
        if (position[i].first > 9 || position[i].first < 0 || position[i].second < 0 || position[i].second > 20 || grid[position[i].second][position[i].first] != 0) return true;
    }
    return false;
}


/*********************************************************************************************
***** END OF BLOCK CLASS 
*********************************************************************************************/
