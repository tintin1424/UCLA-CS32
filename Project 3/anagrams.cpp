
#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
#include <string>

using namespace std;

const int MAXRESULTS = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

//---------------------------------------------------------------------------------
// Puts each string in dictfile into the array dict. Returns the number of words 
// read into dict.This number should not be larger than MAXDICTWORDS since that is
// the size of the array.

int lexiconBuilderHelp(int i, istream& dictfile, string dict[]);

int lexiconBuilder(istream& dictfile, string dict[]) {
    return lexiconBuilderHelp(0, dictfile, dict);
}

int lexiconBuilderHelp(int i, istream& dictfile, string dict[]) { //helper function, as per FAQ (using recursion by introducing third parameter i). 
    if (i >= MAXDICTWORDS) { // number should not be larger than MAXDICTWORDS
        return MAXDICTWORDS;
    }
        
    string nextLine;
    if (getline(dictfile, nextLine)) { //get next line from dictfile and copy into nextLine
        dict[0] = nextLine;
    }
    else {
        return i;
    }
      
    return lexiconBuilderHelp(i + 1, dictfile, dict + 1);
}

//---------------------------------------------------------------------------------
//Puts all the possibilities of word which are found in dict into results. Returns
//the NUMBER OF MATCHED WORDS found. Number should not be larger than MAXRESULTS.
// The size is the number of words inside the dict array.

        //-------------------- STRATEGY -----------------------
        //1. compute all permutations of given word
        //2. compare to words in result
        //3. for each permutation, if it matches word in dict, add to results. add 1 to count. 
        //          **update: check if word matches results first! 
        //4. otherwise, do not add. no changes made. 

int permute(string prefix, const string dict[], string word, int size, string results[]);   //if MATCH is in dict and not already in results, add to results and add 1 to number of matches. Otherwise, return 0 and remain unchanged. 
int allWords(int max, string prefix, string rest, const string dict[], int size, string results[], int i);
//returns number of permutations of the word rest. prefix is empty when passed in. 

bool inDict(string word, const string dict[], int numwords, string results[]);        // checks if permutations match any words in dict
bool inResults(int it, string prefix, string results[]);                              // checks if permutations match any words in results


int firstEmptyR(const string results[], int n) { //returns the first empty position of results array
    if (results[n] == "") { //if nothing found, return 0. 
        return 0;
    }

    return 1 + firstEmptyR(results, n + 1);
}

int permute(string prefix, const string dict[], string word, int size, string results[]) {
    //prefix starts as empty string.

   bool isInResults = inResults(0, prefix, results);           //match found in results
   // bool isInDict = inDict(prefix, dict, size, results);        //match found in dictionary
   //********** MISTAKE


    if ((word.length() == 0) && inDict(prefix, dict, size, results) && !isInResults) { //if word size = 0 and word is in dict AND word is not in results, return 1. 
        return 1;
    }

    else if (word.length() == 0) {    //otherwise, if length = 0, return 0
        return 0;
    }

    //if word length is not 0, find all permutations of word
    else return allWords(word.length(), prefix, word, dict, size, results, 0); //allWords is helper function to determine all permutations
    
}

int allWords(int length, string prefix, string word, const string dict[], int numwords, string results[], int i) { //all permutations of word
    if (i >= length) { //if all permutations have been checked, return 0
        return 0;
    }

    char restChar = word[i];

    string beginning = word.substr(0, i);               //take letter at beginning of word for i characters
    string ending = word.substr(i + 1, length);         //from ith character to end of word (delete letter from rest of word)

    int permutereturn = allWords(length, prefix, word, dict, numwords, results, i + 1) + permute(prefix + restChar, dict, beginning + ending, numwords, results);
                        //increment letter                                                  //permutations of prefix + restChar
                        // this function recursively calls itself twice: once on the prefix and once on the rest of the letters. 
                        // 
    //cout << "permutereturn: "<< permutereturn << endl;
    return permutereturn;
}

bool inDict(string word, const string dict[], int numwords, string results[]) { //fills results[] with word if found in dictionary
    if (numwords == 0) {                        //if empty, return false. 
        //cout << "not in dict " << endl;
        return false;
    }

    if (word == *dict) {                  //if matching word is found, add word to results. 
       // if (!inResults(0, word, results)){   
       // cout << "adding " << word << " to results" << endl;
        results[firstEmptyR(results, 0)] = word; //firstEmpty returns position of first empty memory in resuts. sets word to prefix. 
       
        // }
       //cout << " in dict " << endl;
        return true;
    }

    return inDict(word, dict + 1, numwords - 1, results);
}

bool inResults(int it, string prefix, string results[]) {                              // checks if permutations match any words in results

    if (results[it] == prefix) {
        //cout << " in results " << endl;
        return true;
    }
    
    else if (results[it] == "") {
        //cout << " not in results " << endl;
        return false;
    }

     return inResults(it + 1, prefix, results);
}


int theJumbler(string word, const string dict[], int size, string results[]) { //FUNCTION 2

    //int test = permute("", word, dict, size, results);
    //cout << "permute returns: " << test << endl << "---------------" << endl;
    //-------------------------------------------------------------------------------------------------delete after!
    ///

    return permute("", dict, word, size, results);      //helper function!
}

//---------------------------------------------------------------------------------
//Displays size number of strings from results. The results can be printed in any order.


int findMatch(int i, int max, const string results[]);

void divulgeSolutions(const string results[], int size) {
    
    if (findMatch(0, size, results) == 0) {                 //if no matches, display message.

        cout << "No matches found" << endl << endl;
    }
}

int findMatch(int i, int max, const string results[]) {
    if (i >= max) {                                         //went through all words in results array, no matches found. 
        return 0;                                           //must return value. 
    }

    cout << "Matching word " << results[i] << endl;         //print word at ith position of results

    int cnt = 1 + findMatch(i + 1, max, results);           //increment number of matches if found. 
    
    return cnt; 
}


//---------------------------------------------------------------------------------
int main()
{
    string results[MAXRESULTS];
    string dict[MAXDICTWORDS];
    ifstream dictfile;         // file containing the list of words
    int nwords;                // number of words read from dictionary
    string word;

    dictfile.open("words.txt");
    if (!dictfile) {
        cout << "File not found!" << endl;
        return (1);
    }

    nwords = lexiconBuilder(dictfile, dict);
    //cout << nwords << endl;
   // cout << dict[nwords - 1] << endl;

    cout << "Please enter a string for an anagram: ";
    cin >> word;

      int numMatches = theJumbler(word, dict, nwords, results);
      
      //cout << endl<< "numMatches: " << numMatches << endl;
      
      if (!numMatches) {
          cout << "No matches found" << endl;
      }
      else {
          divulgeSolutions(results, numMatches);
      }

    return 0;
}