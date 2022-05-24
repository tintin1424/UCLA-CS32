#include "LevelList.h"
#include "Sequence.h"
#include <iostream>
#include <cassert>

using namespace std;

void testLevelList();

void testLevelList() {
//int main() {
    LevelList test;
    assert(test.size() == 0);
    assert(test.maximum() == NO_LEVEL);
    assert(test.maximum() == NO_LEVEL);
    test.add(46);
    test.add(75);
    test.add(350);
    test.add(289);
    test.add(-5);
    assert(test.size() == 4);
    assert(test.maximum() == 350);
    assert(test.minimum() == 46);
    assert(test.remove(350) == true);
    assert(test.remove(42) == false);
    assert(test.size() == 3);
    cerr << "Passed all tests!" << endl;
}