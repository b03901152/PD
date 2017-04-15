#include "FSA.h"

void FSA::updateLoop()
{
  HighTempratureRandomSearch();
  for ( unsigned i = 1; i < k; ++i )
    PseudoGreedyLocalSearch();
  while ( 1 )
    HillClimbingSearch();
}

void FSA::PseudoGreedyLocalSearch()
{

}

void FSA::HillClimbingSearch()
{

}


void FSA::HighTempratureRandomSearch()
{

}