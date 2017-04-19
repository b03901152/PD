#include "FSA.h"
#include <cstdlib>
#include <math.h>
#include <cassert>
#include <cfloat>

void FSA::updateLoop()
{
  HighTempratureRandomSearch();
  for ( unsigned i = 1; i < k; ++i )
    PseudoGreedyLocalSearch();
  while ( 1 )
    HillClimbingSearch();
}

void FSA::HillClimbingSearch() {
  temp = T1*(newCost-initCost)/iter;
}

void FSA::PseudoGreedyLocalSearch() {
  temp = T1*(newCost-initCost)/(c*iter);
}

void FSA::HighTempratureRandomSearch() {
  double cost = 0;
  unsigned times = (double)(tree->nBlocks)/20 + 1;
  double Anorm;
  double Wnorm;
  for ( unsigned i = 1; i < times; ++i ) {
    randomPerturb();
    cost += tree->cost();
    Anorm += tree->area();
    Wnorm += tree->HPWL();
  }
  newCost = initCost = tree->cost();
  cost /= times;
  Anorm /= times;
  Wnorm /= times;
  T1 = cost/log(prob);
  temp = T1;
}

void FSA::perturb() {
  unsigned ran = rand()%3;
  if ( ran == 0 )
    tree->randomSwap();
  else if ( ran == 1 )
    tree->randomMove();
  else
    tree->randomRotate();
}

void FSA::init() {
  tree->alpha = 0.5;
  tree->alphaBase = 0.35;
  tree->beta = 0.35;
  temp = 10000000;
  c = 100;
  prob = 0.99;
  cost = 0;
  iter = 0;   
  totalCost = 0;
  bestCost = DBL_MAX;
  newCost = 0;
}

void FSA::randomPerturb() {
  unsigned r =rand();
  r %= (tree->nBlocks/20);
  r += (tree->nBlocks/20);
  r++;
  for( unsigned i=0; i<r; ++i )
    perturb();
}

void FSA::SA() {
  tree->recursiveBuildTree( tree->root );
	while( 1 ) {
    if ( iter % 10000 == 0 ) {
      tree->printBST();
      cout << "iteration times: " << iter << endl;
      cout << "best cost: " << bestCost << endl;
      cout << "cost: " << newCost << endl;
      cout << "prob: " << prob << endl << endl;
      cin.get();
    }
    if ( iter == 1 )
      HighTempratureRandomSearch();
    else if ( iter < k )
      PseudoGreedyLocalSearch();
    else
      HillClimbingSearch();
    originCost = newCost;
    randomPerturb();
    tree->recursiveBuildTree( tree->root );
    newCost = tree->cost();
    prob = min( (double)1, exp( (originCost-newCost)/temp ) );
    if ( prob < 0.000005 )
      return;
    #ifdef _DEBUG
      cout << "deltaCost: " << newCost - originCost << endl;
      cout << "iteration times: " << iter << endl;
      cout << "best cost: " << bestCost << endl;
      cout << "cost: " << newCost << endl;
      cout << "prob: " << prob << endl << endl;
    #endif
		if ( newCost > originCost && (double)rand()/RAND_MAX > prob ) {
        tree->action->undo();
        tree->recursiveBuildTree( tree->root );
        #ifdef _DEBUG
          cout << "undo" << endl;
          double afterUndoCost = tree->cost();
          assert( afterUndoCost == originCost );
        #endif
        continue;      
    }
    if ( newCost < bestCost ) {
      bestCost = newCost;
      bestTreeNodes = tree->Nodes;
    }
    ++iter;
    tree->runningAlpha += tree->allNodesInTheOutline;
	}
}