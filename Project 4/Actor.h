#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>
#include <algorithm>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, float size, unsigned int depth)
		: GraphObject(imageID, startX, startY, dir, size, depth), m_studentWorld(world), m_isLiving(true) {

		setVisible(true);	//visible if alive. 
	}

	virtual ~Actor() { } //destructors always virtual

	//ACTIONS
	virtual void doSomething() = 0; // all actors doSomething each tick.
	void squirtEnemy(Direction dir, int x, int y); //fire water at enemy protesters

	//STATE
	StudentWorld* getStudentWorld() { return m_studentWorld; }
	bool isLiving() {
		return m_isLiving;
	}

	void completelyAnnoyed() { //actor is dead
		m_isLiving = false;
		setVisible(false);		//actors dead no longer appear
	}

	double distanceAway(double x1, double y1, double x2, double y2);

	

private:
	bool m_isLiving;	//if actor is living. 
	StudentWorld* m_studentWorld;
};

//---------------------------TUNNELMAN
class TunnelMan : public Actor {
public:
	TunnelMan(StudentWorld* world)
		: Actor(world, TID_PLAYER, 30, 60, right, 1.0, 0),
		m_hp(10), m_water(5), m_sonar(1), m_gold(0) {
		//tunnelMan starts at location x = 30, y = 60. 
		//10 hit points, 5 units of water, 1 sonar charge, 0 gold nuggets, facing rightward. 
		//depth 0, size 1.0
	}

	virtual ~TunnelMan() { } //virtual destructor

	virtual void doSomething();
	virtual void annoyed(int cnt) {
		m_hp-= cnt;
	}

	int getHP() {
		return m_hp * 10;
	}
	
	int getWater() {
		return m_water;
	}
	
	int getSonar() {
		return m_sonar;
	}
	
	int getGold() {
		return m_gold;
	}

	void gotGold() { m_gold++; }
	void gotWater() { m_water++; }
	void gotSonar() { m_sonar++; }
	

private:
	int m_hp;
	int m_water;
	int m_sonar;
	int m_gold;
};

//---------------------------EARTH
class Earth : public Actor {
public: 
	Earth(StudentWorld *world, int x, int y)
		: Actor(world, TID_EARTH, x, y, right, 0.25, 3) {
		//depth 3 (meaning it should alwyas be in the background), size 0.25 (p. 30)
	}

	virtual ~Earth() { } //destructor

	virtual void doSomething(){ }
	virtual void annoyed(int cnt) {} //does not lose lives when annoyed. 
};

//------------------------------------------------------------BOULDER
class Boulder : public Actor {
public:
	Boulder(StudentWorld* world, int x, int y)
		: Actor(world, TID_BOULDER, x, y, down, 1.0, 1) {

		//depth 1 (appear in above earth), size 1.0
		//m_state = stable;
		setVisible(true);
	}

	virtual ~Boulder() { } //destructor

	virtual void doSomething();
	virtual void annoyed(int cnt) { } 
	//boulders cannot be annoyed!

	int getTicks() {
		return m_waitingTicks;
	}

	void resetTicks() {
		m_waitingTicks = 0; //resets to 0 ticks
	}
	void addTick() {
		m_waitingTicks++;
	}

	// functions to set states of boulder
	void stableBoulder() { m_state = stable; }
	void fallingBoulder() {	m_state = falling; }
	void waitingBoulder() {	m_state = waiting; }
	//State curState() const { return m_state; }
	
private:

	enum State { stable, waiting, falling };
	State m_state = stable; //start off stable.

	int m_waitingTicks;
};
//------------------------------------------------------------SQUIRT

class Squirt : public Actor {
public:
	Squirt(StudentWorld* world, int x, int y, Direction dir)
		: Actor(world, TID_WATER_SPURT, x, y, dir, 1.0, 1), m_travel(4) {
		//page 32 manual: start at x, y; size 1.0 and depth 1; initial travel distance of 4 squares 
		setVisible(true);
	}

	virtual ~Squirt() { }
	virtual void doSomething();

	//squirt can only move if space permits (check direction)
	void moveSquirt(GraphObject::Direction dir);

private:
	int m_travel;
};

//------------------------------------------------------------PROTESTOR
//------------------------------------------------------------PROTESTOR

class Protester : public Actor { // two types of protesters
public: 
	Protester(StudentWorld* world, int imageID);

	virtual ~Protester() { }

	virtual bool hardMode() = 0;

	virtual void hasGold() = 0;

	void doSomething();

	virtual void annoyed(int n) = 0;

	void setProtesterHP(int n) { m_protesterHP = n; } //set to different 
	int getProtesterHP() {
		return m_protesterHP * 10;
	}
	bool OOB() { return m_OOB; } //if protestor is out of bounds of oil field
	
	//int getTicksToWaitBetweenMoves() { return max(0, 3 - getLevel() / 4); }

private:
	int m_protesterHP;
	bool m_OOB;
	int m_restingTick; //page 39, resting ticks. 
	int ticksToWaitBetweenMoves;
};

//------------------------------------------------REGULAR PROTESTOR
class RegularProtester : public Protester {
public:
	RegularProtester(StudentWorld* world)
		: Protester(world, TID_PROTESTER) {
		setProtesterHP(5);
	}

	virtual bool hardMode() { return false; }
	virtual void hasGold(); //protesters are different when they have gold 
private:
};

//------------------------------------------------HARDCORE PROTESTOR
class HardcoreProtester : public Protester {
public:
	HardcoreProtester(StudentWorld* world)
		: Protester(world, TID_HARD_CORE_PROTESTER) {
		setProtesterHP(20);
	}

	virtual ~HardcoreProtester() { }

	virtual bool hardMode() { return true; }
	virtual void hasGold();
};


class Goodie: public Actor {
public:
	Goodie(StudentWorld* world, int imageID, int x, int y, Direction dir, unsigned int depth, bool visible, bool TMcanPick, bool pcanPick, bool perma)
		: Actor(world, imageID, x, y, dir, 1.0, depth), m_TMcanPick(TMcanPick), m_pcanPick(pcanPick), m_perma(perma) {
		setVisible(visible); //different goodies appear and disappear at different times
		//all goodies have size 1
	}

	virtual ~Goodie() { } // destructor

	virtual void doSomething() = 0;
	virtual void annoyed(int cnt) = 0; // when goodies are picked up, they are destroyed
	
	bool TMcanPick() {
		return m_TMcanPick;
	}
	bool pcanPick() {
		return m_pcanPick;
	}
	bool isPerma() {
		return m_perma;
	}
	/*bool isVisible() {
		return m_visible;
	}*/

	void setPerma() {
		m_perma = true;
	}
	void setNotPerma() {
		m_perma = false;
	}
	/*void setVisibility(bool isSeen) {
		setVisible(isSeen);
		m_visible = isSeen;
	}
	void setNotVisible() {
		m_visible = false;
	}*/

private:
	bool m_TMcanPick;
	bool m_pcanPick;
	bool m_perma;
	//bool m_visible;
};

class Barrel : public Goodie { //page 33
public:
	Barrel(StudentWorld* world, int x, int y)
		: Goodie (world, TID_BARREL, x, y, right, 2, false, true, false, true){
		//not visible until shown, TMcanpick, Pcannotpick, perma. 
	}

	virtual ~Barrel() {} //virtual destructor
	virtual void doSomething();
	virtual void annoyed(int cnt) { }

private:
};

class GoldNugget : public Goodie {
public:
	GoldNugget(StudentWorld* world, int x, int y, bool visible, bool TMcanPick, bool pcanPick, bool perma)
		: Goodie(world, TID_GOLD, x, y, right, 2, visible, TMcanPick, pcanPick, perma), m_tick(0) {
		//can be picked up by protesters for bribing. start out invisible or visible
		//p. 34 -- depth 2, size 1.0
	}

	virtual ~GoldNugget() {	}
	virtual void doSomething();
	virtual void annoyed(int cnt) { }

private:
	int m_tick;
};

class SonarKit : public Goodie {
public:
	SonarKit(StudentWorld* world, int x, int y);

	virtual ~SonarKit() { }
	virtual void doSomething();
	virtual void annoyed(int cnt) { }

private:
	int m_tick;
};

class Water : public Goodie {
public:
	Water(StudentWorld* world, int x, int y);

	virtual ~Water() { }
	virtual void doSomething();
	virtual void annoyed(int cnt) { }

private:
	int m_tick;
};

#endif // ACTOR_H_