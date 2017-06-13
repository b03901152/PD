#ifndef GRAPH_H
#define GRAPH_H
#include <memory>
#include <vector>
#include <limits>
#include <cassert>
#include <string> 
#include "../GnuplotLivePlotter/GnuplotLivePlotter.h"
// #include "../FloorPlan/FloorPlan.h"

using namespace std;

class Edge;
class R_shape;
class Pin;

// enum Vertex_type {
// 	Steiner = 0,
// 	Pin = 1
// }
class Graph;
class Vertex;

#include <iostream>
using namespace std;
class Edge
{
public:
	Edge(Vertex* v1, Vertex* v2)
	:_v1(v1), _v2(v2),_ID(0),_selected(false) {}
	~Edge(){};
	Vertex* get_the_other_node( Vertex* v) { return _v1 == v ? _v2 : _v1 ; }
	Vertex* 	_v1;
	Vertex*		_v2;
	unsigned 	_ID;
	bool 		_selected;
};



class Vertex
{
//friend class Graph;
public:
	Vertex(unsigned x, unsigned y ):_ID(numeric_limits<unsigned>::max()),_x(x),_y(y) {}
	void addEdge( Edge* e ) { _edges.push_back(e); }
	~Vertex() {};
	unsigned x() const {return _x;}
	unsigned y() const {return _y;}
	void merge(Vertex* v) {
		assert(_x == v->_x );
		assert(_y == v->_y );
		for(auto& e:v->_edges) {
			if(e->_v1->x() == e->_v2->x() &&
				e->_v1->y() == e->_v2->y())
				continue;
			if(e->_v1 == v)
				e->_v1 = this;
			else
				e->_v2 = this;
			_edges.push_back(e);
		}
		v->_edges.clear();
	}
    static double slope( const Vertex& v1,  const Vertex& v2 ) {
    	if ( v1.x() == v2.x() ) {
    		int y = v1.y()-v2.y();
    		assert(y!=0);
    		if( y > 0)
				return numeric_limits<double>::max();
			else
				return -numeric_limits<double>::max();
    	}
    	return ((double)v1.y()-v2.y()) / ((double)v1.x()-v2.x());
    }
    static unsigned dist(Vertex* v1, Vertex* v2) {
    	int r1 = v1->x() - v2->x();
    	int r2 = v1->y() - v2->y();
    	return abs(r1) + abs(r2);
    }
    static bool same(Vertex* v1, Vertex* v2)
    {
    	return v1->x() == v2->x() && v1->y() == v2->y();
    }
	void setPos(unsigned x, unsigned y) { _x = x; _y = y; }
// private:
	unsigned					_ID;
	unsigned 					_x;
	unsigned 					_y;
	bool 						_used;
	vector<Edge*> 				_edges;
};


class Graph
{
public:
	Graph(){};
	~Graph(){};
	void addEdge( Edge* e ) { 
		_edges.push_back(e); 
		e->_v1->addEdge(e);
		e->_v2->addEdge(e);
	}
	void addVertex( Vertex* n ) { _vertices.push_back(n); }

	unsigned getVerticesNum() { return _vertices.size(); }
	unsigned getEdgesNum() { return _edges.size(); }
	void plot(GnuplotLivePlotter& gp) {
		// need to setTitle() and setRange() 
		for(Vertex* v: _vertices) {
			if(v->_ID == numeric_limits<unsigned>::max())
				continue;
			gp.addLabel(v->x()+1,v->y()+1,to_string(v->_ID));
			gp.addPoint((double)v->x(),(double)v->y());
		}
		for(Edge* e: _edges) {
			// if(!(e->_selected)) continue;
			vector< vector<double> > l = 
			{ 
				{ (double)e->_v1->x(), (double)e->_v1->y() } ,
				{ (double)e->_v2->x(), (double)e->_v2->y() }
			};
			double x = e->_v1->x() + e->_v2->x();
			x /= 2;
			double y = e->_v1->y() + e->_v2->y();
			y /= 2;
			if(e->_ID)
				gp.addLabel(x,y,to_string(e->_ID));
			gp.addLine(l);
		}
		gp.show();
	}
	vector<Vertex*>& getVertices(){ return _vertices; }
// private:
	vector<Vertex*>		_vertices;
	vector<Edge*> 		_edges;
};

#endif