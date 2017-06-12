#include "SteinerTree.h"
#include <set>
void SteinerTree::construnction() {
	init();
	Kruskal();
	// plot();
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

void SteinerTree::Kruskal() {
	unsigned counter = 0;
	for(Edge*e: _edges) {
		Vertex* u_v = e->_v1;
		Vertex* v_v = e->_v2;
		unsigned u = u_v->_ID;
		unsigned v = v_v->_ID;
		if( findSet(u) == findSet(v) )
			continue;
		set<Vertex*> w_vs;
		for(Edge* nbr_e: u_v->_edges) {
			Vertex* w_v = nbr_e->get_the_other_node(u_v);
			w_vs.emplace(w_v);
		}
		for(Edge* nbr_e: v_v->_edges) {
			Vertex* w_v = nbr_e->get_the_other_node(u_v);
			w_vs.emplace(w_v);
		}
		for(Vertex* w_v:w_vs) {
			if (w_v == v_v || w_v == u_v)
				continue;
			unsigned w = w_v->_ID;
			if(findSet(w) == findSet(u))
				_lca_query.push_back({w,u,e->_ID});
			else if (findSet(w) == findSet(v))
				_lca_query.push_back({w,v,e->_ID});
			else {
				_lca_query.push_back({w,v,e->_ID});
				_lca_query.push_back({w,u,e->_ID});
			}
			unsigned x = w_v->x();
			unsigned y = w_v->y();
			unsigned max_x = max(u_v->x(),v_v->x());
			unsigned min_x = min(u_v->x(),v_v->x());
			unsigned max_y = max(u_v->y(),v_v->y());
			unsigned min_y = min(u_v->y(),v_v->y());
			assert( 
				x >= max_x ||
				x <= min_x ||
				y >= max_y || 
				y <= min_y);
		}
		lca_tree_edge(e->_ID);
		unionSet(u,v);
		e->_selected = true;
		if(++counter == nVertices -1)
			break;
	}
	unsigned MST_cost = 0;
	for(Edge*e : _edges) {
		if(e->_selected)
			MST_cost += Vertex::dist(e->_v1,e->_v2);
	}
	cout << "MST cost: " << MST_cost << endl;
	if(0) {
		for(Edge*e:_edges)
			if(e->_selected)
				_MST.push_back(e);
		_edges = _MST;
		plot();
		return;
	}
	for(Edge* e:_edges)
		assert(e->_v1 != e->_v2);
	for(Vertex* v: _vertices) {
		for(Edge*e : v->_edges)
			assert(e->_v1 != e->_v2);
	}
	generate_pairs();
	connect();

	if(0) {
		for(Edge*e:_edges)
			if(e->_selected)
				_MST.push_back(e);
		_edges = _MST;
		plot();
		return;
	}
	tranformToRectLine();
	_edges = _MST;
	for (unsigned i = nVertices; i < _vertices.size(); ++i)
		_vertices[i]->_ID = i;
	nVertices = _vertices.size();
	for (unsigned i = 0; i < _edges.size(); ++i)
		_edges[i]->_ID = nVertices + i;
	nEdges = _edges.size();
	for (unsigned i = 0; i < nVertices; ++i)
		_vertices[i]->_edges.clear();
	for(Edge* e: _edges) {
		e->_v1->_edges.push_back(e);
		e->_v2->_edges.push_back(e);
	}
	removeOverlap();	
	plot();
}

void SteinerTree::removeOverlap() {
	for(Edge* e:_edges)
		assert(e->_v1 != e->_v2);
	for(Vertex* v: _vertices) {
		for(Edge*e : v->_edges) {
			assert(e->_v1 != e->_v2);
			assert(e->get_the_other_node(e->_v1) == e->_v2);
		}
	}
	for(Edge* e:_edges)
		e->_selected = true;
	for(Vertex* v:_vertices) {
		vector<Vertex*> check_u(4,NULL);
		vector<Edge*> 	check_e(4,NULL);
		for (unsigned i = 0; i < v->_edges.size(); ++i) {
			Edge* e = v->_edges[i];
			Vertex* u = e->get_the_other_node(v);
			assert(e->_v1 != e->_v2);
			assert(e->_v1 == v || e->_v2 == v);
			assert(e->_v1 == u || e->_v2 == u);
			assert(u!=v);
			if (u->x() > v->x()) {
				if (!check_u[0] || u->x() > check_u[0]->x()) {
					check_u[0] = u;
					if (check_e[0])
						check_e[0]->_selected = false;
					check_e[0] = e;
				}
				else
					e->_selected = false;
			}
			else if (u->x() < v->x()) {
				if (!check_u[2] || u->x() < check_u[2]->x()) {
					check_u[2] = u;
					if (check_e[2])
						check_e[2]->_selected = false;
					check_e[2] = e;
				}
				else
					e->_selected = false;
			}
			else if (u->y() < v->y()) {
				if (!check_u[3] || u->y() < check_u[3]->y()) {
					check_u[3] = u;
					if (check_e[3])
						check_e[3]->_selected = false;
					check_e[3] = e;
				}
				else
					e->_selected = false;
			}
			else if (u->y() > v->y()) {
				if (!check_u[1] || u->y() > check_u[1]->y()) {
					check_u[1] = u;
					if (check_e[1])
						check_e[1]->_selected = false;
					check_e[1] = e;
				}
				else
					e->_selected = false;
			}
			else {
				e->_selected = false;
				assert(v!=u);
				assert (u->_ID > v->_ID);
				v->merge(u);
			}
		}
		v->_edges.clear();
	}
	auto edges = _edges;
	_edges.clear();
	for(Edge* e:edges) {
		if (e->_selected)
			_graph.addEdge(e);
		else
			delete e;
	}
	auto vertices = _vertices;
	_vertices.clear();
	unsigned counter = 0;
	for(Vertex* v: vertices)
		if(!v->_edges.empty()) {
			_vertices.push_back(v);
			v->_ID = counter++;
		}
}

void SteinerTree::tranformToRectLine() {
	unsigned newEdgeNum = 0;
	for(Edge* e: _edges) {
		if(e->_selected)
			++newEdgeNum;
	}
	_edges.reserve(nEdges + newEdgeNum);
	_vertices.reserve(nVertices + newEdgeNum);
	for (unsigned i = 0; i < nEdges; ++i) {
		Edge* e = _edges[i];
		if(e->_selected) {
			double slope;
			slope = Vertex::slope(*e->_v1,*e->_v2);
			int top = 0;
			int bottom = 0;
			int left = 0;
			int right = 0;
			Vertex* v1 = e->_v1;
			Vertex* v2 = e->_v2;
			Vertex* t_v = v1->y() > v2->y() ? v1 : v2;
			Vertex* b_v = v1->y() < v2->y() ? v1 : v2;
			Vertex* l_v = v1->x() < v2->x() ? v1 : v2;
			Vertex* r_v = v1->x() > v2->x() ? v1 : v2;
			if (slope == 0 || slope == numeric_limits<double>::max() 
				|| slope == -numeric_limits<double>::max()) {
				_MST.push_back(e);
				continue;
			}
			if(slope > 0) {
				for(Edge* b_e:b_v->_edges) {
					Vertex* v = b_e->get_the_other_node(b_v);
					if (v->y() == b_v->y() && v->x() > b_v->x())
						bottom += min(v->x(),r_v->x()) - l_v->x();
					else if (v->x() == b_v->x() && v->y() > b_v->y()) 
						left += min(v->y(),t_v->y()) - b_v->y();
				}
				for(Edge* t_e:t_v->_edges) {
					Vertex* v = t_e->get_the_other_node(t_v);
					if (v->y() == t_v->y() && v->x() < t_v->x())
						top += r_v->x() - max(l_v->x(),v->x());
					else if (v->x() == t_v->x() && v->y() < t_v->y())
						right += t_v->y() - max(v->y(),b_v->y());
				}
				assert(left >= 0);
				assert(top >= 0);
				assert(bottom >= 0);
				assert(right >= 0);
				if(left + top >= bottom + right)
					_vertices.push_back(new Vertex(l_v->x(),t_v->y()));
				else
					_vertices.push_back(new Vertex(r_v->x(),b_v->y()));
			}
			else {
				for(Edge* b_e:b_v->_edges) {
					Vertex* v = b_e->get_the_other_node(b_v);
					if (v->y() == b_v->y() && v->x() < b_v->x())
						bottom += r_v->x() - max(v->x(),l_v->x());
					else if (v->x() == b_v->x() && v->y() > b_v->y())
						right += min(v->y(),t_v->y()) - b_v->y();
				}
				for(Edge* t_e:t_v->_edges) {
					Vertex* v = t_e->get_the_other_node(t_v);
					if (v->y() == t_v->y() && v->x() > t_v->x())
						top += min(v->x(),r_v->x()) - l_v->x();
					else if (v->x() == t_v->x() && v->y() < t_v->y())
						left += t_v->y() - max(v->y(),r_v->y());
				}
				assert(left >= 0);
				assert(top >= 0);
				assert(bottom >= 0);
				assert(right >= 0);
				if(left + bottom >= top + right)
					_vertices.push_back(new Vertex(l_v->x(),b_v->y()));
				else
					_vertices.push_back(new Vertex(r_v->x(),t_v->y()));
			}
			for(Edge* e:_edges)
				assert(e->_v1 != e->_v2);
			_graph.addEdge(new Edge(_vertices.back(),v1));
			_MST.push_back(_graph._edges.back());
			_graph.addEdge(new Edge(_vertices.back(),v2));
			_MST.push_back(_graph._edges.back());
		}
	}
}

void SteinerTree::connect() {
	vector<unsigned> _order;
	for (unsigned i = 0; i < _lca_query.size(); ++i)
		_order.push_back(i);
	sort(_order.begin(),_order.end(),
		[&](unsigned i, unsigned j) {
			return _lca_query_gain[i] > _lca_query_gain[j];
		}
	);

	for(auto& idx:_order) {
		auto& q = _lca_query[idx];
		// assert(_lca_query_gain[idx] < 100000);
		if (_lca_query_gain[idx] <= 0)
			return;
		Edge* subEdge = _edges[q[2]-nVertices];
		Edge* longestEdge = _edges[q[1]-nVertices];
		if( !(subEdge->_selected) || !(longestEdge->_selected))
			continue;
		if(longestEdge == subEdge)
			continue;
		Vertex* nbr_v = _vertices[q[0]];
		subEdge->_selected = false;
		longestEdge->_selected = false;
		unsigned max_x = max(subEdge->_v1->x(),subEdge->_v2->x());
		unsigned min_x = min(subEdge->_v1->x(),subEdge->_v2->x());
		unsigned max_y = max(subEdge->_v1->y(),subEdge->_v2->y());
		unsigned min_y = min(subEdge->_v1->y(),subEdge->_v2->y());
		unsigned x = nbr_v->x();
		unsigned y = nbr_v->y();
		unsigned edgeOrientation = 0; 
		Vertex *v = NULL;
		if(x >= max_x) {
			v = new Vertex(max_x,y);
			_vertices.push_back(v);
			_MST.push_back(new Edge(nbr_v,v));
			x = max_x;
			edgeOrientation = 1;
		}
		else if(x <= min_x) {
			v = new Vertex(min_x,y);
			_vertices.push_back(v);
			_MST.push_back(new Edge(nbr_v,v));
			x = min_x;
			edgeOrientation = 2;
		}
		
		if(y >= max_y) {
			Vertex* u = new Vertex(x,max_y);
			_vertices.push_back(u);
			_MST.push_back(new Edge(u,v?v:nbr_v));
			edgeOrientation = 3;
		}

		else if(y <= min_y) {
			Vertex* u = new Vertex(x,min_y);
			_vertices.push_back(u);
			_MST.push_back(new Edge(u,v?v:nbr_v));
			edgeOrientation = 4;
		}
		assert(edgeOrientation);
		double slope = Vertex::slope(*(subEdge->_v1),*(subEdge->_v2));
		if(slope >= 0) {
			if(edgeOrientation == 1 || edgeOrientation == 4) {
				Vertex* v = new Vertex(max_x,min_y);
				_vertices.push_back(v);
				_MST.push_back(new Edge(v,subEdge->_v1));
				_MST.push_back(new Edge(v,subEdge->_v2));
			}
			if(edgeOrientation == 2 || edgeOrientation == 3) {
				Vertex* v = new Vertex(min_x,max_y);
				_vertices.push_back(v);
				_MST.push_back(new Edge(v,subEdge->_v1));
				_MST.push_back(new Edge(v,subEdge->_v2));
			}
		}
		else {
			if(edgeOrientation == 1 || edgeOrientation == 3) {
				Vertex* v = new Vertex(max_x,max_y);
				_vertices.push_back(v);
				_MST.push_back(new Edge(v,subEdge->_v1));
				_MST.push_back(new Edge(v,subEdge->_v2));
			}
			if(edgeOrientation == 2 || edgeOrientation == 4) {
				Vertex* v = new Vertex(min_x,min_y);
				_vertices.push_back(v);
				_MST.push_back(new Edge(v,subEdge->_v1));
				_MST.push_back(new Edge(v,subEdge->_v2));
			}
		}
	}
	for(Edge* e:_MST)
		_graph.addEdge(e);
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
		assert(query.size() == 3);
		_nodes[query[0]]->_lca_query.push_back({query[0],query[1],i});
		_nodes[query[1]]->_lca_query.push_back({query[0],query[1],i});
	}
	_counter = 0;
	resetNodesParameter();
	// printNode(_root);
	TarjanOLCA(_root);
	assert(_counter == _lca_query.size());
	for(auto& q:_lca_query) {
		assert(q[1]>=nVertices);
		assert(q[2]>=nVertices);
		// unsigned edgeID = _lca_query[q[2]][2];
		Edge* subEdge = _edges[q[2]-nVertices];
		assert(subEdge->_selected);
		Edge* longestEdge = _edges[q[1]-nVertices];
		assert(longestEdge->_selected);
		Vertex* nbr_v = _vertices[q[0]];
		int before = Vertex::dist(longestEdge->_v1,longestEdge->_v2);
		unsigned max_x = max(subEdge->_v1->x(), subEdge->_v2->x());
		unsigned min_x = min(subEdge->_v1->x(), subEdge->_v2->x());
		unsigned max_y = max(subEdge->_v1->y(), subEdge->_v2->y());
		unsigned min_y = min(subEdge->_v1->y(), subEdge->_v2->y());
		int after = 0;
		// if(!(nbr_v->x() >= max_x || nbr_v->x() <= min_x 
		// || nbr_v->y() >= max_y || nbr_v->y() <= min_y) ) {
		// 	cout << "nbr_v->x(): " << nbr_v->x() << endl;
		// 	cout << "nbr_v->y(): " << nbr_v->y() << endl;
		// 	cout << "subEdge->_v1: " << subEdge->_v1 << subEdge->_v1->_ID << endl;
		// 	cout << "subEdge->_v2: " << subEdge->_v2 << subEdge->_v2->_ID << endl;
		// 	cout << "max_x: " << max_x << endl;
		// 	cout << "max_y: " << max_y << endl;
		// 	cout << "min_x: " << min_x << endl;
		// 	cout << "min_y: " << min_y << endl << endl;
		// 	cout << "nbr_v: " << nbr_v->_ID << endl;
		// 	cout << "subEdge: " << subEdge->_ID << endl;
		// }
		assert(nbr_v->x() >= max_x || nbr_v->x() <= min_x 
		|| nbr_v->y() >= max_y || nbr_v->y() <= min_y);
		after += max((int)nbr_v->x() - (int)max_x,0);
		after += max((int)nbr_v->y() - (int)max_y,0);
		after += max((int)min_x - (int)nbr_v->x(),0);
		after += max((int)min_y - (int)nbr_v->y(),0);
		_lca_query_gain.push_back(before - after);
	}
	assert(_lca_query_gain.size() == _lca_query.size() );
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
	_gp.setRange(0,_FP._W,0,_FP._H);
	_graph.plot(_gp);
}