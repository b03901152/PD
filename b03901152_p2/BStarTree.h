#ifndef BSTARTTREE_H
#define BSTARTTREE_H

#include "FloorPlanning.h"
#include <utility> 
#include <queue> 

// #define _DEBUG

class YContour
{
  public:
    YContour() { post = pre = this; }
    ~YContour() { delete this; }
    void pop();
    void insertBack( YContour* );
    YContour*       pre;
    YContour*       post;
    unsigned        x; // left point of the range
    unsigned        y;

};

class Node
{
  public:
    Node() {
      child[0] = child[1] = parent= NULL;
    }
    Node( const Node& n ) {
      block = n.block;
      child[0] = n.child[0];
      child[1] = n.child[1];
      parent = n.parent;
      yContour = n.yContour;
    }
    Node& operator = ( const Node& );
    ~Node() {}
    Block*          block;
    Node*           child[2]; // child[0] is left child, child[1] is right child
    Node*           parent;
    YContour*       yContour;
};

class perturbAction;

class BStarTree : public FloorPlanning
{
  public:
    BStarTree();
    ~BStarTree() {}
    void            randonConstructTree();
    void            recursiveBuildTree( Node* );
    void            randomRotate();
    void            randomMove();
    void            move( Node*, Node* );
    void            randomSwap();
    void            swap( Node*, Node* );
    void            rotate( Node* );
    void            printBST();
    double          cost();
    unsigned        calcArea();
    double          calcHPWL();
    void            randomAddAsChild( Node*, Node* );
    double          runningCost();
    double          realCost();
    void            updateIsFeasibleQueue();

    double          alphaBase;
    double          alpha;
    double          beta;
    Node*           root;
    // FSA*            fsa;
    vector<Node*>   Nodes;
    YContour*       yContourBegin;
    perturbAction*  action;
    double          runningAlpha;
    unsigned        area;
    double          HPWL;
    unsigned        MIN_W;
    unsigned        MIN_H;
    unsigned        Anorm;
    unsigned        Wnorm;
    unsigned        feasibleSol;
    bool            isFeasible;
    queue<bool>     isFeasibleQueue;
    unsigned        nTrueInQueue;
};


class perturbAction {
  public:
    perturbAction( BStarTree* bst ) {
        tree = bst;
    }

    void undo();

    // for type1
    Node*           swapNode1;
    Node*           swapNode2;
    
    // for type2, move bLeaf as aLeaf'new leaf
    vector<Node>    Nodes;
    vector<Node*>   NodePointers;
    
    // for type3
    Node*           rotateNode;
    unsigned        type;// 1 for swap ,2 for move, 3 for rotate
    BStarTree*      tree;

};

#endif