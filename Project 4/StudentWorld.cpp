#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
//-----------------------------------------------------------------------------------------------------
//----------------------------------------------------------- INIT
int StudentWorld::init() {

    m_tick = 0;
    m_tunnelMan = new TunnelMan(this);  //initalize new tunnelMan (create new)

    int level = getLevel(); //level of game player is on

    vector<vector<Earth*>> earthArr;
    for (int x = 0; x < 64; x++) { //populate block of earth row by row. 
        vector <Earth*> row;

        for (int y = 0; y < 60; y++) {
            row.push_back(new Earth(this, x, y));

            if (!(x < 30 || x > 33 || y < 4)) { // single tunnel, 4 squares wide and 56 squares deep leads from surface into bottom; devoid of earth.
                row.back()->completelyAnnoyed();
                //for every square that is not in this range, new earth square is created and added to vector
            }
        }
        earthArr.push_back(row);
    }
    m_earth = earthArr;

    //init vector of actors
    vector<Actor*> actorVec; 
    m_actors = actorVec;

    int B = min(((int)level / 2) + 2, 9);
    for (int boulder = 0; boulder < B; boulder++) {
        int startX, startY;

        randomPos(startX, startY);
        if (startY < 20) {
            startY += 20; //startY must be greater than 20. 
        }
        newCharacter(new Boulder(this, startX, startY));
        digEarth(startX, startY);
    }
    //cout << "boulders created";

    int L = min(2 + (int)level, 21); //cast level to int
    m_nBarrel = L;
    for (int oil = 0; oil < m_nBarrel; oil++) {
        int a, b;

        randomPos(a, b);
        newCharacter(new Barrel(this, a, b));
        cout << "oil created at " << a << ", " << b << endl;
        
    }


    int G = max(5 - (int)level / 2, 2);
    for (int gold = 0; gold < G; gold++) {
        int startX, startY;
        randomPos(startX, startY);
        newCharacter(new GoldNugget(this, startX, startY, false, true, true, true));
        cout << "gold created at " << startX << ", " << startY << endl;
    }
    cout << "gold created";

    //updateGameStatText();

    return GWSTATUS_CONTINUE_GAME; //returns status to continue game
}
//-----------------------------------------------------------------------------------------------------
//----------------------------------------------------------- MOVE
int StudentWorld::move() {

    m_tick++; //each tick, each actor moves

    setDisplayText();
    addGoodie();

    m_tunnelMan->doSomething();

    vector<Actor*>::iterator actors = m_actors.begin();

    while (actors != m_actors.end()) { //each actor's doSomething() function is called each tick 
        if ((*actors)->isLiving()) {        //if actor is still alive
            (*actors)->doSomething();       //ask each actor to do something
            if (!m_tunnelMan->isLiving()) { //if player died during this tick, return player died
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
        actors++;
    }
    actors = m_actors.begin();

    //check if earth pointer == tunnelMan --> destroy earth

    /*for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Earth* currentEarth = getEarth(m_tunnelMan->getX() + i, m_tunnelMan->getY() + j); //returns pointer 

                //delete currentEarth
            if (currentEarth != NULL) {
                currentEarth->completelyAnnoyed();
            }
        }
    }
    */

    //remove newly-dead actors after each tick
    removeDeadGameObjects(); //delete dead game objects

    //return proper result
    if (!m_tunnelMan->isLiving()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    //if player collects all barrels, return result that player finished level
    if (m_nBarrel == 0) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }

    //player hasn't completed cur level and hasn't died; let them continue playing
    return GWSTATUS_CONTINUE_GAME;
}
//-----------------------------------------------------------------------------------------------------
void StudentWorld::addGoodie(){ //(page 21)
    int G = getLevel() * 25 + 300;
    //1 in G chance water or sonar kit will be added

    if (rand() % G >= 1) {
        return;
    }

    if (rand() % 5 < 1) {
        newCharacter(new SonarKit(this, 0, 60));
    }
    else {
        int x, y;
        randomPosNoEarth(x, y);
        newCharacter(new Water(this, x, y));
    }

}


bool StudentWorld::digEarth(int x, int y) {
    bool removed = false;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            if (x + i >= 0 && x + i < 64 && y + j >= 0 && y + j < 60) {
                if (m_earth[x + i][y + j]->isLiving() && m_earth[x + i][y + j] != nullptr) {
                    m_earth[x + i][y + j]->completelyAnnoyed();
                    removed = true;
                }
            }
        }
    }

    return removed; //if earth was removed, return true;
}

//-----------------------------------------------------------------------------------------------------
void StudentWorld::removeDeadGameObjects() {
    vector<Actor*>::iterator actors = m_actors.begin();

    while (actors != m_actors.end()) {
        if ((*actors)->isLiving()){
            actors++;
        }
        else {
            Actor* p = *actors;
            delete p;
            actors = m_actors.erase(actors);
        }
    }
}
//----------------------------------------------------------- CLEANUP
void StudentWorld::cleanUp() {

	delete m_tunnelMan; //pointer to tunnelMan

    vector <Actor*>::iterator actors = m_actors.begin(); //to be deleted: earth, all actors. 
    while (actors != m_actors.end()) {
        Actor* p = *actors;
        delete p;
        actors = m_actors.erase(actors);
    }

    for (int i = 0; i < m_earth.size(); i++) {
        vector<Earth*>::iterator earth = m_earth[i].begin();

        while (earth != m_earth[i].end()) {
            Earth* p = *earth;
            delete p;
            //++earth;
            earth = m_earth[i].erase(earth);
        }
    }
	
}

//if item is in map AND not a boulder, return true. 
bool StudentWorld::inMap(double x, double y) {

    if (x < 0 || x > 60 || y < 0 || y > 60) {
        return false;
    }

    /*for (Actor* i : m_actors) { //for all actors, check if location is in map
        if ((x + SPRITE_WIDTH - 1 > i->getX()) && (x + 1 < i->getX() + SPRITE_WIDTH)) {
            if ((y + SPRITE_HEIGHT - 1 > i->getY()) && (y + 1 < i->getY() + SPRITE_HEIGHT)) {
                return false;
            }
        }
    }*/

    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        Boulder* b = dynamic_cast<Boulder*>(*it);
        if (b) {
            if (distanceAway(x, y, b->getX(), b->getY()) <= 3.00) {
                return false;
            }
        }
        ++it;
    }

    return true;
}

bool StudentWorld::canMove(GraphObject::Direction dir, int x, int y) {
    switch (dir) {
    case GraphObject::left:
        x--;
        break;
    case GraphObject::right:
        x++;
        break;
    case GraphObject::up:
        y++;
        break;
    case GraphObject::down:
        y--;
        break;
    } //cehck all directions!

    //if not in bounds, or coordinates overlap with earth, return false. 
    if (x < 0 || x > 60 || y < 0 || y > 60 || earthVisible(x, y)) { 
        return false;
    }

    return true;

}

bool StudentWorld::withinRadiusofProtester(int x, int y, int hpDamage) {
    bool hit = false;

    vector<Actor*>::iterator it;
    it = m_actors.begin();

    while (it != m_actors.end()) { //loop through all actors
        
        Protester* pro = dynamic_cast<Protester*>(*it);
        if (pro && distanceAway(x, y, pro->getX(), pro->getY()) <= 3.00) {  //if actor is a protester, 
            //and distance away is less than or equal to 3.00, hit the protester
            pro->annoyed(hpDamage);
            hit = true; //return true.
        }
        it++;
    }

    return hit;
}

Protester* StudentWorld::closestProtestor(int x, int y) {
    vector<Actor*>::iterator it;
    it = m_actors.begin();

    while (it != m_actors.end()) { //loop through all actors

        Protester* pro = dynamic_cast<Protester*>(*it);
        if (pro && distanceAway(x, y, pro->getX(), pro->getY()) <= 3.00) {  //if actor is a protester, 
            //and distance away is less than or equal to 3.00, hit the protester
            return pro;
        }
        it++;
    }

    return nullptr;
}
//-----------------------------------------------------------------------------------------------------ISEARTH
//-----------------------------------------------------------------------------------------------------
bool StudentWorld::earthVisible(int x, int y) { //check if earth is visible within next 4 by 4 block
    for (int m = 0; m < 4; m++) {
        for (int n = 0; n < 4; n++) {
            if (getEarth(x + m, y + n) != NULL) {       //if earth exists within parameters, return true. 
                if (getEarth(x + m, y + n)->isLiving()) {
                    return true;
                }
            } //otherwise, return false. 
            //return false;
        }
    }

    return false;
}

void StudentWorld::setDisplayText() {
    int level = getLevel();
    int lives = getLives();
    int health = m_tunnelMan->getHP();
    int squirts = m_tunnelMan->getWater();
    int gold = m_tunnelMan->getGold();
    int barrelsLeft = m_nBarrel;
    int sonar = m_tunnelMan->getSonar();
    int score = getScore();
    // create a string from your statistics, of the form:
    // Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
    string s = formatText(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
    // update the display text at the top of the screen with newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld::formatText(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score) {
    // create a string from your statistics, of the form:
    // Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
    string ss = "";
    
    ostringstream flevel;
    flevel.fill(' ');
    
    flevel << setw(2) << level; //set to two digits
    ss += "Lvl: " + flevel.str() + "  ";

    ss += "Lives: ";
    ss += lives + '0';
    ss += "  ";

    ostringstream fhealth; //health format
    fhealth.fill(' '); //3 digits, leading spaces, followed by percent sign
    fhealth << setw(3) << health;
    ss += "Hlth: " + fhealth.str() + "%  ";

    ostringstream fsquirts; //water format; 2 digits, leading space. 
    fsquirts.fill(' ');
    fsquirts << setw(2) << squirts;
    ss += "Wtr: " + fsquirts.str() + "  ";
    
    ostringstream fgold; // gold format; 2 digits, leading space
    fgold.fill(' ');
    fgold << setw(2) << gold;
    ss += "Gld: " + fgold.str() + "  ";
   
    ostringstream fbarrels; // oil format; 2 digits, leading space
    fbarrels.fill(' ');
    fbarrels << setw(2) << barrelsLeft;
    ss += "Oil Left: " + fbarrels.str() + "  ";
    
    ostringstream fsonar; // sonar format; 2 digits, leading space
    fsonar.fill(' ');
    fsonar << setw(2) << sonar;
    ss += "Sonar: " + fsonar.str() + "  ";

    ostringstream fscore; // score format; 6 digits, leading zeros
    fscore.fill('0');
    fscore << setw(6) << score;
    ss += "Scr: " + fscore.str();
    return ss;
}

void StudentWorld::randomPos(int &startX, int &startY) { //returns random position IN earth
    bool inMap = false;

    do {
        startX = rand() % 60;
        startY = rand() % 56;
        inMap = true;

        vector <Actor*>::iterator it = m_actors.begin();
        while (it != m_actors.end()) {
            if (distanceAway(startX, startY, (*it)->getX(), (*it)->getY()) <= 6.0) {
                inMap = false;
            }
            it++;
        }

        if ((startX > 25 && startX < 34) && (startY > 0 && startY < 60)) {
            inMap = false;
        }

    } while (!inMap);
}

void StudentWorld::randomPosNoEarth(int& x, int& y) {
    bool inMap = false;

    do {
        x = rand() % 60;
        y = rand() % 56;
        inMap = true;

        
        if (earthVisible(x, y)) {
            inMap = false;
        }
        
         //if other actors are within 6 units, rturn false
        vector <Actor*>::iterator it = m_actors.begin();
        while (it != m_actors.end()) {
            if (distanceAway(x, y, (*it)->getX(), (*it)->getY()) <= 6.0) {
                inMap = false;
            }
            it++;
        }

    } while (!inMap);
}

double StudentWorld::distanceAway(double x1, double y1, double x2, double y2) {
    double dist = 1.0 * sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return dist;
}