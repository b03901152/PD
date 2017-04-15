#include "BStarTree.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>   
#include <cmath>
#include "../gnuplot-iostream/gnuplot-iostream.h"


void YContour::pop()
{
  cout << "pop YContour: ( " << x << " ," << y  << " )" << endl;
  post->pre = pre;
  pre->post = post;
}

void YContour::insertBack( YContour* yc )
{
  cout << "insert" 
  << "( " << yc->x << ", " << yc->y << " )" 
  << " after"
  << "( " << x << ", " << y << " )" 
  << endl;
  yc-> pre = this;
  yc->post = post;
  post = post->pre = yc;
}

void BStarTree::randonConstructTree() {
  // cout << "nBlock" << endl;
  // for( unsigned i=0; i<nBlocks; ++i ) {
  //   cout << Blocks[i]->name << endl;
  //   for( unsigned j=0; j<Blocks[i]->nets.size(); ++j )
  //     cout << Blocks[i]->nets[j]->netID << " ";
  //   cout << endl;
  // }
  // random_shuffle( Blocks.begin(), Blocks.end() );
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
  for( unsigned i=0; i<nBlocks; ++i )
    cout << Nodes[i]->block->name << " ";
  cout << endl;
  recursiveBuildTree( root );
}

void BStarTree::recursiveBuildTree( Node* n ) {  
  if ( n == NULL ) return;
  cout << "recursiveBuildTree" << endl;
  cout << "n->block->name:" << n->block->name << endl;
  // cin.get();
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
    cout << "left" << endl;
    n->block->posx = n->parent->block->posx + n->parent->block->W;
    unsigned y = 0;
    yc = n->parent->yContour->post;
    while ( yc->x < n->block->posx + n->block->W && yc->post != yContourBegin ) {
      yc = yc->post;
      y = max( y, yc->y );
    }
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
      YContour* newYC = new YContour;
      newYC->x = n->block->posx + n->block->W ;
      newYC->y = yc->pre->y;
      n->yContour->insertBack( newYC );
    }
  }
  else if ( n == n->parent->child[1] ) {
    cout << "right" << endl;
    n->block->posx = n->parent->block->posx;
    unsigned y = 0;
    YContour* yc = n->parent->yContour;
    while ( yc->x < n->block->posx + n->block->W && yc != yContourBegin ) {
      y = max( y, yc->y );
      yc = yc->post;
    }
    n->parent->yContour->y = y + n->block->H;
    n->block->posy = y;

    yc  = n->parent->yContour->post; 
    while ( yc->x < n->block->posx + n->block->W && yc != yContourBegin ) {
      YContour* YCpop = yc;
      yc = yc->post;
      YCpop->pop();
    }
    YContour* newyc = new YContour;
    newyc->x = n->block->posx + n->block->W;
    newyc->y = n->block->posy;
    n->parent->yContour->insertBack( newyc );
  }
  YContour* y = yContourBegin; 
  while ( 1 ) {
    cout << "( " << y->x << ", " << y->y << " ) ,";
    y = y->post;
    if ( y == yContourBegin ) break;
  } 
  cout << endl;
  recursiveBuildTree( n->child[0] );
  recursiveBuildTree( n->child[1] );

}
// bool BStarTree::addToLeft() {

// }
// bool BStarTree::addToRight() {

// }


void BStarTree::randomRotate() {
  Node* n = Nodes[ rand() % nBlocks ];
  unsigned i = n->block->W;
  n->block->W = n->block->H;
  n->block->H = i;
}

void BStarTree::randomMove() {
  Node* parent = Nodes[ rand() % nBlocks ];
  Node* child = Nodes[ rand() % nBlocks ];
  popFromTree( child );
  bool ran = rand() % 2;
  Node* hold = parent->child[ ran ];
  parent->child[ ran ] = child;
  ran = rand() % 2;
  child->child[ ran ] = hold;
}

void BStarTree::randomAddAsChild( Node* parent, Node* child ) {
}

void BStarTree::popFromTree( Node* p ) {

}

void BStarTree::randomSwap() {
  Node* a = Nodes[ rand() % nBlocks ];
  Node* b = Nodes[ rand() % nBlocks ];
  Block* p = a->block;
  a->block = b->block;
  b->block = p;
}

void BStarTree::printBST() {
  cout << "printBST" << endl;
  // Gnuplot gp;
  vector<pair<double, double> > blockBorders;
  for (unsigned i = 0; i < nBlocks; ++i ) {
    for (double x = Blocks[i]->posx; x < Blocks[i]->posx + Blocks[i]->W; x+=0.05 ) {
      blockBorders.push_back( make_pair( x, (double)Blocks[i]->posy ) );
      blockBorders.push_back( make_pair( x, (double)Blocks[i]->H + (double)Blocks[i]->posy ) );
    }
    for (double y = Blocks[i]->posy; y < Blocks[i]->posy + Blocks[i]->H; y+=0.05 ) {
      blockBorders.push_back( make_pair( (double)Blocks[i]->posx , y ) );
      blockBorders.push_back( make_pair( (double)Blocks[i]->posx + (double)Blocks[i]->W , y ) );
    }
  }

  // gp << "set xrange [0:120]\nset yrange [0:120]\n";
  // // Data will be sent via a temporary file.  These are erased when you call
  // // gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
  // // (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
  // // and won't be deleted (this is useful when creating a script).
  // gp << "plot" << gp.file1d( xy_pts_B ) << "with lines title 'blocks'" << endl;
  // cout << "printBST END";


    Gnuplot gp;


  vector<pair<double, double> > xy_pts_B;
  for(double alpha=0; alpha<1; alpha+=1.0/24.0) {
    double theta = alpha*2.0*3.14159;
    xy_pts_B.push_back(make_pair(cos(theta), sin(theta)));
  }
  for (unsigned i = 0; i < blockBorders.size(); ++i )
    cout << blockBorders[i].first << " " <<  blockBorders[i].second << endl;
  gp << "set xrange [0:120]\nset yrange [0:120]\n";
  // Data will be sent via a temporary file.  These are erased when you call
  // gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
  // (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
  // and won't be deleted (this is useful when creating a script).
  gp << "plot" << gp.file1d(blockBorders) << "with points title 'circle'" << endl;

}
