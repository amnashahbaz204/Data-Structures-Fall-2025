// Assignment1DSA.cpp 

#include <iostream>
#include<string>
#include<cctype>
#include<fstream>

using namespace std;

const int MAX = 50;
const int PhoneLength = 11;

enum RelationType { LESS, GREATER, EQUAL };

class ItemType {
private:
	string name;
	string phoneNo;
	string address;
public:

//Setters
	void setName(string n)
	{
		name = n;
	}
	void setPhoneNo(string p) 
	{
		phoneNo = p; 
	}
	void setAddress(string a)
	{
		address = a; 
	}

//Getters
	string getName()const
	{
		return name;
	}
	string getPhoneNo()const
	{
		return phoneNo;
	}
	string getAddress()const 
	{
		return address;
	}

	RelationType comparedTo(const ItemType& other) const
	{
		if (name < other.name)
			return LESS;
		else if (name > other.name)
			return GREATER;
		else
			return EQUAL;
	}
};

class  SortedType {
private:
	ItemType contacts[MAX];
	int length;
public:
	SortedType() {
		length = 0;
	}
	bool isFull() {
		return length == MAX;
	}

//Adding contacts into the sorted list
	void AddContact(ItemType c) {
		if (isFull()) {
			cout << "List is full!" << endl;
			return ;
		}
		int i = length - 1;
		while (i >= 0 && c.comparedTo(contacts[i]) == LESS) {
			contacts[i + 1] = contacts[i];
			i--;
		}
		contacts[i + 1] = c;
		length++;
	}

//Searching contacts from the list
	void SearchContact(string name) {
		bool found = false;
		for (int i = 0; i < length; i++) {
			if (contacts[i].getName() == name) {
				cout << "Name: " << contacts[i].getName() << endl;
				cout << "Phone Number: " << contacts[i].getPhoneNo() << endl;
				cout << "Address: " << contacts[i].getAddress() << endl;
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "Contact not found!" << endl;
		}
	}
//Deleting contacts from the list
	void DeleteContact(string name) {
		int pos = -1;
		for (int i = 0; i < length; i++) {
			if (contacts[i].getName() == name) {
				pos = i;
				break;
			}
		}
		if (pos == -1) {
			cout << "Contact not found!" << endl;
			return;
		}
		for (int i = pos; i < length - 1; i++) {
			contacts[i] = contacts[i + 1];
		}
		length--;
		cout << "Contact deleted!" << endl;
	}

	void DisplayContacts() {
		if (length==0) {
			cout << "No contacts found to display!" << endl;
			return;
		}
		cout << "\nCONTACT LIST\n";
		for (int i = 0; i < length; i++) {
			cout << "Name: " << contacts[i].getName() << endl;
			cout << "Phone Number: " << contacts[i].getPhoneNo() << endl;
			cout << "Address: " << contacts[i].getAddress() << endl;
			cout << "-----" << endl;
		}
	}
//length for file handling
	int getLength() const {
		return length;
	}
	ItemType getContact(int index) const {
		return contacts[index];
	}
};

//Functions to verify the input validations
bool isValidName(const string& name) {
	if (name.empty())
		return false;
	for (char c : name) {
		if (!isalpha(c) && c != ' ') {
			return false;
		}
	}
	return true;
}

bool isValidPhoneNo(const string& phone) {
	if (phone.length() != PhoneLength)
		return false;
	for (char c : phone) {
		if (!isdigit(c))
			return false;
	}
	return true;
}

bool isValidAddress(const string& address) {
	return !address.empty();
}

//saving contacts to file
void saveContacts(const SortedType& list) {
	ofstream outFile("contacts.txt");
	if (!outFile) {
		cout << "Error saving contacts to file." << endl;
		return;
	}
	for (int i = 0; i < list.getLength(); i++) {
		ItemType c = list.getContact(i);
		outFile << c.getName() << "," << c.getPhoneNo() << "," << c.getAddress() << endl;
	}
	outFile.close();
	cout << "Contacts saved to file:)" << endl;
}

//loading contacts from file
void loadContacts(SortedType& list) {
	ifstream inFile("contacts.txt");
	if (!inFile) {			//file doesn't exist
		return;
	}
	string name, phoneNo, address;
	while (getline(inFile, name, ',')) {
		if (getline(inFile, phoneNo, ',') &&  getline(inFile, address)) {
			if (isValidName(name) && isValidPhoneNo(phoneNo) && isValidAddress(address)) {
			ItemType c;
			c.setName(name);
			c.setPhoneNo(phoneNo);
			c.setAddress(address);
			list.AddContact(c);
		    }
		}
	}
	inFile.close();
	cout << "Contacts loaded from file:)" << endl;
}

//Main Function
int main() {
	SortedType contactList;
	loadContacts(contactList);
	int choice;

	do {
		cout << "////////////////////////////" << endl;
		cout << "!...Main Menu...!" << endl;
		cout << "1. Add Contact" << endl;
		cout << "2. Search Contact" << endl;
		cout << "3. Delete Contact" << endl;
		cout << "4. Display all Contacts" << endl;
		cout << "5. Exit" << endl;
		cout << "Enter your choice(1-5)" << endl;
		cin >> choice;
		cin.ignore();

		if (choice >= 1 && choice <= 5) {
			if (choice == 1) {
				string name, phoneNo, address;
//names must be alphabets only
				do {
					cout << "Enter Name: ";
					getline(cin, name);
					if (!isValidName(name)) {
						cout << "Invalid Name! Please Try Again:)" << endl;
					}
				} while (!isValidName(name));
//phone number must be of 11 digits
				do {
					cout << "Enter Phone number (11-digits): ";
					getline(cin, phoneNo);
					if (!isValidPhoneNo(phoneNo)) {
						cout << "Phone number is invalid! Please try again:)" << endl;
					}
				} while (!isValidPhoneNo(phoneNo));
//address will not be empty
				do {
					cout << "Enter Address: ";
					getline(cin, address);
					if (address.empty()) {
						cout << "Address cannot be empty! Try again:)" << endl;
					}
				} while (!isValidAddress(address));
//making object of class ItemType
				ItemType c;
				c.setName(name);
				c.setPhoneNo(phoneNo);
				c.setAddress(address);
				contactList.AddContact(c);
				cout << "Contact added!...:)" << endl;
				saveContacts(contactList);
			}
//checking other choices of the menu
			//2 is to search the contact by name
			else if (choice == 2) {
				string name;
				cout << "Enter name to search: " << endl;
				getline(cin, name);
				contactList.SearchContact(name);
			}
			//3 is to delete the contact
			else if (choice == 3) {
				string name;
				cout << "Enter name to delete: " << endl;
				getline(cin, name);
				contactList.DeleteContact(name);
				saveContacts(contactList);
			}
			//enter 4 to display the contacts
			else if (choice == 4) {
				contactList.DisplayContacts();
			}
			//finally 5 is for exit
			else if (choice == 5) {
				saveContacts(contactList);
				cout << "Exiting the program:(" << endl;
			}
		}
//if we enter any other digit than 1-5 then it gives us invalid choice
			else {
				cout << "Invalid choice!" << endl;
			}
		} while (choice != 5);

		return 0;
	}