#ifndef FSA_H
#define FSA_H

#include "BStarTree.h"

class FSA
{
  public:
    FSA( BStarTree* t )
    {
      tree = t;
      k = 1;
      c = 1;
      prob = 0.99;
      cost = 0;
          recur = 0;   

    }
    ~FSA(){}
    void updateLoop();
    void PseudoGreedyLocalSearch();
    void HillClimbingSearch();
    void HighTempratureRandomSearch();
  private:  
    BStarTree*        tree;
    unsigned          temp; // temperature
    unsigned          recur; // recursive times
    unsigned          c; // User-specified parameters
    unsigned          k; // User-specified parameters
    double            prob; // uphill climb probability
    double            cost;

};

#endif