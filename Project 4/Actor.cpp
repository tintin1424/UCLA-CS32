#include "Actor.h"
#include "StudentWorld.h"
#include <string>
#include <vector>
#include <algorithm>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
SonarKit::SonarKit(StudentWorld* world, int x, int y)
	: Goodie(world, TID_SONAR, x, y, right, 2, true, true, false, false), m_tick(0) {
	//faces right, visible, only pickupable by TM, temporary
	setVisible(true);

	m_tick = std::max(100, 300 - 10 * (int)getStudentWorld()->getLevel());
}

Water::Water(StudentWorld* world, int x, int y)
	: Goodie(world, TID_WATER_POOL, x, y, right, 2, true, true, false, false) {
	//facing right, start visible, only pickupable by TM, start temporary
	m_tick = std::max(100, 300 - 10 * (int)getStudentWorld()->getLevel());
}

void TunnelMan::doSomething() {

	if (!isLiving()) {
		return;			//if not alive, return immediately without performing steps
	}

	//std::cout << "\nman=(" << getStudentWorld()->getTunnelMan()->getX() << ", " << getStudentWorld()->getTunnelMan()->getY() << ")\n";

	int keypress;


	if (getStudentWorld()->getKey(keypress)) { //if user has pressed any of the action keys,
		
		switch (keypress) {

		case KEY_PRESS_LEFT:
			if (getDirection() == left && getStudentWorld()->inMap(getX() - 1, getY())) {
				moveTo(getX() - 1, getY());
			}
			else {
				setDirection(left);
			}
			break;

		case KEY_PRESS_RIGHT:
			if (getDirection() == right && getStudentWorld()->inMap(getX() + 1, getY())) {
				moveTo(getX() + 1, getY());
			}
			else {
				setDirection(right);
			}
			break;

		case KEY_PRESS_UP:
			if (getDirection() == up && getStudentWorld()->inMap(getX(), getY() + 1)) {
				moveTo(getX(), getY() + 1);
			}
			else {
				setDirection(up);
			}
			break;

		case KEY_PRESS_DOWN:
			if (getDirection() == down && getStudentWorld()->inMap(getX(), getY() - 1)) {
				moveTo(getX(), getY() - 1);
			}
			else {
				setDirection(down);
			}
			break;
		case KEY_PRESS_SPACE:
			//fire squirt into oil field, assuming sufficient water
			//reduce water count by 1
			if (m_water >= 1) {
				squirtEnemy(getDirection(), getX(), getY()); //fire water!
				getStudentWorld()->playSound(SOUND_PLAYER_SQUIRT);
				m_water--;

			}
			break;
		case KEY_PRESS_ESCAPE:
			completelyAnnoyed(); //set player to completely annoyed
			break;

		case KEY_PRESS_TAB: //GOLD ACTIVATE
			if (m_gold >= 1) {
				m_gold--;
				GoldNugget* p = new GoldNugget(getStudentWorld(), getX(), getY(), true, false, true, false);
				getStudentWorld()->newCharacter(p);
			}
			break;

		case 'z': //same as uppercase Z; SONAR CHARGE ACTIVATE
		case 'Z':
			break;

		}
		
		//if (getStudentWorld()->earthVisible(getX(), getY())) {
		if (getStudentWorld()->digEarth(getX(), getY())){
			//getStudentWorld()->digEarth(getX(), getY());
			getStudentWorld()->playSound(SOUND_DIG);
		}
		
	}
}

void Boulder::doSomething() {
	addTick();

	if (!isLiving()) {
		return;			//if not alive, return immediately without performing steps
	}
	int cnt = 0;

	switch (m_state) { //page 32
	case stable:
		
		for (int i = 0; i < 4; i++) {
			if (getStudentWorld()->getEarth(getX() + i, getY() - 1) != NULL) {
				if (!getStudentWorld()->getEarth(getX() + 1, getY() - 1)->isLiving()) {
					cnt++;
				}
			}
		}
		if (cnt == 4) {
			waitingBoulder(); 
			resetTicks();
		}
		break;

	case waiting: //enter waiting state for 30 ticks. 
		addTick();
		if (getTicks() >= 30) { //if 30 ticks have elapsed, enter falling state. play FALLING ROCK sound
			getStudentWorld()->playSound(SOUND_FALLING_ROCK);
			fallingBoulder();
		}
		break;
	case falling:
		moveTo(getX(), getY() - 1); //fall downwards per tick

		if (getY() == 0) {
			completelyAnnoyed();
			return;
		}

		for (int i = 0; i < 4; i++) {
			if (getStudentWorld()->getEarth(getX() + i, getY() - 1) != NULL) {
				if (getStudentWorld()->getEarth(getX() + i, getY() - 1)->isLiving()) {
					completelyAnnoyed();
					return;
				}
			}
		}

		if (distanceAway(getX(), getY(), getStudentWorld()->getTunnelMan()->getX(), getStudentWorld()->getTunnelMan()->getY() <= 3.00)) {
			//if within radius of 3, cause 100 points of annoyance. if annoys tunnelman, loses 1 life. 
			getStudentWorld()->getTunnelMan()->completelyAnnoyed();
		}

		getStudentWorld()->withinRadiusofProtester(getX(), getY(), 100);

		break;
	}

}
//----------------------------------SQUIRT FUNCTIONS
//----------------------------------SQUIRT DOSOMETHING
void Squirt::doSomething() {
	
	if (m_travel == 0) { // when squirt has travelled it's full distance, set to dead. 
		completelyAnnoyed();
	}
	if (getStudentWorld()->withinRadiusofProtester(getX(), getY(), 2)) {
		completelyAnnoyed();
	}
	//if square is blocked by either earth or boulder, immediately set state to dead. 
	if (!getStudentWorld()->canMove(getDirection(), getX(), getY())) {
		completelyAnnoyed();
	}
	else {
		moveSquirt(getDirection()); //move in direction tunnelman is facing
		m_travel--;
	}
	
}
//----------------------------------SQUIRTENEMY
void Actor::squirtEnemy(Direction dir, int x, int y) {
	switch (dir) {
	case left:
		if (x - 4 >= 0 && !(getStudentWorld()->earthVisible(x - 4, y))) {
			//if water can travel to left by 4 blocks, 
			//check boulders!!
			Squirt* p = new Squirt(getStudentWorld(), getX() - 4, getY(), left);
			getStudentWorld()->newCharacter(p);
		}
		break;
	case right:
		if (x + 4 <= 60 && !(getStudentWorld()->earthVisible(x + 4, y))) {
			//if water can travel to left by 4 blocks, 
			//check boulders!!
			Squirt* p = new Squirt(getStudentWorld(), getX() + 4, getY(), right);
			getStudentWorld()->newCharacter(p);
		}
		break;
	case up:
		if (y + 4 <= 57 && !(getStudentWorld()->earthVisible(x, y + 4))) {
			//if water can travel to left by 4 blocks, 
			//check boulders!!
			Squirt* p = new Squirt(getStudentWorld(), getX(), getY() + 4, up);
			getStudentWorld()->newCharacter(p);
		}
		break;
	case down:
		if (y - 4 >= 0 && !(getStudentWorld()->earthVisible(x, y - 4))) {
			//if water can travel to left by 4 blocks, 
			//check boulders!!
			Squirt* p = new Squirt(getStudentWorld(), getX(), getY() - 4, down);
			getStudentWorld()->newCharacter(p);
		}
	}
}

void Squirt::moveSquirt(GraphObject::Direction dir) {
	switch (dir) {
	case left:
		if (getStudentWorld()->inMap(getX() - 1, getY())) {
			moveTo(getX() - 1, getY());
		}
		break;

	case right:
		if (getStudentWorld()->inMap(getX() + 1, getY())) {
			moveTo(getX() + 1, getY());
		}
		break;
	case up:
		if (getStudentWorld()->inMap(getX(), getY() + 1)) {
			moveTo(getX(), getY() + 1);
		}
		break;
	
	case down:
		if (getStudentWorld()->inMap(getX(), getY() - 1)) {
			moveTo(getX(), getY() - 1);
		}
		break;
	}
}

void Barrel::doSomething() {

	//std::cout << "a" << std::endl;
	if (!isLiving()) { //if not alive, return immediately
		return;
	}

	//std::cout << "b" << std::endl;
	if (!isVisible()) {
		if (distanceAway(getStudentWorld()->getTunnelMan()->getX(), getStudentWorld()->getTunnelMan()->getY(), getX(), getY()) <= 4.00) {  //if within radius (4 units away from tunnelman)
			setVisible(true); //make itself visible
			return;
		}
		
	}

	//std::cout << distanceAway(getStudentWorld()->getTunnelMan()->getX(), getStudentWorld()->getTunnelMan()->getY(), getX(), getY()) << std::endl;
	
	//std::cout << "\nman=(" << getStudentWorld()->getTunnelMan()->getX() << ", " << getStudentWorld()->getTunnelMan()->getY() << ") oil=(" << getX() << "," << getY() << ") dist=" << distanceAway(getStudentWorld()->getTunnelMan()->getX(), getStudentWorld()->getTunnelMan()->getY(), getX(), getY()) << "\n";
	
	//std::cout << "c" << std::endl;

	if (distanceAway(getX(), getY(), getStudentWorld()->getTunnelMan()->getX(), getStudentWorld()->getTunnelMan()->getY()) <= 3.00) { //othrwise, if barrel within radius of 3, barrel will acivate
		
		std::cout << "oil dead" << std::endl;
		completelyAnnoyed();	//set state to dead
		getStudentWorld()->playSound(SOUND_FOUND_OIL); // b. play sound to indicate player has picked up goodie
		getStudentWorld()->increaseScore(1000); //c. increase score by 1000 points
		getStudentWorld()->barrelPicked();
	}
}

void GoldNugget::doSomething() {
	if (!isLiving()) {
		return;
	}

	m_tick++;

	if (!isVisible() && distanceAway(getX(), getY(), getStudentWorld()->getTunnelMan()->getX(), getStudentWorld()->getTunnelMan()->getY()) <= 4.00) {
		setVisible(true);
		return;
	}

	if (TMcanPick() && distanceAway(getX(), getY(), getStudentWorld()->getTunnelMan()->getX(), getStudentWorld()->getTunnelMan()->getY()) <= 3.00) {
		//activate gold nugget
		completelyAnnoyed();

		getStudentWorld()->playSound(SOUND_GOT_GOODIE);
		getStudentWorld()->increaseScore(10);
		getStudentWorld()->getTunnelMan()->gotGold();
	}

	if (pcanPick() && getStudentWorld()->withinRadiusofProtester(getX(), getY(), 0)) {
		completelyAnnoyed();
		getStudentWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
		getStudentWorld()->closestProtestor(getX(), getY())->hasGold();
		getStudentWorld()->increaseScore(25);
	}
	else if (!isPerma()) {
		if (m_tick > 100) {
			completelyAnnoyed();
		}
	}
}

void SonarKit::doSomething() {
	m_tick--;

	if (!isLiving()) {
		return;
	}

	if (distanceAway(getX(), getY(), getStudentWorld()->getTunnelMan()->getX(), getStudentWorld()->getTunnelMan()->getY()) <= 3.00) {
		//activate sonar kit

		completelyAnnoyed();
		getStudentWorld()->playSound(SOUND_GOT_GOODIE);
		getStudentWorld()->getTunnelMan()->gotSonar();
		getStudentWorld()->increaseScore(75);
	}

	//int T = max(100, 300 - 10*getStudentWorld()getLevel()

	if (m_tick == 0) {
		completelyAnnoyed();
	}
}

void Water::doSomething() {
	if (!isLiving()) {
		return;
	}

	if (distanceAway(getX(), getY(), getStudentWorld()->getTunnelMan()->getX(), getStudentWorld()->getTunnelMan()->getY()) <= 3.00) {
		completelyAnnoyed();
		getStudentWorld()->playSound(SOUND_GOT_GOODIE);
		getStudentWorld()->getTunnelMan()->gotWater();
		getStudentWorld()->increaseScore(100);
	}

	if (m_tick == 0) {
		completelyAnnoyed();
	}
}

//---------------------------
double Actor::distanceAway(double x1, double y1, double x2, double y2) {
	double dist = 1.0 * sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	return dist;
}