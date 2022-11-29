
#include "WeddingGuest.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
using namespace std;

WeddingGuest::WeddingGuest() { // constructor
	head = nullptr;
	m_size = 0;					//set list size to 0
	//head->prev = nullptr;     //head points to itself because it is a circular linked list
	//head->next = nullptr;
}

WeddingGuest::WeddingGuest(const WeddingGuest& src) { // copy constructor
	
	if (src.head == nullptr) {
		head = nullptr;
		m_size = 0;			//set list size to 0
	}
	else {

		m_size = src.m_size;

		head = new Node;        //create a new node
		head->firstName = src.head->firstName;
		head->lastName = src.head->lastName;
		head->value = src.head->value;

		head->next = src.head->next;      //assign head to point to itself, both next and prev
		head->prev = nullptr;

		//copy in rest of list
		Node* p = head;                     //ptr points to the head of other map
		Node* pSrc = src.head->next;		//pSrc is the NEXT node in the src list. 

		while (pSrc != nullptr) {     //iterate through all items in src list 
			p->next = new Node;
			p->next->lastName = pSrc->lastName;		//copy all values (firstName, lastName, value)
			p->next->firstName = pSrc->firstName;
			p->next->value = pSrc->value;

			p->next->prev = p;
			p->next->next = nullptr;		//IMPORTANT !! setting last node to point to nullptr

			p = p->next;			//iterate through list. 
			pSrc = pSrc->next;
		}
	}	
}

 const WeddingGuest& WeddingGuest::operator=(const WeddingGuest& rhs) { // assignment operator

	if (this != &rhs) { //if this is not equal to other, new tempmap is made. swap this current map and tempmap. 
		WeddingGuest tempWeddingGuest(rhs);
		swapWeddingGuests(tempWeddingGuest);
	}
	return *this;

}

WeddingGuest::~WeddingGuest() { // destructor
	Node* p;
	while (head != nullptr) {
		p = head;
		head = head->next;
		delete p;
	}
}

bool WeddingGuest::noGuests() const { // Return true if the WeddingGuest list is empty, otherwise false.
	return (m_size == 0);
}

int WeddingGuest::guestCount() const { // Return the number of matches on the WeddingGuest list.
	return m_size;
}

bool WeddingGuest::inviteGuest(const std::string& firstName, const std::string& lastName, const GuestType& value) {
	// If the full name is not equal to any full name currently in the list then add it and return
	// true. Otherwise, make no change to the list and return false (indicating that the name is already in the list).
	
	if (invitedToTheWedding(firstName, lastName)) { //already in list
		return false;
	}
	
	//otherwise, ADD IT and return true. create a new node
	m_size++;	// increase size by 1

	Node* p = new Node;
	
	p->firstName = firstName;
	p->lastName = lastName;
	p->value = value;

	//----------------------------------------------------------------------------
	//ADDING to list! CASE 1: FRONT of list. ----------------------------------------------------------
	if ((head == nullptr) || (head->lastName == lastName && head->firstName > firstName) || head->lastName > lastName){ //if list was empty before. 
		
		p->next = head;		// link next and prev

		if (head != nullptr) {
			head->prev = p;
		}
		p->prev = nullptr;
		head = p;			// set new head and return true.
		return true;
	}
	

	else { //CASE 2: MIDDLE OR END of list ----------------------------------------------------------
		Node* it = head;
		while (it->next != nullptr) {
			if (it->lastName == lastName && (it->next->lastName == lastName) && it->next->firstName > firstName) { //e.g. BOB Smith goes between ALEX Smith and CARL Smith
				break;
			}

			else if (it->lastName == lastName && (lastName < it->next->lastName) && (it->firstName < firstName)) { //e.g. Bob Smith goes between Albert Smith and Sam Trebek
				break;
			}

			else if ((it->lastName < lastName) && (it->next->lastName > lastName)) { // Li Robert goes between Zhang Alan and Chen Amanda
				break;
			}

			else if ((it->lastName < lastName) && (it->next->lastName == lastName) && it->next->firstName > firstName){ //first of many Li's after the last Le
				break;
			}			

			it = it->next;
		}

		if (it->next == nullptr) { //if adding to END:
			if (lastName > it->lastName || (lastName == it->lastName && firstName > it->firstName)) {
				it->next = p;

				p->prev = it;
				p->next = nullptr;

				return true;
			}
		}

		p->next = it->next;		//relink nodes in list (setting prev and next)
		p->prev = it;

		if (p->next != nullptr) {
			p->next->prev = p;
		}
		
		it->next = p;
	}

	return true;
	
}

bool WeddingGuest::alterGuest(const std::string& firstName, const std::string& lastName, const GuestType& value) {
	// If the full name is equal to a full name currently in the list, then make that full name no longer map to the value it
	// currently maps to, but instead map to the value of the third parameter; return true in this case. Otherwise, make no change to the list and return false.

	Node* p = head;

	while (p != nullptr) {
		if (p->firstName == firstName && p->lastName == lastName) { //full name match!
			break;
		}

		p = p->next; //iterate through list. 
	}
	if (p != nullptr) { //IMPORTANT! ensure p is not pointing to nullptr. 
		p->value = value;
		return true; 
	}


	return false;		//no change and return false. 
}

bool WeddingGuest::inviteOrAlter(const std::string& firstName, const std::string& lastName, const GuestType& value) {
	// If full name is equal to a name currently in the list, then make that full name no longer map to the value it currently
	// maps to, but instead map to the value of the third parameter. Return true in this case. If the full name is not equal to
	// any full name currently in the list then add it and return true. In fact, this function always returns true.
	if (invitedToTheWedding(firstName, lastName)) {
		alterGuest(firstName, lastName, value);
	}
	else {
		inviteGuest(firstName, lastName, value);
	}

	return true;
}

bool WeddingGuest::crossGuestOff(const std::string& firstName, const std::string& lastName) {
	// If the full name is equal to a full name currently in the list, remove the full name and value from the list and return
	// true. Otherwise, make no change to the list and return false.

	if (!invitedToTheWedding(firstName, lastName)) {			// if not invited or not on guest list, make no change and return false. 
		return false;
	}

	m_size--;		//subtract 1 from number of guests. 

	//otherwise, remove full name (aka remove node). CASE 1: FRONT of list. CASE 2: MIDDLE/END of list
	//CASE 1:------------------- FRONT
	if ((firstName == head->firstName) && (lastName == head->lastName)) {
		
		if (head->next == nullptr) { //if list only contains head. 
			delete head;

			return true;
		}
		
		Node* killMe = head;		// killMe is node to be deleted
		head = killMe->next;		// set head to next
		head->prev = nullptr;		// unlink

		delete killMe;	
		return true;
	}

	//CASE 2:------------------- MIDDLE/END

	Node* p = head; //temp pointer to traverse to entry we want to delete

	while (p != nullptr) {
		if (p->next != nullptr && (p->next->firstName == firstName && p->next->lastName == lastName)) {
			break;
		}
		p = p->next;		

	}

	//at this point, p should be the node BEFORE killMe.

	if (p != nullptr) { //found our value!
		Node* killMe = p->next;

		p->next = killMe->next;		// relinking nodes

		if (killMe->next != nullptr) { //if last node is not nullptr, the list needs to be doubly linked. if last node is nullptr, there is no prev linking to be done.
			killMe->next->prev = p;
		}

		delete killMe;
	}

	return true;

}

bool WeddingGuest::invitedToTheWedding(const std::string& firstName, const std::string& lastName) const {
	// Return true if the full name is equal to a full name currently in the list, otherwise false.
	
	Node* p = head;

	while (p != nullptr) {
		if (firstName == p->firstName && lastName == p->lastName) { //if full name matches, return true. 
			return true;
		}
		
		p = p->next;
	}

	return false;		// no matches after traversing list; false. 
}

bool WeddingGuest::matchInvitedGuest(const std::string& firstName, const std::string& lastName, GuestType& value) const {
	// If the full name is equal to a full name currently in the list, set value to the value in the list that that full name
	// maps to, and return true. Otherwise, make no change to the value parameter of this function and return false.
	if (invitedToTheWedding(firstName, lastName)) {
		Node* p = head;

		while (p != nullptr && (firstName != p->firstName || lastName != p->lastName)) {
			p = p->next;
		} //finding the position of MATCHED name. 

		value = p->value; //set value to the value in the list that that full name maps to. 
		return true;
	}

	return false;		//make no change and return false.
}

bool WeddingGuest::verifyGuestOnTheList(int i, std::string& firstName, std::string& lastName, GuestType& value) const {
	// If 0 <= i < size(), copy into firstName, lastName and value parameters the corresponding information of the element at
	// position i in the list and return true. Otherwise, leave the parameters unchanged and return false. 

	if (0 <= i && i < guestCount()) {
		
		Node* p = head;
		for (int j = 0; j < i; j++) {
			p = p->next;
		}

		firstName = p->firstName;			// copy into firstName, lastName, and value
		lastName = p->lastName;
		value = p->value;

		return true;
	}

	return false;
}

void WeddingGuest::swapWeddingGuests(WeddingGuest& other) {
	// Exchange the contents of this list with the other one.

	Node* temphead = head;      //introduce temporary placeholder pointers head and tail
	int tempsize = m_size;      //introduce tempoary int tempsize to store map size

	m_size = other.m_size;      //assign other size to original map
	other.m_size = tempsize;    //assign original size to other size

	head = other.head;          //assign contents of other to original map
	other.head = temphead;      //assign contents of original map to other
}

bool joinGuests(const WeddingGuest& odOne, const WeddingGuest& odTwo, WeddingGuest& odJoined) {
	
	if (!odJoined.noGuests()) {      //if the joined list is not empty,
		WeddingGuest emptylist;           //create an empty list
		odJoined.swapWeddingGuests(emptylist);  //swap result with empty list
	}
	
	bool valuesSame = true; //this is the value to be returned!

	string tempfirst;
	string templast;
	GuestType tempval;

	GuestType valcheck;

	for (int i = 0; i < odOne.guestCount(); i++) { //loop through all items in odOne

		odOne.verifyGuestOnTheList(i, tempfirst, templast, tempval);	//copy into tempfirst, templast, tempval corresponding information

		if (!odTwo.invitedToTheWedding(tempfirst, templast)) {          //check whether each item is contained in temp2.
			odJoined.inviteGuest(tempfirst, templast, tempval);			//if name is distinct (not in temp2), insert into result
		}

		else if (odTwo.matchInvitedGuest(tempfirst, templast, valcheck)) { //otherwise if values are equal, valcheck will have value of name in the list 
			if (tempval == valcheck) {						//if values are equal, insert into odJoined
				odJoined.inviteGuest(tempfirst, templast, tempval);		
			} 
			else {
				valuesSame = false;			// otherwise, set valuesSame to false and do not put into the combined list. 
			}
			
		}
	}


	for (int j = 0; j < odTwo.guestCount(); j++) {		// loop through all items in odTwo
		
		odTwo.verifyGuestOnTheList(j, tempfirst, templast, tempval);		//get tempval of item and check whether name is already in odJoined

		if (odOne.invitedToTheWedding(tempfirst, templast)) {	//check whether each item is contained in odJoined.
			GuestType tempvalfromOdJoined;

			bool tempbool = odJoined.matchInvitedGuest(tempfirst, templast, tempvalfromOdJoined);	// get tempval of item and check whether name is already in odJoined

			if (tempval != tempvalfromOdJoined) {						//if values are not the same
				valuesSame = false;										//set valuesSame to false and do not put into result
			}
			else {														//otherwise, put into odJoined
				odJoined.inviteGuest(tempfirst, templast, tempval);		
			}
		}
		else {															//if name is not in odJoined, put into odJoined
			odJoined.inviteGuest(tempfirst, templast, tempval);
		}
	}
	
	return valuesSame;
}
void attestGuests(const std::string& fsearch, const std::string& lsearch, const WeddingGuest& odOne, WeddingGuest& odResult) {
	
	if (!odResult.noGuests()) {      //if the result list is not empty,
		WeddingGuest emptylist;           //create an empty list
		odResult.swapWeddingGuests(emptylist);  //swap result with empty list
	}
	
	string tempfirst;
	string templast;
	GuestType tempval;

	if (fsearch == "*" && lsearch == "*") {
		odResult = odOne;
	} 
	else {
		for (int i = 0; i < odOne.guestCount(); i++) { //iterate through odOne

			odOne.verifyGuestOnTheList(i, tempfirst, templast, tempval);

			if (fsearch == "*" && (templast == lsearch)) {
				odResult.inviteGuest(tempfirst, templast, tempval);
			}
			else if(lsearch == "*" && (tempfirst == fsearch)) {
				odResult.inviteGuest(tempfirst, templast, tempval);
			}
			else if (fsearch == tempfirst && lsearch == templast){
				odResult.inviteGuest(tempfirst, templast, tempval);
			}
		}
	}

}

