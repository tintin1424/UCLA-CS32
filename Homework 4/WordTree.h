#ifndef WORDTREE_H
#define WORDTREE_H

#include <iostream>
#include <string>
using namespace std;

typedef std::string WordType;

struct WordNode {
	WordType m_data;
	WordNode* m_left;
	WordNode* m_right;

	int m_count = 1;
	
	WordNode()
		: m_data(""), m_left(nullptr), m_right(nullptr){
	}

	WordNode(WordType val)
		: m_data(val), m_left(nullptr), m_right(nullptr) {
	}
};

class WordTree; //forward declaration
ostream& operator<<(ostream& stream, const WordTree& obj);

class WordTree {

private:
	WordNode* root;
	
	void copyWT(WordNode*& newRoot, WordNode* rhsRoot);
	void deleteTree(WordNode* root);
	int  distinctWordCnt(const WordNode* p) const;
	void displayList(ostream& stream, WordNode* root) const;
	void totalWordsHelper(int& cnt, WordNode* p) const;

public:
	// default constructor
	WordTree() : root(nullptr) { };

	// copy constructor
	WordTree(const WordTree& rhs);

	// assignment operator
	const WordTree& operator=(const WordTree& rhs);

	// Inserts v into the WordTree
	void add(WordType v);
	// Returns the number of distinct words / nodes

	int distinctWords() const;
	// Returns the total number of words inserted, including
	// duplicate values
	int totalWords() const;

	// Prints the WordTree
	friend std::ostream& operator<<(std::ostream& out, const
		WordTree& rhs);
	// Destroys all the dynamically allocated memory in the
	// tree
	~WordTree();
};

#endif WORDTREE_H