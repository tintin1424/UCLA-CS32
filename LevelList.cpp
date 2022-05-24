#include "LevelList.h"

LevelList :: LevelList() {

}

bool LevelList :: add(unsigned long level) {

	if (30 <= level && level <= 400 && m_levelList.size() < DEFAULT_MAX_ITEMS) {
		//add it to level list and return true
		m_levelList.insert(level);
		return true;
	}
	else {
		return false;
	}
}

bool LevelList :: remove(unsigned long level) {

	//find will return the smallest position if level exists
	int pos = m_levelList.find(level);
	bool removed = m_levelList.erase(pos);

	return removed; //return true if a level was removed, otherwise false
}

int LevelList :: size() const {

	return m_levelList.size();

}// Return the number of levels in the list.

unsigned long LevelList :: minimum() const {

	unsigned long val;

	if (m_levelList.size() <= 0) { //if list is empty, return no level
		return NO_LEVEL;
	}
	else {
		m_levelList.get(0, val); //return lowest valued level in list
		return val;
	}
}

unsigned long LevelList :: maximum() const {

	unsigned long val;

	if (m_levelList.size() <= 0) { //if list is empty, return no level
		return NO_LEVEL;
	}
	else {
		m_levelList.get(size() - 1, val); //return HIGHEST valued level in list
		return val;
	}
}