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
      child[0] = child[1] = parent= NULL;
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
    void      recursiveBuildTree( Node* );
    void      randomRotate();
    void      randomMove();
    void      randomSwap();
    double    cost() { return 0; };
    void      printBST();
    // bool      addToLeft( Node*, Node* );
    // bool      addToRight( Node*, Node* );
    void      popFromTree( Node* );
    void      randomAddAsChild( Node*, Node* );

    Node*           root;
    vector<Node*>   Nodes;
    YContour*       yContourBegin;
};

#endif