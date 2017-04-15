#include "BStarTree.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>   
#include "gnuplot-iostream/gnuplot-iostream.h"


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

void BStarTree::randonConstructTree() {
	random_shuffle( Blocks.begin(), Blocks.end() );
	Nodes.resize( nBlocks );
	for ( unsigned i = 0; i < nBlocks; ++i )
		Nodes[i] = new Node;

	for ( unsigned i = 0; i < nBlocks; ++i ) {
		unsigned idx1 = 2 * i + 1;
		unsigned idx2 = 2 * i + 2;
		if ( idx1 >= nBlocks )
			Nodes[i]->child[0] = NULL;
		else
			Nodes[i]->child[0] = Nodes[ idx1 ];
		if ( idx2 >= nBlocks )
			Nodes[i]->child[1] = NULL;
		else
			Nodes[i]->child[1] = Nodes[ idx2 ];
		if ( i != 0 )
			Nodes[i]->parent = Nodes[ (i-1) / 2 ];		
	}
	Nodes[0]->parent = NULL;
	yContourBegin->x = yContourBegin->y = 0;
	recursiveBuildContour( root );
}

void BStarTree::recursiveBuildContour( Node* n ) {
	if ( n == NULL ) return;
	YContour* yc;
	if ( n->parent == NULL ) {
		yc = new YContour;
		n->block->posx = n->block->posy = 0;
		yContourBegin->y = 0;
		yc->x = 0;
		yc->y = n->block->H;
		yContourBegin->x = n->block->posx + n->block->W;
		yContourBegin->insertBack( yc );
	}
	else if ( n == n->parent->child[0] ) {
		n->block->posx = n->parent->block->posx + n->parent->block->W;
		unsigned y = 0;
		yc = n->parent->yContour->post;
		while ( yc->x <= n->block->posx + n->block->W && yc != yContourBegin ) {
			yc = yc->post;
			y = max( y, yc->y+1 );
		}
		yc = yc->pre;
		YContour* newYC = new YContour;
		n->block->posy = y;
		newYC->x = n->parent->block->posx + n->parent->block->W ;
		newYC->y = yc->y;
		yc = n->parent->yContour->post;
		while ( yc->x <= n->block->posx + n->block->W && yc != yContourBegin ) {
			YContour* popYC = yc;
			yc = yc->post;
			popYC->pop();
		}
		n->parent->yContour->y = n->block->posy + n->block->H;
		n->parent->yContour->insertBack( newYC );
	}
	else if ( n == n->parent->child[1] ) {
		n->block->posx = n->parent->block->posx;
		unsigned y = 0;
		YContour* yc = n->parent->yContour;
		while ( yc->x <= n->block->posx + n->block->W && yc != yContourBegin ) {
			y = max( y, yc->y+1 );
			yc = yc->post;
		}
		n->parent->yContour->y = y;
		n->block->posy = y;
		YContour* newyc = new YContour;
		newyc->x = n->block->posx + n->block->W;
		newyc->y = n->block->posy + n->block->H;
		yc  = n->parent->yContour->post; 
		while ( yc->x <= n->block->posx + n->block->W ) {
			YContour* YCpop = yc;
			yc = yc->post;
			YCpop->pop();
		}
		n->parent->yContour->insertBack( newyc );
	}
	recursiveBuildContour( n->child[0] );
	recursiveBuildContour( n->child[0] );
}

bool BStarTree::rotate() {
	return false;
}

bool BStarTree::move() {
	return false;
}

bool BStarTree::swap() {
	return false;
}

void BStarTree::printBST() {
	cout << "printBST" << endl;
	Gnuplot gp;

	vector<std::pair<double, double> > xy_pts_A;
	for(double x=-2; x<2; x+=0.01) {
		double y = x*x*x;
		xy_pts_A.push_back(std::make_pair(x, y));
	}

	vector<std::pair<double, double> > xy_pts_B;
	for(double alpha=0; alpha<1; alpha+=1.0/24.0) {
		double theta = alpha*2.0*3.14159;
		xy_pts_B.push_back(std::make_pair(cos(theta), sin(theta)));
	}

	gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	gp << "plot" << gp.file1d(xy_pts_A) << "with lines title 'cubic',"
		<< gp.file1d(xy_pts_B) << "with points title 'circle'" << std::endl;
}
