// Flight Reservation Directory(Lab-3).cpp 

#include<iostream>
#include<string>
#include<iomanip>
#include<map>
#include<vector>
using namespace std;

enum relationType{LESS, GREATER, EQUAL};

class Flight
{
private:
	int flightID;
	string destination;
	string departureTime;
	double duration;
	string status;
public:
	Flight() 
	{
		flightID = 0;
		destination = " ";
		departureTime = " ";
		duration = 0;
		status = " ";
	}
	Flight(int id, string dest, string dep, double dur, string stat)
	{
		flightID = id;
		destination = dest;
		departureTime = dep;
		duration = dur;
		status = stat;
	}
	void setData(int id, string dest, string dep, double dur, string stat)
	{
		flightID = id;
		destination = dest;
		departureTime = dep;
		duration = dur;
		status = stat;
	}
	int getFlightID() const
	{
		return flightID;
	}
	string getDestination() const
	{
		return destination;
	}
	string getDepartureTime() const
	{
		return departureTime;
	}
	double getDuration() const
	{
		return duration;
	}
	string getStatus() const
	{
		return status;
	}
	relationType compareTo(const Flight& other) const
	{
		if (flightID < other.flightID)
			return LESS;
		else if (flightID > other.flightID)
			return GREATER;
		else return EQUAL;
	}
	void displayFlight() const 
	{
		cout << flightID << "      | " << destination << "        | "
			<< departureTime << "        | " << duration << "        | "
			<< status << endl;
	}
};

struct Node
{
	Flight flight;
	Node* left;
	Node* right;
	Node(Flight f)
	{
		flight = f;
		left = right = nullptr;
	}
};

class FlightBST
{
private:
	Node* root;
	Node* insert(Node* node, Flight f)
	{
		if (node == nullptr)
			return new Node(f);
		if (f.compareTo(node->flight) == LESS)
			node->left = insert(node->left, f);
		else if (f.compareTo(node->flight) == GREATER)
			node->right = insert(node->right, f);
		else
			cout << "Duplicate Flight ID not allowed!" << endl;
		return node;
	}
	Node* findMin(Node* node)
	{
		while (node && node->left != nullptr)
			node = node->left;
		return node;
	}
	Node* transplant(Node* root, Node* u, Node* v)
	{
		if (u == nullptr) return root;
		if (root == u)
			return v;
		Node* parent = root;
		while (parent) 
		{
			if (parent->left == u)
			{
				parent->left = v;
				break;
			}
			else if (parent->right == u) 
			{
				parent->right = v;
				break;
			}
			else if (u->flight.getFlightID() < parent->flight.getFlightID())
				parent = parent->left;
			else
				parent = parent->right;
		}
		return root;
	}
	Node* deleteNode(Node* node, int id) 
	{
		if (node == nullptr)
			return node;
		if (id < node->flight.getFlightID())
			node->left = deleteNode(node->left, id);
		else if (id > node->flight.getFlightID())
			node->right = deleteNode(node->right, id);
		else 
		{
			if (node->left == nullptr)
				return transplant(root, node, node->right);
			else if (node->right == nullptr)
				return transplant(root, node, node->left);
			Node* temp = findMin(node->right);
			node->flight = temp->flight;
			node->right = deleteNode(node->right, temp->flight.getFlightID());
		}
		return node;
	}
	Node* search(Node* node, int id)
	{
		if (node == nullptr || node->flight.getFlightID() == id)
			return node;
		if (id < node->flight.getFlightID())
			return search(node->left, id);
		else
			return search(node->right, id);
	}
	void inorder(Node* node)
	{
		if (node)
		{
			inorder(node->left);
			node->flight.displayFlight();
			inorder(node->right);
		}
	}
	void collectByDestination(Node* node, map<string, vector<Flight>>& groups) 
	{
		if (node)
		{
			collectByDestination(node->left, groups);
			groups[node->flight.getDestination()].push_back(node->flight);
			collectByDestination(node->right, groups);
		}
	}
	void collectByStatus(Node* node, map<string, vector<Flight>>& groups) 
	{
		if (node) 
		{
			collectByStatus(node->left, groups);
			groups[node->flight.getStatus()].push_back(node->flight);
			collectByStatus(node->right, groups);
		}
	}
	void displayETA(Node* node) 
	{
		if (node)
		{
			displayETA(node->left);
			string depTime = node->flight.getDepartureTime();
			int hour = stoi(depTime.substr(0, 2));
			int minute = stoi(depTime.substr(3, 2));
			double dur = node->flight.getDuration();
			int addH = static_cast<int>(dur);
			int addM = static_cast<int>((dur - addH) * 60);
			minute += addM;
			hour += addH + (minute / 60);
			minute %= 60;
			hour %= 24;
			string eta = (hour < 10 ? "0" + to_string(hour) : to_string(hour)) + ":" +
				(minute < 10 ? "0" + to_string(minute) : to_string(minute));
			string punctuality = "On-Time";
			if (node->flight.getStatus() == "Early") punctuality = "Early";
			else if (node->flight.getStatus() == "Delayed") punctuality = "Delayed";
			cout << left << setw(10) << node->flight.getFlightID()
				<< setw(10) << node->flight.getDepartureTime()
				<< setw(10) << node->flight.getDuration()
				<< setw(10) << eta
				<< setw(10) << punctuality << endl;
			displayETA(node->right);
		}
	}
//surprise feature(This function checks flights b\w two IDs)
	void displayFlightsInRange(Node* node, int low, int high)
	{
		if (node == nullptr)
			return;
		if (low < node->flight.getFlightID())
			displayFlightsInRange(node->left, low, high);
		if (node->flight.getFlightID() >= low && node->flight.getFlightID() <= high)
			node->flight.displayFlight();
		if (high > node->flight.getFlightID())
			displayFlightsInRange(node->right, low, high);
	}
public:
	FlightBST()
	{
		root = nullptr;
	}
	void insertFlight(Flight f)
	{
		root = insert(root, f);
	}
	void deleteFlight(int id)
	{
		root = deleteNode(root, id);
	}
	void searchFlight(int id)
	{
		Node* result = search(root, id);
		if (result)
			result->flight.displayFlight();
		else
			cout << "Flight is not found!" << endl;
	}
	void displayAll()
	{
		if (root == nullptr)
			cout << "No flights available." << endl;
		else
		{
			cout << "\nAll Flights (sorted by ID):\n";
			cout << left << setw(10) << "ID" << setw(15) << "Destination"
				<< setw(10) << "Depart" << setw(10) << "Dur(hr)"
				<< setw(10) << "Status" << endl;
			cout << "-----------------------------------------------------------"<<endl;
			inorder(root);
		}
	}
	void displayGroupedByDestination()
	{
		map<string, vector<Flight>> groups;
		collectByDestination(root, groups);
		for (auto& pair : groups)
		{
			cout << "\nDestination: " << pair.first << endl;
			cout << left << setw(10) << "ID" << setw(15) << "Destination"
				<< setw(10) << "Depart" << setw(10) << "Dur(hr)"
				<< setw(10) << "Status" << endl;
			for (auto& f : pair.second)
				f.displayFlight();
		}
	}
	void displayGroupedByStatus()
	{
		map<string, vector<Flight>> groups;
		collectByStatus(root, groups);
		for (auto& pair : groups) 
		{
			cout << "\nStatus Group: " << pair.first << endl;
			cout << left << setw(10) << "ID" << setw(15) << "Destination"
				<< setw(10) << "Depart" << setw(10) << "Dur(hr)"
				<< setw(10) << "Status" << endl;
			for (auto& f : pair.second)
				f.displayFlight();
		}
	}
	void displayAllETA()
	{
		cout << "\nETA & Punctuality Report:\n";
		cout << left << setw(10) << "ID" << setw(10) << "Depart"
			<< setw(10) << "Dur" << setw(10) << "ETA"
			<< setw(10) << "Status" << endl;
		cout << "-----------------------------------------------------------"<<endl;
		displayETA(root);
	}
	void displayFlightsBetweenIDs(int low, int high) 
	{
		cout << "\nFlights with IDs between " << low << " and " << high << ":\n";
		cout << left << setw(10) << "ID" << setw(15) << "Destination"
			<< setw(10) << "Depart" << setw(10) << "Dur(hr)"
			<< setw(10) << "Status" << endl;
		displayFlightsInRange(root, low, high);
	}
};
// time format validation (HH:MM)
bool isValidTime(const string& time)
{
	if (time.size() != 5 || time[2] != ':')
		return false;
	int h = stoi(time.substr(0, 2));
	int m = stoi(time.substr(3, 2));
	return (h >= 0 && h < 24 && m >= 0 && m < 60);
}

int main()
{
	FlightBST bst;
	int choice;

	do
	{
		cout << "\n-----Flight Reservation Directory-----\n";
		cout << "1. Insert new flight record" << endl;
		cout << "2. Delete a flight record" << endl;
		cout << "3. Search for a flight" << endl;
		cout << "4. Display all flights" << endl;  //sorted by flight ID
		cout << "5. Display flights grouped by destination" << endl;
		cout << "6. Display flights grouped by status" << endl;
		cout << "7. Display ETA and punctuality report" << endl;
		cout << "8. Surprise Feature: Display flights between two flight IDs" << endl;
		cout << "9. Exit" << endl;
		
		while (true) 
		{
			cout << "Enter your choice: ";
			cin >> choice;
			if (cin.fail() || choice < 1 || choice > 9) 
			{
				cin.clear();                   
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
				cout << "Invalid choice! Enter a number between 1 and 9."<<endl;
			}
			else 
			{
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
				break;  
			}
		}
		if (choice == 1)
		{
			int id;
			double dur;
			string dest, dep, stat;
			while (true) {
				cout << "Enter flight ID: ";
				cin >> id;
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid input! Please enter a numeric Flight ID."<<endl;
				}
				else
				{
					cin.ignore(10000, '\n');
					break;
				}
			}
			cout << "Enter Destination : ";
			cin >> dest;
			cout << "Enter Departure Time: ";
			cin >> dep;
			while (!isValidTime(dep)) 
			{
				cout << "Invalid format! Re-enter (HH:MM): ";
				cin >> dep;
			}
			while (true)
			{
				cout << "Enter Duration (hrs): ";
				cin >> dur;
				if (cin.fail() || dur < 0)
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid input! Enter a positive number." << endl;
				}
				else
				{
					cin.ignore(10000, '\n');
					break;
				}
			}
			cout << "Enter Status(Landed, Departed, Delayed): ";
			cin >> stat;
			Flight f(id, dest, dep, dur, stat);
			bst.insertFlight(f);
		}
		else if (choice == 2)
		{
			int id;
			while (true)
			{
				cout << "Enter flight ID: ";
				cin >> id;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid input! Please enter a numeric Flight ID."<<endl;
				}
				else 
				{
					cin.ignore(10000, '\n');
					break;
				}
			}
			bst.deleteFlight(id);
		}
		else if (choice == 3)
		{
			int id;
			while (true)
			{
				cout << "Enter flight ID: ";
				cin >> id;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid input! Please enter a numeric Flight ID."<<endl;
				}
				else 
				{
					cin.ignore(10000, '\n');
					break;
				}
			}
			bst.searchFlight(id);
		}
		else if (choice == 4)
		{
			bst.displayAll();
		}
		else if (choice == 5)
		{
			bst.displayGroupedByDestination();
		}
		else if (choice == 6)
		{
			bst.displayGroupedByStatus();
		}
		else if (choice == 7)
		{
			bst.displayAllETA();
		}
		else if (choice == 8)
		{
			int low, high;
			while (true) {
				cout << "Enter lower Flight ID: ";
				cin >> low;
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid input! Enter a numeric Flight ID."<<endl;
				}
				else
				{
					cin.ignore(10000, '\n');
					break;
				}
			}
			while (true) 
			{
				cout << "Enter higher Flight ID: ";
				cin >> high;
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid input! Enter a numeric Flight ID."<<endl;
				}
				else 
				{
					cin.ignore(10000, '\n');
					break;
				}
			}
			bst.displayFlightsBetweenIDs(low, high);
		}
	} while (choice != 9);
	cout << "Exiting the program." << endl;
	return 0;
}