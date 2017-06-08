#include "FSA.h"
#include <cstdlib>
#include <math.h>
#include <cassert>
#include <cfloat>
#include <fstream>
#include <algorithm>

void FSA::HillClimbingSearch() {
  cout << "HillClimbingSearch" << endl;
  temp = T1*(tree->nBlocks)/( 10*iter );
}

void FSA::PseudoGreedyLocalSearch() {
  cout << "PseudoGreedyLocalSearch" << endl;
  temp = T1*(tree->nBlocks)/( c*iter );
}

void FSA::HighTempratureRandomSearch() {
  cout << "HighTempratureRandomSearch" << endl;
  unsigned times = tree->nBlocks;
  double costNorm = 0.01;
  double Anorm = 0;
  double Wnorm = 0;
  for ( unsigned i = 0; i < times; ++i ) {
    perturb();
    tree->calcArea();
    tree->calcHPWL();
    Anorm += tree->area;
    Wnorm += tree->HPWL;
  }
  Anorm /= times;
  Wnorm /= times;
  tree->Anorm = Anorm;
  tree->Wnorm = Wnorm;
  initCost = tree->runningCost();
  unsigned uphillCounter = 0;
  double a;
  double b;
  a = tree->runningCost();
  for ( unsigned i = 0; i < 5*times; ++i ) {
    b = tree->runningCost();
    if ( b > a ) {
      ++uphillCounter;
      costNorm+= (b - a) ;
    }
    perturb();
    tree->recursiveBuildTree( tree->root );
    a = b;
  }
  costNorm /= uphillCounter;
  costBeforePerturb = originCost = tree->runningCost();
  T1 = costNorm*50;
  temp = T1;
}

void FSA::perturb() {
  unsigned ran = rand()%3;
  if ( ran == 0 )
    tree->randomSwap();
  else if ( ran == 1 )
    tree->randomMove();
  else if ( ran == 2 )
    tree->randomRotate();
  tree->action->type = ran;
}

void FSA::init() {
  tree->alphaBase = 0.35;
  tree->beta = 0.35;
  tree->feasibleSol = 0;
  temp = 0;
  c = 100;
  k = max( (tree->nBlocks)/2 , (unsigned)7 );
  prob = 0.99;
  iter = 0;   
  totalCost = 0;
  bestCost = DBL_MAX;
  newCost = 0;
}

void FSA::randomPerturb( bool useRealCost ) {
  unsigned r = (tree->nBlocks)*5;
  costAfterPerturb = useRealCost? (double)tree->realCost() : tree->runningCost();
  double originCostBeforePerturb;
  double totalDeltaCostOfTemp = 0;
  for( unsigned i=0; i<r; ++i ) {
    originCostBeforePerturb = costBeforePerturb;
    costBeforePerturb = costAfterPerturb;
    perturb();
    tree->recursiveBuildTree( tree->root );
    costAfterPerturb = useRealCost? (double)tree->realCost() : tree->runningCost();
    double deltaCost = costAfterPerturb - costBeforePerturb;
    totalDeltaCostOfTemp += abs(deltaCost);
    if ( totalDeltaCostOfTemp )
      prob = min( (double)1, exp( -deltaCost/(temp*totalDeltaCostOfTemp/(i+1)) ) );
    else
      prob = 1;
    if ( deltaCost > 0 && (double)rand()/RAND_MAX > prob ) {
      tree->action->undo();
      tree->recursiveBuildTree( tree->root );
      costAfterPerturb = costBeforePerturb;
      costBeforePerturb = originCostBeforePerturb;
      continue;
    }
    if ( costAfterPerturb < bestCost && tree->isFeasible ) {
      bestCost = (double)tree->realCost();
      bestIter = iter;
      bestTreeNodesPointer = tree->Nodes;
      if ( bestTreeNodes.empty() ) {
        bestTreeNodes.resize( tree->Nodes.size() );
        bestBlockW.resize( tree->nBlocks );
      }
      for( unsigned i=0; i<tree->nBlocks; ++i ) 
        bestBlockW[i] = tree->Blocks[i]->W;
      for( unsigned i=0; i<tree->Nodes.size(); ++i )
        bestTreeNodes[i] = *(tree->Nodes[i]);
    }
  }
}

void FSA::SA() {
  tree->recursiveBuildTree( tree->root );
  tree->calcArea();
  tree->calcHPWL();
  costBeforePerturb = originCost = tree->runningCost();
  bool useRealCost = false;
  unsigned realCostTunningCounter = 0;
  double bc = 0;//best cost
  unsigned notImproveCounter = 0;
	while( 1 ) {
    if ( iter == 0 )
      HighTempratureRandomSearch();
    else if ( !bestTreeNodes.empty() ) {
      if ( !useRealCost ) {
        useRealCost = true;
        T1 *= 10;
      }
      if ( bestCost != bc ) {
        notImproveCounter = 0;
        bc = bestCost;
      }
      // tree->printBST();
      // cin.get();
      // return;
      if ( notImproveCounter++ == 20*k ) {
        return;
      }
    }
    else if ( iter < 20*k )
      PseudoGreedyLocalSearch();
    else if ( iter < 40*k )
      HillClimbingSearch();
    else {
      init();
      HighTempratureRandomSearch();
      tree->randonConstructTree();
      tree->recursiveBuildTree( tree->root );
      continue;
    }
    cout << "total Iteration times: " << totalIter++ << endl;
    cout << "iter " << iter << endl;
    iter++;

    // cout << "prob " << prob << endl;
    tree->recursiveBuildTree( tree->root );
    // if(iter%10 == 0) {
    if(0){
        tree->printBST();
        cin.get();
    }
    randomPerturb( useRealCost );
    // newCost = tree->runningCost();
    // originCost = newCost;
	}
}

void FSA::output( string& outputPath ) {
  cout << "outputPath: " << outputPath << endl;
  tree->Nodes = bestTreeNodesPointer;
  assert(!bestTreeNodes.empty());
  for( unsigned i=0; i<tree->Nodes.size(); ++i )
    *(tree->Nodes[i]) = bestTreeNodes[i];
  for( unsigned i=0; i<tree->nBlocks; ++i ) {
    if ( bestBlockW[i] != tree->Blocks[i]->W ) {
      swap( tree->Blocks[i]->W, tree->Blocks[i]->H );
    }
  }
  // cout << "output2" << endl;
  tree->recursiveBuildTree( tree->root );
  cout << "Cost: " << tree->realCost() << endl;
  assert(  (double)tree->realCost() == bestCost );
  ofstream result;
  result.open( outputPath, ofstream::out );
  result << tree->realCost() << endl;
  result << tree->HPWL << endl;
  result << tree->area << endl;
  unsigned W = 0;
  unsigned H = 0;
  for ( unsigned i=0; i<tree->nBlocks; ++i ) {
    Block* b = tree->Blocks[i];
    W = max( W, b->posx + b->W );
    H = max( H, b->posy + b->H );
  }
  result << W << " " << H << endl;
  result << (double)(clock()-startTime)/CLOCKS_PER_SEC << endl;
  cout << "Running time:" << (double)(clock()-startTime)/CLOCKS_PER_SEC << endl;
  cout << "bestIter " << bestIter << endl;
  tree->printBST();
  for ( unsigned i=0; i<tree->nBlocks; ++i ) {
    Block* b = tree->Blocks[i];
    result << b->name << " " << b->posx << " " << b->posy << " "
    << b->posx + b->W << " " << b->posy + b->H << endl;
  }
  result.close();
}
