#ifndef FLOORPLAN_H
#define FLOORPLAN_H
#include "Graph/Graph.h"
#include <string>
class FloorPlan
{
friend class SpanningTree;
friend class SteinerTree;
public:
	FloorPlan(Graph& graph):_graph(graph){};
	void parse(string& inputPath);
	void output(string& outputPath);
	~FloorPlan(){};
private:
	Graph& _graph;
	unsigned _W;
	unsigned _H;
	vector<Vertex*> _pins;
	vector<string> _names;
};
#endif