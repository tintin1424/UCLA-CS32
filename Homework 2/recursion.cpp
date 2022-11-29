#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

// Returns the product of two non-negative integers, m and n,
// using only repeated addition.
int multiplyTwoNumbers(unsigned int m, unsigned int n)
{

	if (m == 0 || n == 0) {
		return 0;
	}

	int product = m;
	product += multiplyTwoNumbers(m, n-1);

	return product;
}


// Returns the number of occurrences of digit in the decimal
// representation of num. digit is an int between 0 and 9 inclusive.
//
// Pseudocode Example:
// tabulationStation(18838, 8) => 3
// tabulationStation(55555, 3) => 0
// tabulationStation(0, 0) => 0 or 1 (either if fine)

int tabulationStation(int num, int digit)
{
	if (num == 0) {
		return 0;
	}

	if (num % 10 == digit) {
		return (1 + tabulationStation(num / 10, digit));
	}
	
	return tabulationStation(num / 10, digit);
}


// Returns a string where the same characters next each other in
// string n are separated by "55" (you can assume the input
// string will not have 5’s)
//
// Pseudocode Example:
// highFives("goodbye") => "go55odbye"
// highFives("yyuu") => "y55yu55u"
// highFives("aaaa") => "a55a55a55a"
//
string highFives(string n)
{
	if (n.size() == 0 || n.size() == 1) {
		return n;
	}

	if (n.substr(0, 1) == n.substr(1, 1)) {
		return (n.substr(0, 1) + "55" + highFives(n.substr(1, n.size() - 1)));
	}
	
	return(n.substr(0, 1) + highFives(n.substr(1, n.size() - 1)));

}


// str contains a single pair of parentheses, return a new
// string made of only the parentheses and whatever those
// parentheses contain
//
// Pseudocode Example:
// jheriCurls("abc{ghj}789") => "{ghj}"
// jheriCurls("{x}7") => "{x}"
// jheriCurls("4agh{y}") => "{y}"
// jheriCurls("4agh{}") => "{}"
//
string jheriCurls(string str)
{

	if (str.size() == 0) {
		return str;
	}

	if (str[0] == '{' && str[str.size() - 1] == '}' ) { //if first and last are curly brackets, return string
		return str;
	}

	if (str[0] != '{') {
		return jheriCurls(str.substr(1, -1)); // substring up to the last character
	}

	return jheriCurls(str.substr(0, str.size() - 1));

}


// Return true if the total of any combination of elements in
// the array a equals the value of the target.
//
// Pseudocode Example:
// aggregationNation({2, 4, 8}, 3, 10) => true
// aggregationNation({2, 4, 8}, 3, 6) => true
// aggregationNation({2, 4, 8}, 3, 11) => false
// aggregationNation({2, 4, 8}, 3, 0) => true
// aggregationNation({}, 0, 0) => true
//
bool aggregationNation(const int a[], int size, int target)
{
	if (size == 0 && target == 0) { //base case
		return true;
	}

	if (target == 0){ //some combination of items sum to target
		return true;
	}

	if (size <= 0){//no more items left to add
		return false;
	}
	
	return (aggregationNation(a + 1, size - 1, target) || aggregationNation(a + 1, size - 1, target - a[0]));

}

bool onMyWayHome(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	
	if (sr == er && sc == ec) { //if start location equal to ending location, solvable!
		return true;
	}

	maze[sr][sc] = 'A'; //mark as visited. 

	//if north is unvisited
	if (maze[sr - 1][sc] == '.' && (sr - 1) >= 0) {
		if (onMyWayHome(maze, nRows, nCols, sr - 1, sc, er, ec)) {
			return true;
		}
	}

	//if east is unvisited
	if (maze[sr][sc + 1] == '.' && (sc + 1) < nCols) {
		if (onMyWayHome(maze, nRows, nCols, sr, sc + 1, er, ec)) {
			return true;
		}
	}

	//if south is unvisited
	if (maze[sr + 1][sc] == '.' && (sr + 1) < nRows) {
		if (onMyWayHome(maze, nRows, nCols, sr + 1, sc, er, ec)) {
			return true;
		}
	}

	//if west is unvisited
	if (maze[sr][sc - 1] == '.' && ((sc - 1) >= 0)) {                
		if (onMyWayHome(maze, nRows, nCols, sr, sc - 1, er, ec)) {
			return true;
		}
	}

	return false;

}
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise


//------------------------------------------------------------------------------MAIN
int main(){
	/*
	cout << multiplyTwoNumbers(7, 8) << endl;
	cout << multiplyTwoNumbers(8, 0) << endl;
	cout << multiplyTwoNumbers(0, 8) << endl;
	cout << multiplyTwoNumbers(100, 5) << endl;

	assert(tabulationStation(18838, 8) == 3);
	assert(tabulationStation(55555, 3) == 0);
	assert(tabulationStation(0, 0) == 0);

	assert(highFives("goodbye") == "go55odbye");
	assert(highFives("yyuu") == "y55yu55u");
	assert(highFives("aaaa") == "a55a55a55a");
	assert(highFives("a") == "a"); //one charactre
	assert(highFives("") == ""); //no characters
	assert(highFives("arc") == "arc"); //no repeated
	assert(highFives("   ") == " 55 55 "); //SPACES + 3 repeated characters
	assert(highFives("222") == "2552552"); //3 repeated characters

	assert(jheriCurls("abc{ghj}789") == "{ghj}");
	assert(jheriCurls("abc{ghj}") == "{ghj}"); //no characters at end
	assert(jheriCurls("abc{}fds") == "{}"); //no characters in middle
	assert(jheriCurls("") == "");
	assert(jheriCurls("{x}7") == "{x}"); //no characters at front
	assert(jheriCurls("4agh{y}") == "{y}");
	assert(jheriCurls("4agh{}") == "{}");


	int arr1[3] = { 2, 4, 8 };
	assert(aggregationNation(arr1, 3, 10) == true);
	assert(aggregationNation(arr1, 3, 6) == true);
	assert(aggregationNation(arr1, 3, 11) == false);
	assert(aggregationNation(arr1, 3, 134)== false);
	assert(aggregationNation(arr1, 3, 0) == true);
	assert(aggregationNation({}, 0, 0) == true);

	int arr2[3] = { -1, 3, 0 }; //0 and negative integers
	assert(aggregationNation(arr2, 3, 2));



	string maze[10] = {
		"XXXXXXXXXX",
		"X.......@X",
		"XX@X@@.XXX",
		"X..X.X...X",
		"X..X...@.X",
		"X....XXX.X",
		"X@X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (onMyWayHome(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;


	string maze2[10] = {
	"XXXXXXXXXX",
	"X.......@X",
	"XX@X@@XXXX",
	"X..X.X...X",
	"X..X...@.X",
	"X....XXX.X",
	"X@.....XXX",
	"X..XX.XX.X",
	"X...X....X",
	"XXXXXXXXXX"
	};

	if (onMyWayHome(maze2, 10, 10, 8, 2, 1, 1))
	cout << "Solvable!" << endl;
	else
	cout << "Out of luck!" << endl;


	cout << endl << "====================" << endl << "psased all tests" << endl << "====================";
	*/

	return 0;

}