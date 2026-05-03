#ifndef BST_H
#define BST_H
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
//Flight info structure
struct Flight
{
    string id;             //(e.g., PK108)
    string depart;         //HH:MM
    double duration;       //hours(e.g., 4, 2.5)
    string status;         //On-time/Delayed/Cancelled
};
//BSTNode structure
struct BSTNode
{
    Flight flight;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const Flight& f) : flight(f), left(nullptr), right(nullptr) {}
};
//BST Class
class BST
{
private:
    BSTNode* root;
    //Insert Flight
    BSTNode* insertRec(BSTNode* node, const Flight& f)
    {
        if (!node)
            return new BSTNode(f);
        if (f.id < node->flight.id) node->left = insertRec(node->left, f);
        else if (f.id > node->flight.id) node->right = insertRec(node->right, f);
        return node;
    }
    //Finding minimum node
    BSTNode* findMin(BSTNode* node)
    {
        while (node && node->left)node = node->left;
        return node;
    }
    //Deleting flight
    BSTNode* deleteRec(BSTNode* node, const string& id) 
    {
        if (!node) return nullptr;
        if (id < node->flight.id) node->left = deleteRec(node->left, id);
        else if (id > node->flight.id) node->right = deleteRec(node->right, id);
        else
        {
            if (!node->left)
            {
                BSTNode* r = node->right;
                delete node;
                return r;
            }
            else if (!node->right)
            {
                BSTNode* l = node->left;
                delete node;
                return l;
            }
            else 
            {
                BSTNode* succ = findMin(node->right);
                node->flight = succ->flight;
                node->right = deleteRec(node->right, succ->flight.id);
            }
        }
        return node;
    }
    //Searching flight
    BSTNode* searchRec(BSTNode* node, const string& id)
    {
        if (!node) return nullptr;
        if (id == node->flight.id) return node;
        if (id < node->flight.id) return searchRec(node->left, id);
        return searchRec(node->right, id);
    }
    //Inorder Traversal
    void inorderRec(BSTNode* node)
    {
        if (!node) return;
        inorderRec(node->left);
        cout << node->flight.id << "\t" << node->flight.depart << "\t"
            << node->flight.duration << "\t" << node->flight.status << "\n";
        inorderRec(node->right);
    }
    //for free memory
    void freeRec(BSTNode* node)
    {
        if (!node) return;
        freeRec(node->left);
        freeRec(node->right);
        delete node;
    }
    //Save to file
    void saveRec(BSTNode* node, ofstream& out) 
    {
        if (!node) return;
        saveRec(node->left, out);
        out << node->flight.id << "\t" << node->flight.depart << "\t"
            << node->flight.duration << "\t" << node->flight.status << "\n";
        saveRec(node->right, out);
    }
public:
    BST() : root(nullptr) {}
    ~BST()
    {
        freeRec(root);
    }
    void insert(const Flight& f) 
    {
        root = insertRec(root, f);
    }
    void remove(const string& id) 
    {
        root = deleteRec(root, id);
    }
    BSTNode* search(const string& id)
    {
        return searchRec(root, id);
    }

    void displayInorder()
    {
        cout << "FlightID\tDepart\tDuration\tStatus\n";
        cout << "==============================================\n";
        inorderRec(root);
    }
    //loading data from file
    void loadFromFile(const string& fname) 
    {
        ifstream in(fname.c_str());
        if (!in) return;
        while (true) {
            Flight f;
            if (!(in >> f.id >> f.depart >> f.duration >> f.status)) break;
            insert(f);
        }
        in.close();
    }
   
    void saveToFile(const string& fname) 
    {
        ofstream out(fname.c_str());
        if (!out) { cerr << "Cannot write to " << fname << "\n"; return; }
        saveRec(root, out);
        out.close();
    }
};

#endif 
