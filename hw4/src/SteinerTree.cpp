#include "SteinerTree.h"
#include <set>

void SteinerTree::construnction() {
  init();
  Kruskal();
  generate_pairs();
  connect();
  rect();
}

void SteinerTree::addSteinerPoints() {
  init();
  Kruskal();
  generate_pairs();
  connect();
  for(Vertex* v:_vertices)
    v->_edges.clear();
  for(Edge*e :_edges)
    delete e;
  sort(
    _vertices.begin(),
    _vertices.end(),
    [](Vertex* v1, Vertex* v2) {
      if( v1->x() < v2->x() )
        return true;
      if( v1->x() == v2->x() && v1->y() < v2->y() )
        return true;
      return false;
    }
  );
  _FP._pins.clear();
  _FP._pins.reserve(_vertices.size());
  _FP._pins.push_back(_vertices[0]);
  for (unsigned i = 1; i < _vertices.size(); ++i) {
    if (_vertices[i-1]->x() != _vertices[i]->x() || _vertices[i-1]->y() != _vertices[i]->y())
      _FP._pins.push_back(_vertices[i]);
  }

}

void SteinerTree::addMSTEdge(Edge* e) {
  if(e->_v1->x() == e->_v2->x() && e->_v1->y() == e->_v2->y())
    return;
  _MST.push_back(e);
  e->_v1->_edges.push_back(e);
  e->_v2->_edges.push_back(e);
}

void SteinerTree::init() {
	sort( 
		_edges.begin(),
		_edges.end(),
		[](Edge* i,Edge* j)
		{
			unsigned len_i = Vertex::dist(i->_v1,i->_v2);
			unsigned len_j = Vertex::dist(j->_v1,j->_v2);
			return len_i < len_j;
		}
	);

	nVertices = _graph.getVerticesNum();
	for (unsigned i = 0; i < nVertices; ++i) {
		Vertex* v = _vertices[i];
		v->_ID = i;
	}

	nEdges = _graph.getEdgesNum();
	for (unsigned i = 0; i < nEdges; ++i) {
		Edge*e = _edges[i];
		e->_ID = nVertices + i;
	}

	for (unsigned i = 0; i < nVertices; ++i)
		_nodes.push_back(new Node(i));
	for (unsigned i = 0; i < nEdges; ++i)
		_nodes.push_back(new Node(i+nVertices));
}

void SteinerTree::lca_tree_edge(unsigned edge) {
	assert(edge>=nVertices);
	unsigned u = _edges[edge - nVertices]->_v1->_ID;
	unsigned v = _edges[edge - nVertices]->_v2->_ID;
	unsigned uRoot = findSet(u);
	unsigned vRoot = findSet(v);
	_nodes[edge]->_child.push_back(uRoot);
	_nodes[edge]->_child.push_back(vRoot);
	_nodes[uRoot]->_parent = edge;
	_nodes[vRoot]->_parent = edge;
	_root = _nodes[edge];
	assert(_nodes[edge]->_child.size() == 2);
}

void SteinerTree::printNode(Node* n) {
	if(n->_child.empty()) return;
	cout << "Node: " << n->_ID << endl;
	if(n->_ID >= nVertices) {
		cout << "contain vertex: " << _edges[n->_ID - nVertices]->_v1->_ID << endl;
		cout << "contain vertex: " << _edges[n->_ID - nVertices]->_v2->_ID << endl;
	}
	cout << "child: ";
	for(unsigned& c:n->_child)
		cout << c << " ";
	cout << endl << endl;
	for(unsigned& c:n->_child)
		printNode(_nodes[c]);
}

void SteinerTree::resetNodesParameter() {
	for(Node* n:_nodes) {
		n->_parent = n->_ID;
		n->_rank = 0;
		assert(n->_child.empty() || n->_child.size() == 2);
		assert(!n->_isBlack);
	}
}

void SteinerTree::generate_pairs() {
	for (unsigned i = 0; i < _lca_query.size(); ++i) {
		auto& query = _lca_query[i];
		_nodes[query[0]]->_lca_query.push_back({query[0],query[1],i});
		_nodes[query[1]]->_lca_query.push_back({query[0],query[1],i});
	}
	_counter = 0;
	resetNodesParameter();
	TarjanOLCA(_root);
	for(auto& q:_lca_query) {
		Edge* subEdge = _edges[q[2]-nVertices];
		Edge* longestEdge = _edges[q[1]-nVertices];
		Vertex* nbr_v = _vertices[q[0]];
		int before = Vertex::dist(longestEdge->_v1,longestEdge->_v2);
		unsigned max_x = max(subEdge->_v1->x(), subEdge->_v2->x());
		unsigned min_x = min(subEdge->_v1->x(), subEdge->_v2->x());
		unsigned max_y = max(subEdge->_v1->y(), subEdge->_v2->y());
		unsigned min_y = min(subEdge->_v1->y(), subEdge->_v2->y());
		int after = 0;
		after += max((int)nbr_v->x() - (int)max_x,0);
		after += max((int)nbr_v->y() - (int)max_y,0);
		after += max((int)min_x - (int)nbr_v->x(),0);
		after += max((int)min_y - (int)nbr_v->y(),0);
		_lca_query_gain.push_back(before - after);
	}
}

void SteinerTree::TarjanOLCA(Node* u) {
	u->_ancestor = u->_ID;
	for(unsigned& c:u->_child) {
		assert(u->_ID >= nVertices);
		TarjanOLCA(_nodes[c]);
		unionSet(u->_ID,c);
		_nodes[findSet(u->_ID)]->_ancestor = u->_ID;
	}
	u->_isBlack = true;
	for(auto& pair:u->_lca_query) {
		assert(pair.size() == 3);
		Node* v;
		v = (_nodes[pair[0]] == u )?_nodes[pair[1]]:_nodes[pair[0]];
		if (!v->_isBlack)
			continue;
		unsigned lca = _nodes[findSet(v->_ID)]->_ancestor;
		assert(lca >= nVertices);
		assert(lca <= _lca_query.size());
		_lca_query[pair[2]][1] = lca;
		// cout << _lca_query[pair[2]][0] << " " << _lca_query[pair[2]][1] << " " << _lca_query[pair[2]][2] << endl;
		// cout << endl;
		_counter++;
	}
}

void SteinerTree::unionSet(unsigned x, unsigned y) {
	Node* xRoot = _nodes[findSet(x)];
	Node* yRoot = _nodes[findSet(y)];
	if( xRoot->_rank > yRoot->_rank )
		yRoot->_parent = xRoot->_ID;
	else if( xRoot->_rank < yRoot->_rank )
		xRoot->_parent = yRoot->_ID;
	else
	{
		yRoot->_parent = xRoot->_ID;
		++(xRoot->_rank);
	}
	assert(findSet(x)>=nVertices);
}

unsigned SteinerTree::findSet(unsigned x) {
	Node* n = _nodes[x];
	if(n->_parent != n->_ID)
		n->_parent = findSet(n->_parent);
	return n->_parent;
}

void SteinerTree::plot() {
	GnuplotLivePlotter _gp;
	_gp.setRange(0,_FP._W,0,_FP._H);
	_graph.plot(_gp);
}
