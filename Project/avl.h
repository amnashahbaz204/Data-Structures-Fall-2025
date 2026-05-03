#pragma once
#ifndef AVL_H
#define AVL_H
#include<iostream>
#include<string>
#include "bst.h"
using namespace std;
//AVL Tree Node with height
struct AVLNode
{
    Flight flight;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(const Flight& f) : flight(f), left(nullptr), right(nullptr), height(1) {}
};
//Balanced AVL Tree class
class AVL
{
private:
    AVLNode* root;
    int rotations;
    //Height of node
    int height(AVLNode* n)
    {
        if (n) return n->height;
        return 0;
    }
    //Update height of node
    void update(AVLNode* n)
    {
        if (n) n->height = 1 + max(height(n->left), height(n->right));
    }
    //Calculating the balance factor
    int balance(AVLNode* n)
    {
        if (n) return height(n->left) - height(n->right);
        return 0;
    }
    //right rotation
    AVLNode* rightRotate(AVLNode* y)
    {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        update(y); update(x);
        rotations++;
        return x;
    }
    //left rotation
    AVLNode* leftRotate(AVLNode* x)
    {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        update(x); update(y);
        rotations++;
        return y;
    }
    //inserting with maintaining balance
    AVLNode* insertRec(AVLNode* node, const Flight& f)
    {
        if (!node)
            return new AVLNode(f);
        if (f.id < node->flight.id) 
            node->left = insertRec(node->left, f);
        else if (f.id > node->flight.id) 
            node->right = insertRec(node->right, f);
        else 
            return node;
        update(node);
        int bf = balance(node);
        //LL case
        if (bf > 1 && f.id < node->left->flight.id)
            return rightRotate(node);  
        //RR case
        if (bf < -1 && f.id > node->right->flight.id)
            return leftRotate(node);            
        //LR case
        if (bf > 1 && f.id > node->left->flight.id)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        } 
        //RL case
        if (bf < -1 && f.id < node->right->flight.id)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }
    //Find minimum node
    AVLNode* findMin(AVLNode* node)
    {
        while (node && node->left) node = node->left;
        return node;
    }
    //deleting while maintaining balance
    AVLNode* deleteRec(AVLNode* node, const string& id) 
    {
        if (!node) 
            return nullptr;
        if (id < node->flight.id) 
            node->left = deleteRec(node->left, id);
        else if (id > node->flight.id)
            node->right = deleteRec(node->right, id);
        else
        {
            if (!node->left || !node->right)
            {
                AVLNode* tmp = node->left ? node->left : node->right;
                if (!tmp)
                {
                    delete node; 
                    return nullptr;
                }
                else 
                {
                    *node = *tmp; 
                    delete tmp;
                }
            }
            else 
            {
                AVLNode* tmp = findMin(node->right);
                node->flight = tmp->flight;
                node->right = deleteRec(node->right, tmp->flight.id);
            }
        }
        if (!node)
            return node;
        update(node);
        int bf = balance(node);
        if (bf > 1 && balance(node->left) >= 0) 
            return rightRotate(node);
        if (bf > 1 && balance(node->left) < 0)
        {
            node->left = leftRotate(node->left); 
            return rightRotate(node);
        }
        if (bf < -1 && balance(node->right) <= 0) 
            return leftRotate(node);
        if (bf < -1 && balance(node->right) > 0)
        {
            node->right = rightRotate(node->right); 
            return leftRotate(node);
        }
        return node;
    }
    //searching recursively
    AVLNode* searchRec(AVLNode* node, const string& id) 
    {
        if (!node) 
            return nullptr;
        if (id == node->flight.id) 
            return node;
        if (id < node->flight.id) 
            return searchRec(node->left, id);
        return searchRec(node->right, id);
    }
    //Inorder traversal
    void inorderRec(AVLNode* node) 
    {
        if (!node) 
            return;
        inorderRec(node->left);
        cout << node->flight.id << "\t" << node->flight.depart << "\t"
            << node->flight.duration << "\t" << node->flight.status << "\n";
        inorderRec(node->right);
    }
    //free memory
    void freeRec(AVLNode* node) 
    {
        if (!node)
            return;
        freeRec(node->left);
        freeRec(node->right);
        delete node;
    }

public:
    AVL() : root(nullptr), rotations(0) {}
    ~AVL()
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
    AVLNode* search(const string& id) 
    { 
        return searchRec(root, id);
    }
    void displayInorder() 
    {
        cout << "FlightID\tDepart\tDuration\tStatus\n";
        cout << "=============================================\n";
        inorderRec(root);
    }
    int getRotationCount() const 
    {
        return rotations;
    }
    //loading from file
    void loadFromFile(const string& fname)
    {
        ifstream in(fname.c_str());
        if (!in) return;
        while (true) 
        {
            Flight f;
            if (!(in >> f.id >> f.depart >> f.duration >> f.status)) break;
            insert(f);
        }
        in.close();
    }
};

#endif 
