#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main() {
    Sequence s;
    assert(s.empty());
    assert(s.find(42) == -1);
    assert(s.insert(42) == 0);
    assert(s.size() == 1 && s.find(42) == 0);
    cout << "Passed all tests" << endl;
}