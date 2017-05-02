#include "BStarTree.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>   
#include <cmath>
#include <math.h> 
#include "../gnuplot-iostream/gnuplot-iostream.h"
#include "FSA.h"
#include <cstdlib>
#include <math.h>
#include <cassert>
#include <cfloat>
#include <fstream>
#include <algorithm>
Node& Node::operator = ( const Node& n ) {
  block = n.block;
  child[0] = n.child[0];
  child[1] = n.child[1];
  parent = n.parent;
  yContour = n.yContour; 
  return *this;
}

void YContour::pop()
{
  post->pre = pre;
  pre->post = post;
}

void YContour::insertBack( YContour* yc )
{
  yc-> pre = this;
  yc->post = post;
  post = post->pre = yc;
}

BStarTree::BStarTree():FloorPlanning() {
  action = new perturbAction( this );
}

void BStarTree::randonConstructTree() {
  cout << "randonConstructTree" << endl;
  random_shuffle( Blocks.begin(), Blocks.end() );
  Nodes.resize( nBlocks );
  for ( unsigned i = 0; i < nBlocks; ++i ) {
    Nodes[i] = new Node;
    Nodes[i]->block = Blocks[i];
    #ifdef _DEBUG
      cout << Nodes[i]->block->name << endl;
      cout << Blocks[i]->name << endl;
    #endif
  }
  for ( unsigned i = 0; i < nBlocks; ++i ) {
    unsigned idx1 = 2 * i + 1;
    unsigned idx2 = 2 * i + 2;
    if ( idx1 < nBlocks )
      Nodes[i]->child[0] = Nodes[ idx1 ];
    if ( idx2 < nBlocks )
      Nodes[i]->child[1] = Nodes[ idx2 ];
    if ( i != 0 )
      Nodes[i]->parent = Nodes[ (i-1) / 2 ];    
  }
  root = Nodes[0];
  unsigned queueSize = 500;
  if ( isFeasibleQueue.empty() )
    for ( unsigned i = 0; i < queueSize; ++i )
      isFeasibleQueue.push( false );
  nTrueInQueue = 0;
}

void BStarTree::recursiveBuildTree( Node* n ) {  
  if ( n == NULL ) return;
  n->block->placed = true;
  YContour* yc;
  if ( n->parent == NULL ) {
    yc = new YContour;
    yContourBegin = new YContour;
    n->block->posx = n->block->posy = 0;
    yContourBegin->y = n->block->H;
    yContourBegin->x = 0;
    yc->x = n->block->W;
    yc->y = 0;
    yContourBegin->insertBack( yc );
    n->yContour = yContourBegin;
  }
  else if ( n == n->parent->child[0] ) {
    n->block->posx = n->parent->block->posx + n->parent->block->W;
    unsigned y = 0;
    yc = n->parent->yContour->post;
    while ( yc->x < n->block->posx + n->block->W && yc != yContourBegin ) {
      y = max( y, yc->y );
      yc = yc->post;
    }
    YContour* newYC = new YContour;
    newYC->y = yc->pre->y;
    n->block->posy = y;
    yc = n->parent->yContour->post;
    yc->y = y + n->block->H;
    n->yContour = yc;
    yc = yc->post;

    while ( yc->x < n->block->posx + n->block->W && yc != yContourBegin ) {
      YContour* popYC = yc;
      yc = yc->post;
      popYC->pop();
    }

    if ( yc->x != n->block->posx + n->block->W ) {
      newYC->x = n->block->posx + n->block->W ;
      n->yContour->insertBack( newYC );
    }
  }
  else if ( n == n->parent->child[1] ) {
    n->block->posx = n->parent->block->posx;
    YContour* yc = n->parent->yContour;
    unsigned y = yc->y;
    while ( yc->x < n->block->posx + n->block->W ) {
        y = max( y, yc->y );
        yc = yc->post;
        if ( yc == yContourBegin ) break;
    }

    YContour* newyc = new YContour;
    newyc->x = n->block->posx + n->block->W;
    newyc->y = yc->pre->y;

    n->parent->yContour->y = y + n->block->H;
    n->block->posy = y;
    
    yc  = n->parent->yContour->post; 
    while ( yc->x < n->block->posx + n->block->W && yc != yContourBegin ) {
      YContour* YCpop = yc;
      yc = yc->post;
      YCpop->pop();
    }
    n->yContour = n->parent->yContour;
    n->parent->yContour->insertBack( newyc );
    if ( newyc->x == newyc->post->x )
      newyc->pop();
  }
  recursiveBuildTree( n->child[0] );
  recursiveBuildTree( n->child[1] );

}
void BStarTree::rotate( Node* n ) {
  if ( n->block->W > H || n->block->H > W ) return;
  unsigned i = n->block->W;
  n->block->W = n->block->H;
  n->block->H = i;
  action->rotateNode = n;
}

void BStarTree::randomRotate() {
  rotate( Nodes[ rand() % nBlocks ] );
}

void BStarTree::randomMove() {
  Node* a = Nodes[ rand() % nBlocks ];
  Node* b = Nodes[ rand() % nBlocks ];
  while ( a == b ) {
    b = Nodes[ rand() % nBlocks ];
  }
  move( a,b );
}

void BStarTree::move( Node* destination , Node* target ) {
  if ( action->Nodes.empty() )
    action->Nodes.resize( Nodes.size() );
  for ( unsigned i=0; i<Nodes.size(); ++i ) {
    action->Nodes[i] = *(Nodes[i]);
  }
  action->NodePointers = Nodes;
  Block* moveBlock = target->block;
  while ( target->child[0] != NULL || target->child[1] != NULL ) {
    bool r = rand()%2;
    if ( target->child[r] == NULL )
      r = !r;
    target->block = target->child[r]->block;
    target = target->child[r];
  }
  if ( target == destination )
    destination = destination->parent;
  if ( target->parent->child[0] == target )
    target->parent->child[0] = NULL;
  else
    target->parent->child[1] = NULL;
  bool r = rand()%2;
  target->block = moveBlock;
  target->parent = destination;
  target->child[r] = destination->child[r];
  target->child[!r] = NULL;
  if ( destination->child[r] != NULL )
    destination->child[r]->parent = target;
  destination->child[r] = target;
}

void BStarTree::swap( Node* a, Node* b ) {
  Block* hold = a->block;
  a->block =  b->block;
  b->block = hold;
  action->swapNode1 = a;
  action->swapNode2 = b;
}

void BStarTree::randomSwap() {
  Node* a = Nodes[ rand() % nBlocks ];
  Node* b = Nodes[ rand() % nBlocks ];
  while ( a == b ) {
    b = Nodes[ rand() % nBlocks ];
  }
  swap( a, b );
}

void BStarTree::updateIsFeasibleQueue() {
  feasibleSol += isFeasible;
  nTrueInQueue += isFeasible;
  isFeasibleQueue.push( isFeasible );
  nTrueInQueue -= isFeasibleQueue.front();
  isFeasibleQueue.pop();
}

unsigned BStarTree::calcArea() {
  unsigned x;
  unsigned y;
  unsigned maxX = 0;
  unsigned maxY = 0;
  for ( unsigned i=0; i<nBlocks; ++i ) {
    x = Blocks[i]->posx + Blocks[i]->W;
    y = Blocks[i]->posy + Blocks[i]->H;
    maxX = max( maxX, x );
    maxY = max( maxY, y );
  }
  isFeasible = ( maxX < W && maxY < H );
  feasibleSol += isFeasible;
  runningAlpha = 0.5;
  // assert( nTrueInQueue <=500 );
  // unsigned queueSize = 500;
  // runningAlpha = alphaBase + ( 1 - alphaBase ) * ( 1-(double)nTrueInQueue/queueSize );
  runningAlpha = alphaBase;
  MIN_W = maxX;
  MIN_H = maxY;
  area = maxX*maxY;
  return area;
}
double BStarTree::calcHPWL() {
  HPWL = 0;
  for ( unsigned i=0; i<nNets; ++i ) {
    double minX = DBL_MAX;
    double minY = DBL_MAX;
    double maxX = 0;
    double maxY = 0;
    for ( unsigned j=0, n = Nets[i]->terminals.size(); j < n; ++j ) {
      Terminal* t = Nets[i]->terminals[j];
      // minX = min( minX, t->posx );
      // minY = min( minY, t->posy );
      if ( ! (t->isTerminal) ) {
        maxX = max( maxX, t->posx + (double)((Block*)t)->W/2 );
        maxY = max( maxY, t->posy + (double)((Block*)t)->H/2 );
        minX = min( minX, t->posx + (double)((Block*)t)->W/2 );
        minY = min( minY, t->posy + (double)((Block*)t)->H/2 );
      } 
      else {
        maxX = max( maxX, (double)t->posx );
        maxY = max( maxY, (double)t->posy );
        minX = min( minX, (double)t->posx );
        minY = min( minY, (double)t->posy );

      }
    }
    HPWL +=  (maxX-minX + maxY - minY);
  }
  return HPWL;
}

double BStarTree::runningCost() {
  calcArea();
  // calcHPWL(); 
  // double cost = runningAlpha*area/Anorm + beta*HPWL/Wnorm;
  double areaCost = runningAlpha*area/Anorm;
  // double wireCost = beta*HPWL/Wnorm;
  double wireCost = 0;
  double ratioCost = pow( (double)W/H-(double)MIN_W/MIN_H, 2 );
  double cost =  areaCost + wireCost + ratioCost;
  return cost ;
}

double BStarTree::realCost() {
  calcArea();
  calcHPWL(); 
  double cost = alpha*area + (1-alpha)*HPWL;
  return cost ;
}

void BStarTree::printBST() {
  cout << "printBST" << endl;
  Gnuplot gp;
  gp << "set xrange [-1:" << 3*W << "]\nset yrange [-2:" << 3*H << "]\n";
  gp << "set object " << 1 << " rect from "
  << "0 , 0 to " << W << "," << H 
  << " fc rgb \"yellow\" " << endl;

  for (unsigned i = 0; i < nBlocks; ++i ) {
    unsigned x = Blocks[i]->posx;
    unsigned y = Blocks[i]->posy;
    unsigned H = Blocks[i]->H;
    unsigned W = Blocks[i]->W;
    gp << "set object " << i+2 << " rect from "
    << x << "," << y 
    << "to " << x + W << "," << y + H 
    << " fc rgb \"white\" " << endl;

    // gp << "set label \"" << Blocks[i]->name << "\" at " << x + W/2 
    gp << "set label \"" << i << "\" at " << x + W/2 
    << ", " << y + H/2 << endl;
  }
  gp << "set size ratio -1" << endl;

  YContour* yc = yContourBegin;
  vector<pair<double, double> > blockBorders;
  while( 1 ) {
  //   cout << "( " << yc->x << "," << yc->y << " )" << endl;
    blockBorders.push_back( make_pair( yc->x, yc->y ) );
    yc = yc->post;
    if ( yc == yContourBegin ) break;
  }
  gp << "plot " << "'-' binary" << gp.binFmt1d(blockBorders, "record"); 
  gp << "with lines title 'yContour'";
  gp << "lt 1 \n";
  gp.sendBinary1d(blockBorders);
}

void perturbAction::undo() {
  switch ( type ) {
    case 0:
      tree->swap( swapNode1, swapNode2 );
      return;
    case 1:
      for ( unsigned i=0; i<Nodes.size(); ++i )
        *( tree->Nodes[i] ) = Nodes[i];
      tree->Nodes = NodePointers;
      return;
    case 2:
      tree->rotate( rotateNode );
      return;
  }
}