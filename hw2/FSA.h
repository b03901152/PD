#ifndef FSA_H
#define FSA_H

#include "BStarTree.h"
#include "FloorPlanning.h"

class FSA
{
  public:
    FSA( BStarTree* t )
    {
      tree = t;
      init();
    }
    ~FSA(){}
    void updateLoop();
    void PseudoGreedyLocalSearch();
    void HillClimbingSearch();
    void HighTempratureRandomSearch();
    void SA();
    void perturb();
    void randomPerturb();
    void init();
  private:  
    BStarTree*        tree;
    vector<Node*>     bestTreeNodes;
    double            bestCost;
    double            cost;
    double            temp; // temperature
    double            feasibleCounter;
    unsigned          iter; // iteration times
    double            c; // User-specified parameters, for update temperature
    double            k; // User-specified parameters, for update temperature
    double            prob; // uphill climb probability
    double            totalCost;
    double            avgDeltaCost; // avg temp change
    double            T1;
    double            newCost;
    double            originCost;
    double            initCost;
    double            totalDeltaCost;
};

#endif