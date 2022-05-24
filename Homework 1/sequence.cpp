#include "Sequence.h"
using namespace std;

Sequence::Sequence() {
	m_size = 0;
}

bool Sequence :: empty() const { // Return true if the sequence is empty, otherwise false. 
	if (m_size == 0) {
		return true;
	}
	else {
		return false;
	}

} 
int Sequence :: size() const {    // Return the number of items in the sequence.
	return m_size;
}

int Sequence :: insert(int pos, const ItemType& value) {
	if (0 <= pos && pos <= size()) {

		if (pos == size()) { //value is inserted at the end
			m_size++; //increase size by 1
			m_arr[pos] = value; //add into array

			return pos;
		}

		for (int i = size(); i > pos; i--) { //shift all items back by 1 position
			m_arr[i] = m_arr[i - 1];			
		}

		m_size++; //add into array, increase array size by 1
		m_arr[pos] = value;
		return pos;
	}

	else {
		return -1; //leave sequence unchanged
	}
}

int Sequence::insert(const ItemType& value) {

	//if sequence full, return -1
	if (size() == DEFAULT_MAX_ITEMS) {
		return -1;
	}

	else {
		int p = size();

		for (int i = 0; i < size(); i++) {
			if (value <= m_arr[i]) {
				p = i;
			}
		}

		if (p == size()) {
			m_arr[p] = value; //insert value at position p, increase size of array
			m_size++;
		}
		else { //otherwise, insert in the middle of array and shift all items behind it back by 1
			for (int i = size(); i > p; i--) {
				m_arr[i] = m_arr[i - 1];
			}
			m_arr[p] = value;
			m_size++;
		}
		return p;
	}
}

bool Sequence::erase(int pos) {

	if (0 <= pos && pos < size()) {
		//remove item at position pos from the sequence
		for (int i = pos; i < size(); i++) {
			m_arr[i] = m_arr[i + 1];
		}

		m_size--;
		return true;
	}
	else {
		return false;
	}
}

int Sequence::remove(const ItemType& value) {

	int count = 0;
	int comp = 0;

	for (int i = 0; i < size(); i++) {
		if (m_arr[i] != value) { //if not equal, keep in array
			m_arr[comp] = m_arr[i];
			comp++;
		}
		else {
			count++; //if equal, increase count and do not add back to array
		}
	}

	m_size = m_size - count; //decrease size

	return count;

}

bool Sequence::get(int pos, ItemType& value) const {
	if (0 <= pos && pos < size()) {
		//copy into value the item at position pos and return true
		value = m_arr[pos];
		return true;
	}
	else { //otherwise leave value unchanged and return false.
		return false;
	}
}
bool Sequence::set(int pos, const ItemType& value) {
	if (0 <= pos && pos < size()) {
		//replace item at position pos with value and return true.
		m_arr[pos] = value;
		return true;
	}
	else { //otherwise, leave sequence unchanged and return false;
		return false;
	}
}

int Sequence::find(const ItemType& value) const {
	int p = 0;
	bool found = false;

	for (int i = 0; i < size(); i++) {
		if (m_arr[i] == value) {
			p = i;
			found = true;
		}
	}
	if (found == true) {
		return p;
	}
	else {
		return -1;
	}
}

void Sequence::swap(Sequence& other) {
	int maxSize = size();

	if (size() < other.size()) {
		maxSize = other.size();
	}

	for (int i = 0; i < maxSize; i++) {
		ItemType temp = m_arr[i];
		m_arr[i] = other.m_arr[i];
		other.m_arr[i] = temp;
	}

	int tempSize = m_size;
	m_size = other.m_size;
	other.m_size = tempSize;
}
// Exchange the contents of this sequence with the other one.

void Sequence::dump() const {
	for (int i = 0; i < size(); i++)
	{
		cerr << m_arr[i] << endl;
	}
}