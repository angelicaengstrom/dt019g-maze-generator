/*
Angelica Engström
anen1805, TDTEA
DT019G
2021-05-20
Betyg: A
*/
#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <stdio.h>

class Maze{
private:
    enum Direction{Right, Left, Up, Down};

    struct node{
        node(bool visited, bool path);
        bool visited;
        bool path;
        node* up;
        node* down;
        node* left;
        node* right;
    };
    node* start;
    node* end;
    node* head;

    size_t createStart(size_t pos);
    size_t createEnd(size_t pos);

    void goRight(node* &p, std::vector<node*> &stack);
    void goLeft(node* &p, std::vector<node*> &stack);
    void goUp(node* &p, std::vector<node*> &stack);
    void goDown(node* &p, std::vector<node*> &stack);
    void create(size_t sizeX, size_t sizeY);

    bool hasNoNeighbours(const node* p) const;
    bool isOldDirection(const node* p, const std::vector<node*> oldpassages) const;

    node* startPassage();

public:
    Maze();
    ~Maze();
    Maze(const size_t sizeX, const size_t sizeY);
    Maze(const Maze &src);

    //appends elements from rhs
    Maze& operator=(const Maze &rhs);

    //creating
    void set(std::string input);
    void dfs();
    void bfs();

    //accessing
    size_t getSolution();
    size_t play();
    
    //removing
    void destroy();

    //output
    void print() const;
};

#endif