#ifndef STEINERTREE_H
#define STEINERTREE_H
#include <limits>
#include "Graph/Graph.h"
#include "FloorPlan.h"
class Node
{
public:
	Node(unsigned ID)
	:_ID(ID), _parent(ID),_ancestor(ID),
	_rank(0),_isBlack(false)
	{};


	~Node(){};

	unsigned 			_ID;
	unsigned 			_parent;
	unsigned			_ancestor;
	unsigned 			_rank;
	vector<unsigned>	_child;
	bool				_isBlack;
	vector<vector<unsigned> > _lca_query;
};


class SteinerTree
{
public:
	SteinerTree(FloorPlan& FP)
	:_graph(FP._graph),_FP(FP),_root(NULL),_vertices(FP._graph._vertices),_edges(FP._graph._edges) {};
	~SteinerTree() {};
	void construnction();
	void Kruskal();
	void init();
	void addLCA(Edge* e);
	void plot();
	unsigned findSet(unsigned x);
	void lca_tree_edge(unsigned edgeID);
	void unionSet(unsigned x,unsigned y);
	void generate_pairs();
	void TarjanOLCA(Node* u);
	void resetNodesParameter();
	void printNode(Node* n);
	void connect();
	void tranformToRectLine();
	void removeOverlap();
private:
	unsigned 					_counter;
	Graph& 						_graph;
	FloorPlan& 					_FP;
	vector< vector<unsigned> >  _lca_query;
	vector<int>			_lca_query_gain;
	vector<Node*>				_nodes; //merging binary tree
	Node* 						_root;
	unsigned 					nVertices;
	unsigned 					nEdges;
	vector<Vertex*>& 			_vertices;
	vector<Edge*>& 				_edges;
	vector<Edge*>				_MST;
	GnuplotLivePlotter 			_gp;

};


#endif
