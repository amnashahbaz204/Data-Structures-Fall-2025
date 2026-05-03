Flight Management System (DSA Term Project)

Overview:

*This project is a Flight Management System developed in C++ to demonstrate the use of different data structures including Binary Search Tree (BST), AVL Tree, and Graph (BFS). *The system allows users to manage flight data, compare search performance, and plan trips between cities.

Data Structures Used:

Binary Search Tree (BST)
AVL Tree (Self-balancing BST)
Graph (BFS Traversal)

Modules:

1. Flight Database (BST):
   
Store flight records using Flight ID as key
Insert, delete, search operations
Display flights in sorted order (Inorder traversal)
File handling using flights.txt

2. Flight Database (AVL):
   
Balanced tree implementation
Rotations (LL, RR, LR, RL)
Count number of rotations
Compare search time with BST

3. Trip Planning (Graph - BFS):
   
Airports as vertices
Routes as edges (adjacency matrix)
Find minimum stops between airports
Display route path and traversal table

Features:

Add, search, and delete flights
Display flights using BST and AVL
Compare performance (BST vs AVL)
Plan trips between cities using BFS
File handling for persistent data

Files Included:

main.cpp
bst.h
avl.h
bfs.h
flights.txt
airports.txt
routes.txt

Sample Functionalities:

Sorted flight display
AVL rotations tracking
Minimum stops calculation
Route path display

Interface:

Menu-driven console application:
1. Add New Flight 2. Search Flight 3. Delete Flight 4. Display Flights (BST) 5. Display Flights (AVL) 6. Compare Search Time 7. Plan Trip (BFS) 8. Exit

Language:

C++
