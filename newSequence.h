#ifndef newSequence_h
#define newSequence_h

#include <stdio.h>
#include <iostream>
#include <string>

using ItemType = unsigned long;
const int DEFAULT_MAX_ITEMS = 180; //Question 3

class Sequence
{
public:
    Sequence();    // Create an empty sequence (i.e., one whose size() is 0).
    Sequence(int size);
    Sequence(const Sequence& other);
    ~Sequence();        //destructor
    Sequence& operator=(const Sequence& rhs);

    bool empty() const;  // Return true if the sequence is empty, otherwise false.
    int  size() const;    // Return the number of items in the sequence.
    int  insert(int pos, const ItemType& value);
    int  insert(const ItemType& value);
    bool erase(int pos);
    int  remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int  find(const ItemType& value) const;
    void swap(Sequence& other);
    void dump() const;

private:
    int         m_size;
    int         m_decSize;
    ItemType* m_arr;

};


#endif //newSequence_h