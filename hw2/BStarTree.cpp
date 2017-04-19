#include "BStarTree.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>   
#include <cmath>
#include <math.h> 
#include "../gnuplot-iostream/gnuplot-iostream.h"


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
  random_shuffle( Blocks.begin(), Blocks.end() );
  Nodes.resize( nBlocks );
  for ( unsigned i = 0; i < nBlocks; ++i ) {
    Nodes[i] = new Node;
    Nodes[i]->block = Blocks[i];
    cout << Nodes[i]->block->name << endl;
    cout << Blocks[i]->name << endl;
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

  cout << "nNodes" << endl;
  #ifdef _DEBUG
  for( unsigned i=0; i<nBlocks; ++i )
    cout << Nodes[i]->block->name << " ";
  cout << endl;
  #endif
}


int counter = 0;
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
  }
  recursiveBuildTree( n->child[0] );
  recursiveBuildTree( n->child[1] );

}
void BStarTree::rotate( Node* n ) {
  unsigned i = n->block->W;
  n->block->W = n->block->H;
  n->block->H = i;
  action->type = 3;
  action->rotateNode = n;
}

void BStarTree::randomRotate() {
  rotate( Nodes[ rand() % nBlocks ] );
}

void BStarTree::randomMove() {
  Node* a = Nodes[ rand() % nBlocks ];
  Node* b = Nodes[ rand() % nBlocks ];
  while ( a == b ) {
    b =  Nodes[ rand() % nBlocks ];
  }
}

void BStarTree::move( Node* destination , Node* target ) {
  action->Nodes = Nodes;
  Block* moveBlock = target->block;
  while ( target->child[0] != NULL || target->child[1] != NULL ) {
    bool r = rand()%2;
    if ( target->child[r] == NULL )
      r = !r;
    target->block = target->child[r]->block;
    target = target->child[r];
  }
  bool r = rand()%2;
  target->block = moveBlock;
  target->parent = destination;
  target->child[r] = destination->child[r];
  target->child[!r] = NULL;
}

void BStarTree::swap( Node* a, Node* b ) {
  Block* hold = a->block;
  a->block =  b->block;
  b->block = hold;
}

void BStarTree::randomSwap() {
  Node* a = Nodes[ rand() % nBlocks ];
  Node* b = Nodes[ rand() % nBlocks ];
  while ( a == b ) {
    b = Nodes[ rand() % nBlocks ];
  }
  action->type = 1;
  action->swapNode1 = a;
  action->swapNode2 = b;
  swap( a, b );
}

unsigned BStarTree::area() {
  unsigned x;
  unsigned y;
  unsigned maxX = 0;
  unsigned maxY = 0;
  allNodesInTheOutline = true;
  for ( unsigned i=0; i<nBlocks; ++i ) {
    x = Blocks[i]->posx + Blocks[i]->W;
    y = Blocks[i]->posy + Blocks[i]->H;
    maxX = max( maxX, x );
    maxY = max( maxY, y );
    if ( x > W || y > H )
      allNodesInTheOutline = false;
  }
  return maxX*maxY;
}

unsigned BStarTree::HPWL() {
  unsigned HPWL = 0;
  for ( unsigned i=0; i<nNets; ++i ) {
    unsigned minX = Nets[i]->terminals[0]->posx;
    unsigned minY = Nets[i]->terminals[0]->posy;
    unsigned maxX = Nets[i]->terminals[0]->posx;
    unsigned maxY = Nets[i]->terminals[0]->posy;
    for ( unsigned j=0; j<Nets[i]->terminals.size(); ++i ) {
      Terminal* t = Nets[i]->terminals[j];
      minX = min( minX, t->posx );
      minY = min( minY, t->posy );
      if ( ! (t->isTerminal) ) {
        maxX = max( maxX, t->posx + ((Block*)t)->W );
        maxY = max( maxY, t->posy + ((Block*)t)->H );
      } else {
        maxX = max( maxX, t->posx );
        maxY = max( maxY, t->posy );
      }
    }
    HPWL +=  maxX-minX + maxY - minY;
  }
  return HPWL;
}

double BStarTree::runningCost() {
  unsigned area = area();
  unsigned HPWL = HPWL();
  double cost = runningAlpha*area + beta*wirelength;
  cost += ( 1- runningAlpha - beta ) * 
  pow( (doubel)W/H-(doubel)maxX/maxY, 2 );
  return cost ;
}

double BStarTree::realCost() {
  unsigned area = area();
  unsigned HPWL = HPWL();
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
    gp << "set object " << i+ << " rect from "
    << x << "," << y 
    << "to " << x + W << "," << y + H 
    << " fc rgb \"white\" " << endl;

    gp << "set label \"" << i+1 << "\" at " << x + W/2 
    << ", " << y + H/2 << endl;
  }
  gp << "set size ratio -1" << endl;

  YContour* yc = yContourBegin;
  vector<pair<double, double> > blockBorders;
  while( 1 ) {
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
  cout << "undo type:" << type << endl;
  switch ( type ) {
    cout << "undo type:" << type << endl;
    case 1:
      tree->swap( swapNode1, swapNode2 );
      return;
    case 2:
      tree->Nodes = Nodes;
      return;
    case 3:
      tree->rotate( rotateNode );
  }
}