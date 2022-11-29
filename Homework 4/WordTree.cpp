#include "WordTree.h"

void WordTree::copyWT(WordNode*& newRoot, WordNode* rhsRoot) { //copy constructor helper function
	
	if (rhsRoot == nullptr) { //if nullptr, set new root to nullptr also and finish
		newRoot = rhsRoot;
	} else { // if not nullptr, 

		newRoot = new WordNode(rhsRoot->m_data);
		newRoot->m_count = rhsRoot->m_count;

		copyWT(newRoot->m_left, rhsRoot->m_left);
		copyWT(newRoot->m_right, rhsRoot->m_right);
	}
}

WordTree::WordTree(const WordTree& rhs) { //copy constructor
	if (rhs.root == nullptr) {
		this->root = rhs.root;
	}
	else {
		copyWT(root, rhs.root); //helper function
	}

}

const WordTree &WordTree::operator=(const WordTree& rhs) { //assignment operator
	deleteTree(root); //if already filled, delete what is inside lhs

	copyWT(root, rhs.root); //copy over new tree
	return *this;
}


void WordTree::displayList(ostream& stream, WordNode* root) const {

	if (root == nullptr) {
		return;
	}

	//split list into left and right; recursively call displayList on left and right halves 
	displayList(stream, root->m_left);

	//ostream function call
	stream << root->m_data << " " << root->m_count << endl;

	displayList(stream, root->m_right); //print list

	return;
}

ostream& operator<<(ostream & stream, const WordTree& rhs) {
	rhs.displayList(stream, rhs.root);
	return stream;
}

void WordTree::deleteTree(WordNode* p) { //recursively delete tree (left, then right)
	if (p != nullptr) {			//deleting left and right nodes BEFORE
		deleteTree(p->m_right);
		deleteTree(p->m_left);

		delete p;
	}
}

WordTree::~WordTree() { //destructor
	deleteTree(root);
}

//---------------------------------------------------------------------

void WordTree::add(WordType v) {

	if (root == nullptr) {
		root = new WordNode(v);
		return;
	}

	WordNode* cur = root;
	bool loop = true;

	while (loop) {		//continue looping
		if (cur->m_data == v) { //if data is equal to v, increase m_count and return
			cur->m_count++;
			return;
		}

		if (cur->m_data > v) { //if current data > v, shift left 
			if (cur->m_left != nullptr) {
				cur = cur->m_left;
			}
			else {				// when cur->m_left is NULLPTR
				cur->m_left = new WordNode(v);
				return;
			}
		}
		
		if (cur->m_right != nullptr) { // if cur->m_rght is not nullptr, shift right
			cur = cur->m_right;
		}
		else {
			cur->m_right = new WordNode(v); //add new WordNode to right pointer and return
			return;
		}
	}
}


int WordTree::distinctWordCnt(const WordNode* p) const{
	
	if (p == nullptr) return 0;

	//cnt++; //increment by one each time the function is run

	//count words to left and right of trespectively
	int wordsL = distinctWordCnt(p->m_left);
	int wordsR = distinctWordCnt(p->m_right);

	return (1 + wordsL + wordsR);
}

int WordTree::distinctWords() const {

	int cnt = distinctWordCnt(root); //calling helper function
	return cnt;
}

void WordTree::totalWordsHelper(int& cnt, WordNode* p) const {
	
	if (p == nullptr) return;
	
	cnt += p->m_count;

	//recursively:
	totalWordsHelper(cnt, p->m_left); //count total words to the left of p, and right of p separately
	totalWordsHelper(cnt, p->m_right);
}

int WordTree::totalWords() const {

	if (root == nullptr) { //base case
		return 0;
	}

	int cnt = 0;
	totalWordsHelper(cnt, root); // call helper functoin
	return cnt;
}