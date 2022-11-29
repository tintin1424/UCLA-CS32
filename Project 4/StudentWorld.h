#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), m_tunnelMan(NULL), m_actors(NULL), m_earth(NULL), m_nBarrel(0), m_tick(0)
	{
		//nBarrel is number of barrels, number of ticks
	}

	virtual int init();
	virtual int move();
	virtual void cleanUp();
	virtual ~StudentWorld() {
		cleanUp();
	}

	void removeDeadGameObjects();

	void setDisplayText();
	std::string formatText(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score);

	//--------------------------------------------
	TunnelMan* getTunnelMan() {
		return m_tunnelMan;
	}

	//	void addActor();				//addding new actors
	void newCharacter(Actor* actor) {
		m_actors.push_back(actor);
	}

	void addGoodie();
	bool earthVisible(int x, int y);

	Earth* getEarth(int x, int y) { //get specific block of earth (for when tunnelman digs earth)
		if (y >= 0 && y < 60 && x >= 0 && x < 64) {
			return m_earth[x][y];
		}
		else {
			return NULL;
		}
	}

	bool digEarth(int x, int y);

	void barrelPicked() { //indicate barrel has been picked up by TM
		m_nBarrel--;
	}

	bool inMap(double x, double y); //if coordinate is within the map
	bool canMove(GraphObject::Direction dir, int x, int y);
	void randomPos(int& x, int& y);
	void randomPosNoEarth(int& x, int& y);
	double distanceAway(double x1, double y1, double x2, double y2); //calculate distance between two objects

	bool withinRadiusofProtester(int x, int y, int hpDamage); //whether protester was in radius of 3, and was damaged (returns true if protester is hit)
	Protester* closestProtestor(int x, int y);

private:

	/*void updateGameStatText() {
		std::stringstream ss;
		ss << "Score: " << getScore();
		ss << "Lvl: " << getLevel();
		ss << "Hlth: " << getHP();
		ss << "Wtr: " << getWater();
		ss << "Gld: " << getGold();
		ss << ""
		
		setGameStatText(ss.str());
	}
	*/

	TunnelMan* m_tunnelMan;
	std::vector<Actor*> m_actors;
	std::vector<std::vector<Earth*>> m_earth;
	
	int m_nBarrel;
	int m_tick;
	int m_water;

	int m_score = 0;
	int m_level = 1;
	int m_lives = 3;
};

#endif // STUDENTWORLD_H_
