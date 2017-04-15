#ifndef BSTARTTREE_H
#define BSTARTTREE_H

#include "FloorPlanning.h"
#include <utility> 

class YContour
{
  public:
    YContour() { post = pre = this; }
    ~YContour() { delete this; }
    void pop();
    void insertBack( YContour* );
    YContour* pre;
    YContour* post;
    unsigned x; // left point of the range
    unsigned y;

};

class Node
{
	public:
    Node() {
      child[0] = child[1] = NULL;
    }
    ~Node() {}
    Block*          block;
    Node*           child[2]; // child[0] is left child, child[1] is right child
    Node*           parent;
    YContour*       yContour;
};

class BStarTree : public FloorPlanning
{
  public:
    BStarTree()
    :FloorPlanning() {}
    ~BStarTree() {}
    void      randonConstructTree();
    void      recursiveBuildContour( Node* );
    bool      rotate();
    bool      move();
    bool      swap();
    double    cost();
    void      printBST();

    
    Node*           root;
    vector<Node*>   Nodes;
    YContour*       yContourBegin;
};

#endif