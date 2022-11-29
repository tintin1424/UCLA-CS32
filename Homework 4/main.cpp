#include "WordTree.h"
#include <iostream> 
#include <string>
#include <cassert>

using namespace std;

int main() {

	WordTree k;

	k.add("Kim");
	k.add("Kanye");
	k.add("Kanye");
	k.add("Kanye");
	cout << k;

	cout << k.distinctWords() << endl;
	//assert(k.distinctWords() == 2);

	cout << k.totalWords() << endl;
	assert(k.totalWords() == 4);

	WordTree oneD;

	oneD.add("Harry");
	oneD.add("Niall");
	oneD.add("Niall");
	oneD.add("Liam");
	oneD.add("Louis");
	oneD.add("Harry");
	oneD.add("Niall");
	oneD.add("Zayn");
	oneD.add("Zayn");
	oneD.add("Zayn");
	cout << oneD;

	WordTree w12;
	w12.add("hello");
	w12.add("my");
	w12.add("name");
	w12.add("is");
	w12.add("tina");

	assert(w12.totalWords() == 5);

	w12 = oneD;
	assert(w12.totalWords() == 10);

	assert(oneD.distinctWords() == 5);
	assert(oneD.totalWords() == 10);

	WordTree emptyTree;
	assert(emptyTree.distinctWords() == 0);
	assert(emptyTree.totalWords() == 0);

	WordTree repeats;
	repeats.add("t");
	repeats.add("t");
	repeats.add("t");
	repeats.add("t");

	assert(repeats.distinctWords() == 1);
	assert(repeats.totalWords() == 4);
	
}

