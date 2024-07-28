#include "powerup.h"
#include <iomanip>

/*********************************************************************************************
***** START OF POWERUPITEM STRUCTS 
*********************************************************************************************/


        //--------------------------------------------------------------------------
        // PowerupItem
        //--------------------------------------------------------------------------


PowerupItem::PowerupItem(std::string i, Texture2D txt, int t): id(i), time(t) {
    if (i != "null") {
        pos.x = GetRandomValue(200, 600);
        pos.y = GetRandomValue(200, 600);
        texture = txt;
        int xrand = GetRandomValue(20, 50);
        vel.x = xrand - 2*xrand*GetRandomValue(0, 1);
        int yrand = GetRandomValue(10, 60);
        vel.y = yrand-2*yrand*GetRandomValue(0, 1);
        rotation.x = 0; rotation.y = GetRandomValue(-50, 50);
    }
}

PowerupItem::~PowerupItem() {
    if (id != "null") {
        UnloadTexture(texture);
    }
}

void PowerupItem::DrawItem(bool withRotation) {
    if (id != "null") {
        if (withRotation) {
            Vector2 position = getAdjustedCoordinates({pos.x, pos.y}, cameraRotation);
            DrawTexturePro(texture, (Rectangle){0, 0, 50, 50}, (Rectangle){position.x, position.y, 50, 50}, (Vector2){25, 25}, rotation.x-cameraRotation, Fade(WHITE, fade));
        }
        else {
            DrawTexturePro(texture, (Rectangle){0, 0, 50, 50}, (Rectangle){pos.x, pos.y, 50, 50}, (Vector2){25, 25}, rotation.x, Fade(WHITE, fade));
        }
    }
}

void PowerupItem::moveItem() {
    //air resistance
    float velMagnitude = sqrt(pow(vel.x, 2) + pow(vel.y, 2));
    Vector2 oppositeVel = {-vel.x, -vel.y};
    oppositeVel.x/=sqrt(pow(oppositeVel.x, 2) + pow(oppositeVel.y, 2));
    oppositeVel.y/=sqrt(pow(oppositeVel.x, 2) + pow(oppositeVel.y, 2));
    vel.x+=pow(velMagnitude, 2)*oppositeVel.x/1200;
    vel.y+=pow(velMagnitude, 2)*oppositeVel.y/1200;

    Vector2 oldVel = vel;

    //getting adjusted coordinates of the 4 out of bound corner borders
    std::vector<Vector2> borders = {getAdjustedCoordinates({0, 0}, 360-cameraRotation), getAdjustedCoordinates({800, 0}, 360-cameraRotation),
                                    getAdjustedCoordinates({800, 800}, 360-cameraRotation), getAdjustedCoordinates({0, 800}, 360-cameraRotation)};

    std::vector<std::pair<Vector2, Vector2>> collisions;                //using vector just in case of corner collision

    //checking if border was pushed into item and adjusting item so it is just touching border
    //this is necessary becuase later code assumes that item is not colliding with border before moving
    if (checkCollisionLineCircle(borders[0], borders[1], pos, 25)) collisions.push_back(std::make_pair(borders[0], borders[1]));
    if (checkCollisionLineCircle(borders[1], borders[2], pos, 25)) collisions.push_back(std::make_pair(borders[1], borders[2]));
    if (checkCollisionLineCircle(borders[2], borders[3], pos, 25)) collisions.push_back(std::make_pair(borders[2], borders[3]));
    if (checkCollisionLineCircle(borders[3], borders[0], pos, 25)) collisions.push_back(std::make_pair(borders[3], borders[0]));

    for (int i = 0; i < collisions.size(); i++) {
        std::pair<Vector2, Vector2> line = collisions[i];
        //finding perpendicular Vector to the border for later uses 
        v2 perpVec;
        if (line.first.y-line.second.y != 0 ) {                                             //check in case slope is 0
            perpVec = {1, -(line.first.x-line.second.x)/(line.first.y-line.second.y)};      //the perpendicular vector of the line
        }
        else perpVec = {0, 1};
        
        //making sure perpendicular vector is pointing correct way
        double dist1 = sqrt(pow(line.first.x+perpVec.x-400 , 2) + pow(line.first.y+perpVec.y-400, 2));
        double dist2 = sqrt(pow(line.first.x-perpVec.x-400 , 2) + pow(line.first.y-perpVec.y-400, 2));
        if (dist2 < dist1) {                //making sure perpendicular vector points inwards towards (400, 400) by comparing distances
            perpVec.x*=-1;                  //of some point on the border's line translated towards perpVec's direction with some point
            perpVec.y*=-1;                  //translated in negative direction. The point with the closer distance gives correct direction
        }

        //getting unit vector of perpVec
        float mag = sqrt(pow(perpVec.x, 2) + pow(perpVec.y, 2));
        v2 unitPerp = {(1/mag)*perpVec.x, (1/mag)*perpVec.y};

        //item pos will be 25 units from this point in perpVec direction
        Vector2 p = closestPointLineCircle(line.first, line.second, pos);

        //moving item
        pos = {p.x+float(25.01*unitPerp.x), p.y+float(25.01*unitPerp.y)};
    }
    collisions.clear();

    //moving
    Vector2 oldPos = pos;
    pos.x+= vel.x; pos.y+=vel.y + 1.f/2; vel.y+=1;
    rotation.x += rotation.y;

    float t;                    //time from initial to collision
    
    //checking for collisions against border
    if (checkCollisionLineCircle(borders[0], borders[1], pos, 25)) collisions.push_back(std::make_pair(borders[0], borders[1]));
    if (checkCollisionLineCircle(borders[1], borders[2], pos, 25)) collisions.push_back(std::make_pair(borders[1], borders[2]));
    if (checkCollisionLineCircle(borders[2], borders[3], pos, 25)) collisions.push_back(std::make_pair(borders[2], borders[3]));
    if (checkCollisionLineCircle(borders[3], borders[0], pos, 25)) collisions.push_back(std::make_pair(borders[3], borders[0]));
    if (collisions.empty()) return;

    pos.x-=vel.x; vel.y-= 1; pos.y-=vel.y + 1.f/2;                      //moving item back to original position before collision  


    for (int i = 0; i < collisions.size(); i++) {                       //we can do the exact same thing if we have a second border collision to simulate what would happen when hitting a corner
        std::pair<Vector2, Vector2> line = collisions[i];

        //finding perpendicular Vector to the border for later uses 
        Vector2 perpVec;
        if (line.first.y-line.second.y != 0 ) {                                             //check in case slope is 0
            perpVec = {1, -(line.first.x-line.second.x)/(line.first.y-line.second.y)};      //the perpendicular vector of the line
        }
        else perpVec = {0, 1};
        
        //making sure perpendicular vector is pointing correct way
        double dist1 = sqrt(pow(line.first.x+perpVec.x-400 , 2) + pow(line.first.y+perpVec.y-400, 2));
        double dist2 = sqrt(pow(line.first.x-perpVec.x-400 , 2) + pow(line.first.y-perpVec.y-400, 2));
        if (dist2 < dist1) {                //making sure perpendicular vector points inwards towards (400, 400) by comparing distances
            perpVec.x*=-1;                  //of some point on the border's line translated towards perpVec's direction with some point
            perpVec.y*=-1;                  //translated in negative direction. The point with the closer distance gives correct direction
        }

        //getting unit vector of perpVec
        float mag = sqrt(pow(perpVec.x, 2) + pow(perpVec.y, 2));
        Vector2 unitPerp = {(1/mag)*perpVec.x, (1/mag)*perpVec.y};

        //translating item following its current path so it is just touching the border
        Vector2 slope;
        if (line.first.x-line.second.x < 0.001 && line.first.x-line.second.x > -0.001) {                            //checking incase slope is undefined
            slope = {0, 1};
        }   
        else {
            slope = {1, (line.first.y-line.second.y)/(line.first.x-line.second.x)};                                 //finding slope of our border line
        }
        Vector2 start = {line.first.x + unitPerp.x*25, line.first.y + unitPerp.y*25};                               //some point on this line will be where the center of our item is as the item is colliding with the border
                                                                                                                    //line is parallel to border but translated 25 units in the perpendicular vector direction
        //use quadratic formula to find the time taken from initial point to when it is just about to collide
        //values for A, B, and C were derived using the intersection of a parabola vector equation to represent 
        //path of item and the above line
        if (i == 0 && slope.x != 0) {                       //formula doesn't work on vertical lines                  
            double A = 1.f/2;
            double B = vel.y-(slope.y*vel.x)/slope.x;
            double C = double(pos.y)-double(start.y)-(double(slope.y)/double(slope.x))*(double(pos.x)-double(start.x));
            float t1 = (-B + sqrt(B*B-4*A*C))/(2*A);
            float t2 = (-B - sqrt(B*B-4*A*C))/(2*A);
            t = t2 < 0 ? t1: t2;
            if (t < 0.001) t = 0;
            //FOR DEBUGGING
            // std::cout << std::fixed;
            // std::cout << std::setprecision(8);
            // std::cout << "\n\nvelocity: " << vel.x << " " << vel.y << std::endl;
            // std::cout << "start " << start.x << " " << start.y << ", slope " << slope.x << " " << slope.y << std::endl;
            // std::cout << "pos: " << pos.x << " " << pos.y << std::endl;
            // // std::cout << "A: " << A << " B: " << B << " C: " << C << std::endl;
            // std::cout << "t: " << t << std::endl;
            // std::cout << "camera rotation: " << cameraRotation << std::endl;
        }
        else if (i == 0) {
            t = (start.x-pos.x)/vel.x;
        }
        
        //use t to find new position and correct bounce off velocity
        Vector2 newPos = {pos.x + t*vel.x, pos.y+t*vel.y+t*t/2.f};
        vel = {vel.x, vel.y + t};

        //changing velocity so its relative to the border
        Vector2 p = closestPointLineCircle(line.first, line.second, newPos);
        Vector2 tranVel = {-1*rotationVel*(PI/180)*(p.y-400), rotationVel*(PI/180)*(p.x-400)};
        vel.x-=tranVel.x;
        vel.y-=tranVel.y;

        //changing direction of velocity using vector reflection formula relative to the moving border
        float dotProd = vel.x*unitPerp.x + vel.y*unitPerp.y;
        vel.x = vel.x-2*dotProd*unitPerp.x;
        vel.y = vel.y-2*dotProd*unitPerp.y;
        if (sqrt(vel.x*vel.x + vel.y*vel.y) != 0) {                                                     //if magnitude of velocity is 0 then there is no loss
            float loss = abs(dotProd/(sqrt(vel.x*vel.x + vel.y*vel.y)))*.1;
            vel.x*=(1-(loss)); vel.y*=(1-(loss*1.5));                                                   //the greater the change in velocity direction, the more energy it loses
        }

        //changing velocity back to background frame
        vel.x+=tranVel.x;
        vel.y+=tranVel.y;


        if (collisions.size() == 1 || i == 1) {
                //moving item amount it was supposed to move but couldn't due to collision
                pos = {newPos.x + vel.x*(1-t), newPos.y + vel.y*(1-t) + (1-t)*(1-t)/2};
                vel.y+=(1-t);

                //finding angular velocity based on the magnitude of the component of velocity parallel to the border
                float den = sqrt(pow(vel.x, 2) + pow(vel.y, 2));            
                float theta = PI/2-acos((vel.x*unitPerp.x + vel.y*unitPerp.y)/den);                         //finding angle between velocity and border
                float cross = unitPerp.x*vel.y-unitPerp.y*vel.x;                                            //use the cross product of velocity and unitPerp to determine direction of rotation, clockwise or counterclockwise
                float angvel = (180/PI)*cos(theta)*sqrt(pow(vel.x, 2) + pow(vel.y, 2))/25;                  //angular velocity in degrees
                if (cross < 0) rotation.y = -1*angvel;                                                      //velocity points to the right of perp vec so clockwise rotation
                else if (cross > 0) rotation.y = angvel;                                                    //velocity points left of perp vec so counterclockwise rotation
                else rotation.y = 0;
        }
    }
}
//gives a eerie animation when powerups are collected
void PowerupItem::spaz() {
    //checks if it is our first time calling spaz
    if (!spazzed) {
        sound().play("resources/audio/powerup_collection.mp3");
        spazzed = true;
        time = 45;
        vel.x = 1;
        vel.y = 1;
        spazpos.x = pos.x;
        spazpos.y = pos.y;
    }
    //if position goes some distance from the origin given by distFromOrigin, then reverse the velocity
    if (time == 0) {
        spazpos.x+=vel.x;
        spazpos.y+=vel.y;
        if (spazpos.x > (int)(pos.x)+distFromOrigin.x) {
            vel.x = vel.x*-1-GetRandomValue(1, 2);
            spazpos.x = pos.x+distFromOrigin.x-26;
            distFromOrigin.x+=2;
        }
        else if (spazpos.x < (int)(pos.x)-distFromOrigin.x) {
            vel.x = vel.x*-1+GetRandomValue(1, 2);
            spazpos.x = pos.x-distFromOrigin.x+26;
            distFromOrigin.x+=2;
        }
        if (spazpos.y > pos.y+distFromOrigin.y) {
            vel.y = vel.y*-1-GetRandomValue(-2, 2);
            spazpos.y = pos.y+distFromOrigin.y-26;
            distFromOrigin.y+=2;
        }
        else if (spazpos.y < pos.y-distFromOrigin.y) {
            vel.y = vel.y*-1+GetRandomValue(-2, 2);
            spazpos.y = pos.y-distFromOrigin.y+26;
            distFromOrigin.y+=2;
        }
        fade-=0.007;
    }
    //this is just for our iniitial pause before the spaz
    else {
        time--;
    }
    Vector2 position = getAdjustedCoordinates({spazpos.x, spazpos.y}, cameraRotation);
    DrawTexturePro(texture, (Rectangle){0, 0, 50, 50}, (Rectangle){position.x, position.y, 50, 50}, (Vector2){25, 25}, rotation.x-cameraRotation, Fade(WHITE, fade));
}

float PowerupItem::cameraRotation = 0;
float PowerupItem::rotationVel = 0;

        //--------------------------------------------------------------------------
        // Child structs
        //--------------------------------------------------------------------------
    

Multiplier::Multiplier(double m, int time, Texture2D texture): PowerupItem("multiplier", texture, time) {
    if (m < 1) positive = false;
    else positive = true;
    multiplier = m;
}

Laser::Laser(int time, Texture2D texture): PowerupItem("laser", texture, time) {}

Bomb::Bomb(int time, Texture2D texture): PowerupItem("bomb", texture, time) {}

Nuke::Nuke(int time, Texture2D texture): PowerupItem("nuke", texture, time) {}

ThreeBlock::ThreeBlock(int time, Texture2D texture, int id): PowerupItem("threeblock", texture, time), blockID(id) {}

PlusMultiplier::PlusMultiplier(double m, int time, Texture2D texture): PowerupItem("plusmultiplier", texture, time), multiplier(m) {}

FiveRandom::FiveRandom(int time, Texture2D texture): PowerupItem("fiverandom", texture, time) {}

SpeedChange::SpeedChange(int var, int time, Texture2D texture): PowerupItem("speedchange", texture, time), variant(var) {
    if (var > 0) positive = false;
    else positive = true;
}

Mystery::Mystery(int time, Texture2D texture): PowerupItem("mystery", texture, time) {}

void Mystery::collect() {
    //checks if it is the first time calling this function and sets mystery var to a random powerup
    if (mystery == nullptr) {
        int rand = GetRandomValue(1, 21);

        //setting our mystery powerupitem
        if (rand == 1) mystery = new Multiplier(2, 800, LoadTexture("resources/powerup/2_multiplier.png"));
        if (rand == 2) mystery = new Multiplier(1.5, 800, LoadTexture("resources/powerup/1_5_multiplier.png"));
        if (rand == 3) mystery = new Multiplier(1.2, 800, LoadTexture("resources/powerup/1_2_multiplier.png"));
        if (rand == 4) mystery = new Multiplier(0.7, 800, LoadTexture("resources/powerup/0_7_multiplier.png"));
        if (rand == 5) mystery = new Multiplier(-1, 800, LoadTexture("resources/powerup/negative_multiplier.png"));
        if (rand == 6) mystery = new ThreeBlock(1000, LoadTexture("resources/powerup/I_block.png"), 1);
        if (rand == 7) mystery = new ThreeBlock(1200, LoadTexture("resources/powerup/J_block.png"), 2);
        if (rand == 8) mystery = new ThreeBlock(1200, LoadTexture("resources/powerup/L_block.png"), 3);
        if (rand == 9) mystery = new ThreeBlock(1200, LoadTexture("resources/powerup/O_block.png"), 4);
        if (rand == 10) mystery = new ThreeBlock(1200, LoadTexture("resources/powerup/T_block.png"), 6);
        if (rand == 11) mystery = new Laser(1000, LoadTexture("resources/powerup/lasers.png"));
        if (rand == 12) mystery = new Bomb(1000, LoadTexture("resources/powerup/bomb.png"));
        if (rand == 13) mystery = new Nuke(800, LoadTexture("resources/powerup/nuke.png"));
        if (rand == 14) mystery = new PlusMultiplier(0.1, 1000, LoadTexture("resources/powerup/0_1_multiplier.png"));
        if (rand == 15) mystery = new PlusMultiplier(0.2, 800, LoadTexture("resources/powerup/0_5_multiplier.png"));
        if (rand == 16) mystery = new FiveRandom(1000, LoadTexture("resources/powerup/5_random.png"));
        if (rand == 17) mystery = new SpeedChange(0, 800, LoadTexture("resources/powerup/pause.png"));
        if (rand == 18) mystery = new SpeedChange(1, 800, LoadTexture("resources/powerup/fast_1.png"));
        if (rand == 19) mystery = new SpeedChange(2, 800, LoadTexture("resources/powerup/fast_2.png"));
        if (rand == 20) mystery = new SpeedChange(-1, 800, LoadTexture("resources/powerup/slow_1.png"));
        if (rand == 21) mystery = new SpeedChange(-2, 800, LoadTexture("resources/powerup/slow_2.png"));

        fade = 1;
        mystery->removed = true;
        mystery->fade = 0;
        mystery->pos = pos;
        sound().play("resources/audio/mystery_reveal.mp3");
    }

    //reduces opacity of the mystery powerup texture and increases opacity of new powerup texture
    mystery->fade+=0.0125;
    fade-=0.0125;
    this->DrawItem(true);
    mystery->DrawItem(true);
}


/*********************************************************************************************
***** END OF POWERUPITEM STRUCTS 
*********************************************************************************************/


/*********************************************************************************************
***** START OF POWERUPITEM LIST 
*********************************************************************************************/


void powerList::push_back(PowerupItem* i) {
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

void powerList::remove(item* i) {
    if (i == head) head = i->next;
    if (i == tail) tail = i->prev;
    if (i->prev != nullptr) i->prev->next = i->next;
    if (i->next != nullptr) i->next->prev = i->prev;
    if (!(i->curr->removed)) UnloadTexture(i->curr->texture);
    delete i;
}


/*********************************************************************************************
***** END OF POWERUPITEM LIST 
*********************************************************************************************/

/*********************************************************************************************
***** START OF POWERUP CLASS
*********************************************************************************************/


Powerup::Powerup() {
    for (int i = 0; i < 3; i++) {
        currPower.push_back(new PowerupItem());
    }
}

Powerup::~Powerup() {
    for (int i = 0; i < 3; i++) {
        delete currPower[i];
    }
    
    item* it = spawnedPower.head;
    while (it != nullptr) {
        item* temp = it;
        it = it->next;
        delete temp;
    }

    UnloadFont(allFont);
}

void Powerup::drawPowerupItems() {

    item* it = spawnedPower.head;
    bool removed = false;
    while(it != nullptr) {
        if (it->curr->removed) { //checks if the current powerup has been clicked
            if (it->curr->id == "mystery") { //we need to do something special for mystery powerups
                Mystery* m = dynamic_cast<Mystery*>(it->curr);
                if (m->fade <= 0) {
                    spawnedPower.push_back(m->mystery);
                    item* temp = it;
                    it = it->next;
                    spawnedPower.remove(temp);
                    continue;
                }
                else m->collect();
            }
            else { //the animation that plays everytime powerup is collected
                it->curr->spaz();
                if (it->curr->fade <= 0) {
                    item* temp = it; it = it->next;
                    removed = true;
                    bool added = false;
                    if (temp->curr->id == "multiplier" && !temp->curr->positive) { //we don't need to add bad multipliers to the collected powers, effects are realized immediately
                        sound().play("resources/audio/debuff.mp3");
                        Multiplier* m = dynamic_cast<Multiplier*>(temp->curr);
                        src->addMultiplier(m->multiplier);
                    }
                    else if (temp->curr->id == "fiverandom") { //don't need to add fiverandom to collected powerups, effects are realized immediately
                        for (int i = 0; i < 5; i++) spawnPowerup(false);
                    }
                    else if (temp->curr->id == "speedchange" && !temp->curr->positive) {
                        SpeedChange* s = dynamic_cast<SpeedChange*>(temp->curr);
                        if (s->variant > 0) fastSpeed.push_back(std::make_pair(s->variant == 1 ? 0.8: 0.65, 1800));
                    }
                    else {
                        for (int i = 0; i < 3; i++) {
                            if (currPower[i]->id == "null") {
                                currPower[i] = temp->curr;
                                if (i == 0) {
                                    currPower[i]->pos.x = circle1.x; currPower[i]->pos.y = circle1.y;
                                }
                                else if (i == 1) {
                                    currPower[i]->pos.x = circle2.x; currPower[i]->pos.y = circle2.y;
                                }
                                else if (i == 2) {
                                    currPower[i]->pos.x = circle3.x; currPower[i]->pos.y = circle3.y;
                                }
                                currPower[i]->fade = 1;
                                currPower[i]->rotation.x = 0;
                                added = true;
                                break;
                            }
                        }
                    }
                    if (!added) delete temp->curr;
                    spawnedPower.remove(temp);
                }
            }
        }
        else {
            if (it->curr->wait) {                                   //fade in of item
                if (it->curr->fade == 0) sound().play("resources/audio/powerup_appear.mp3");
                it->curr->DrawItem(true);
                it->curr->fade+=0.02;
                if (it->curr->fade >= 1) it->curr->wait = false;
            }
            else {
                it->curr->moveItem();
                it->curr->DrawItem(true);
                it->curr->time--;
                if (CheckCollisionPointCircle(GetMousePosition(), (Vector2){it->curr->pos.x, it->curr->pos.y}, 30) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    it->curr->removed = true;
                }
            }
        }
        if (it != nullptr && it->curr->time <= 0 && !it->curr->removed) {       //fading out if not collected after certain time
            it->curr->fade-=0.01;
            if (it->curr->fade <= 0) {
                 item* temp = it; it = it->next;
                 delete temp->curr;
                 spawnedPower.remove(temp);
                 removed = true;
            }
        }
        if (!removed) it = it->next;        //if nothing has been removed, move on to next node
        else removed = false;               //otherwise iterator stays on current one as this will be the next node
    }
}

void Powerup::drawPowerBoard() {
    //drawing border and powerup placeholders
    DrawRectangleRoundedLines(powerBoard, 0.2, 100, 5, (Color){80, 60, 60, 255});
    DrawCircleV(circle1, 25, circleColor);
    DrawCircleV(circle1, 23, BLACK);
    DrawCircleV(circle1, 23, (Color){static_cast<unsigned char>(0+level/2), 0, 0});
    DrawCircleV(circle2, 25, circleColor);
    DrawCircleV(circle2, 23, BLACK);
    DrawCircleV(circle2, 23, (Color){static_cast<unsigned char>(0+level/2), 0, 0});
    DrawCircleV(circle3, 25, circleColor);
    DrawCircleV(circle3, 23, BLACK);
    DrawCircleV(circle3, 23, (Color){static_cast<unsigned char>(0+level/2), 0, 0});

    //drawing text
    Color DarkRed = {static_cast<unsigned char>(255-level*1.4), 0, 0, 255};
    DrawTextEx(allFont, "1", (Vector2){240, 715}, 15, 0, DarkRed);
    DrawTextEx(allFont, "1", (Vector2){240, 715}, 15, 0, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    DrawTextEx(allFont, "2", (Vector2){310, 715}, 15, 0, DarkRed);
    DrawTextEx(allFont, "2", (Vector2){310, 715}, 15, 0, Fade((Color){65, 170, 255, 255}, 1-level/60.0));
    DrawTextEx(allFont, "3", (Vector2){380, 715}, 15, 0, DarkRed);
    DrawTextEx(allFont, "3", (Vector2){380, 715}, 15, 0, Fade((Color){65, 170, 255, 255}, 1-level/60.0));

    //drawing powerups
    for (int i = 0; i < 3; i++) if (currPower[i]->id != "null") currPower[i]->DrawItem(false);
}

void Powerup::spawnPowerup(bool include5Rand) {
    int rand1;
    if (include5Rand) rand1 = GetRandomValue(1, 100);
    else rand1 = GetRandomValue(1, 95);

    //multiplier powerup
    if (rand1 <= 25) {
        int rand2 = GetRandomValue(1, 5);
        if (rand2 <= 1) spawnedPower.push_back(new Multiplier(2, 800, LoadTexture("resources/powerup/2_multiplier.png")));
        else if (rand2 <= 2) spawnedPower.push_back(new Multiplier(1.5, 1200, LoadTexture("resources/powerup/1_5_multiplier.png")));
        else if (rand2 <= 3) spawnedPower.push_back(new Multiplier(1.2, 1500, LoadTexture("resources/powerup/1_2_multiplier.png")));
        else if (rand2 <= 4) spawnedPower.push_back(new Multiplier(-1, 1500, LoadTexture("resources/powerup/negative_multiplier.png")));
        else spawnedPower.push_back(new Multiplier(0.7, 1500, LoadTexture("resources/powerup/0_7_multiplier.png")));
    }
    //next three blocks powerup
    else if (rand1 <= 45) {
        int rand2 = GetRandomValue(1, 5);
        if (rand2 <= 1) spawnedPower.push_back(new ThreeBlock(1000, LoadTexture("resources/powerup/I_block.png"), 1));
        else if (rand2 <= 2) spawnedPower.push_back(new ThreeBlock(1200, LoadTexture("resources/powerup/J_block.png"), 2));
        else if (rand2 <= 3) spawnedPower.push_back(new ThreeBlock(1200, LoadTexture("resources/powerup/L_block.png"), 3));
        else if (rand2 <= 4) spawnedPower.push_back(new ThreeBlock(1200, LoadTexture("resources/powerup/O_block.png"), 4));
        else if (rand2 <= 5) spawnedPower.push_back(new ThreeBlock(1200, LoadTexture("resources/powerup/T_block.png"), 6));
    }
    //change block movedown speed powerup
    else if (rand1 <= 65) {
        int rand2 = GetRandomValue(1, 5);
        if (rand2 == 1) spawnedPower.push_back(new SpeedChange(0, 800, LoadTexture("resources/powerup/pause.png")));
        else if (rand2 == 2) spawnedPower.push_back(new SpeedChange(1, 1000, LoadTexture("resources/powerup/fast_1.png")));
        else if (rand2 == 3) spawnedPower.push_back(new SpeedChange(2, 1000, LoadTexture("resources/powerup/fast_2.png")));
        else if (rand2 == 4) spawnedPower.push_back(new SpeedChange(-1, 1000, LoadTexture("resources/powerup/slow_1.png")));
        else if (rand2 == 5) spawnedPower.push_back(new SpeedChange(-2, 1000, LoadTexture("resources/powerup/slow_2.png")));
    }
    //bomb, nuke, and laser powerups
    else if (rand1 <= 80) { 
        int rand2 = GetRandomValue(1, 5);
        if (rand2 <= 2) spawnedPower.push_back(new Laser(1000, LoadTexture("resources/powerup/lasers.png")));
        else if (rand2 <= 4) spawnedPower.push_back(new Bomb(1000, LoadTexture("resources/powerup/bomb.png")));
        else spawnedPower.push_back(new Nuke(800, LoadTexture("resources/powerup/nuke.png")));
    }
    //permanent multiplier boost powerup
    else if (rand1 <= 86) {
        int rand2 = GetRandomValue(1, 3);
        if (rand2 <= 2) spawnedPower.push_back(new PlusMultiplier(0.1, 1000, LoadTexture("resources/powerup/0_1_multiplier.png")));
        else spawnedPower.push_back(new PlusMultiplier(0.2, 800, LoadTexture("resources/powerup/0_2_multiplier.png")));
    }
    else if (rand1 <= 95) spawnedPower.push_back(new Mystery(1000, LoadTexture("resources/powerup/mystery.png")));
    else if (rand1 <= 100) spawnedPower.push_back(new FiveRandom(1000, LoadTexture("resources/powerup/5_random.png")));
}

PowerupItem* Powerup::usePowerup(int k) {
    PowerupItem* p;
    p = currPower[k-1];
    currPower[k-1] = new PowerupItem();
    return p;
}

std::vector<std::pair<double, int>> Powerup::checkFastSpeed() { 
    std::vector<std::pair<double, int>> temp = fastSpeed;
    fastSpeed.clear();
    return temp;
}; 