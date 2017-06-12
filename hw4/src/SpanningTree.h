#ifndef SPANNINGTREE_H
#define SPANNINGTREE_H

#include "Graph/Graph.h"
#include "FloorPlan.h"

#include <map>



class SpanningTree
{
public:
	SpanningTree(FloorPlan& FP):
	_FP(FP), _graph(FP._graph), _pins(FP._pins) {};
	void init();
	void initMap();
	void initGraph();
	void construnction();
	void connect_neighbors_R1();
	void connect_neighbors_R8();
	void connect_neighbors_R2R7();
	void plot();
	~SpanningTree(){};
private:
	FloorPlan& _FP;
	Graph& _graph;
	vector<Vertex*> _pins;
	vector<unsigned> _sort_vec; // sort by x+y
	vector<unsigned> _sort_reverse_vec; // sort by x-y
};
#endif
