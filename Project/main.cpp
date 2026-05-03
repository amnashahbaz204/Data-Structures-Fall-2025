#include <iostream>
#include <fstream>
#include <ctime>
#include "bst.h"
#include "avl.h"
#include "bfs.h"
using namespace std;
//to check the validation of our time format(will be in HH:MM)
bool isValidTime(const string& t)
{
    if (t.size() != 5 || t[2] != ':')
        return false;
    int hh = atoi(t.substr(0, 2).c_str());
    int mm = atoi(t.substr(3, 2).c_str());
    //Check valid hours (must be in b/w 0-23)
    if (hh < 0 || hh > 23)
        return false;
    return true;
}
//Convert time to next if minutes overflow (like 04:76 to 05:16)
string convertTime(string t)
{
    int hh = atoi(t.substr(0, 2).c_str());
    int mm = atoi(t.substr(3, 2).c_str());
    //If minutes >= 60, convert to hours
    if (mm >= 60)
    {
        hh += mm / 60;
        mm = mm % 60;
    }
    //If hours >= 24, it goes to next day (reset to 0-23)
    hh = hh % 24;
    //Format back to HH:MM
    string result = "";
    if (hh < 10) result += "0";
    result += to_string(hh);
    result += ":";
    if (mm < 10) result += "0";
    result += to_string(mm);
    return result;
}
//displaying our main menu options
void printMenu() 
{
    cout << "\n----------------------------------\n";
    cout << "*****Flight Management System*****\n";
    cout << "----------------------------------\n";
    cout << "1. Add New Flight (BST)"<<endl;
    cout << "2. Search Flight (BST)"<<endl;
    cout << "3. Delete Flight (BST)"<<endl;
    cout << "4. Display All Flights (BST)"<<endl;
    cout << "5. Display Flights (AVL) + Rotation Info"<<endl;
    cout << "6. Compare Search Time (BST vs AVL)"<<endl;
    cout << "7. Plan Trip from Airport A to B"<<endl;
    cout << "8. Exit"<<endl;
    cout << "Enter your choice: ";
}
//main function
int main() 
{
    BST bst;
    AVL avl;
    Graph graph;
    // loading data from our files
    bst.loadFromFile("flights.txt");
    avl.loadFromFile("flights.txt");
    graph.loadAirports("airports.txt");
    graph.loadRoutes("routes.txt");

    int choice;
    while (true) 
    {
        printMenu();
        if (!(cin >> choice)) 
        { 
            cin.clear(); cin.ignore(10000,'\n');
            cout << "Invalid input\n";
            continue; 
        }
        //Add Flight
        if (choice == 1)
        {
            Flight f;
            cout << "Enter Flight ID (e.g. PK107): "; 
            cin >> f.id;
            cout << "Enter Departure (HH:MM): ";
            cin >> f.depart;
            while (!isValidTime(f.depart)) 
            {
                cout << "Invalid time. Please Re-enter in the correct format(HH:MM): "; 
                cin >> f.depart;
            }
            //Convert time if minutes overflow
            f.depart = convertTime(f.depart);
            cout << "Converted time: " << f.depart << endl;
            cout << "Enter Duration (hours e.g. 2 or 9.5): "; 
            cin >> f.duration;
            cout << "Enter Status (On-time/Delayed/Cancelled): ";
            cin >> f.status;
            bst.insert(f);
            avl.insert(f);
            bst.saveToFile("flights.txt"); 
            cout << "Flight inserted."<<endl;
        }
        //Search Flight
        else if (choice == 2)
        {
            string id; cout << "Enter Flight ID: "; 
            cin >> id;
            clock_t t1 = clock();
            BSTNode* n = bst.search(id);
            clock_t t2 = clock();
            if (n) cout << "Found (BST): " << n->flight.id << " " << n->flight.depart << " " << n->flight.duration << " " << n->flight.status << "\n";
            else cout << "Not found in BST"<<endl;
            double timeBST = double(t2 - t1) / CLOCKS_PER_SEC;
            cout << "BST search time: " << timeBST << " seconds"<<endl;
        }
        //Delete Flight
        else if (choice == 3)
        {
            string id; 
            cout << "Enter Flight ID to delete: ";
            cin >> id;
            bst.remove(id);
            avl.remove(id);
            bst.saveToFile("flights.txt");
            cout << "Delete attempted (if existed)."<<endl;
        }
        //Display BST
        else if (choice == 4) 
        {
            bst.displayInorder();
        }
        //Display AVL with rotations
        else if (choice == 5)
        {
            avl.displayInorder();
            cout << "AVL rotations: " << avl.getRotationCount() << "\n";
        }
        //Comparing the search times
        else if (choice == 6) 
        {
            string id; 
            cout << "Enter Flight ID to compare search times: ";
            cin >> id;
            clock_t s1 = clock();
            BSTNode* bres = bst.search(id);
            clock_t e1 = clock();
            clock_t s2 = clock();
            AVLNode* ares = avl.search(id);
            clock_t e2 = clock();
            double tbst = double(e1 - s1) / CLOCKS_PER_SEC;
            double tavl = double(e2 - s2) / CLOCKS_PER_SEC;
            //BST result
            string bstResult;
            if (bres) bstResult = "Found";
            else bstResult = "Not Found";
            cout << "BST: " << bstResult << ", time=" << tbst << "s\n";
            //AVL result
            string avlResult;
            if (ares) avlResult = "Found";
            else avlResult = "Not Found";
            cout << "AVL: " << avlResult << ", time=" << tavl << "s\n";
        }
        //Planning trip using BFS
        else if (choice == 7)
        {
            cout << "Airports list:\n";
            graph.displayAirports();
            int s,d;
            cout << "Enter source index: ";
            cin >> s; 
            cout << "Enter destination index: ";
            cin >> d;
            graph.BFS(s,d);
        }
        //Exit
        else if (choice == 8)
        {
            bst.saveToFile("flights.txt");
            cout << "Flights saved. Exiting.\n";
            break;
        }
        else 
        {
            cout << "Invalid choice\n";
        }
    }

    return 0;
}
