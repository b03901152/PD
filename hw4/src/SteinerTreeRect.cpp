#include "SteinerTree.h"
#include <set>
#include <numeric>

void SteinerTree::Kruskal() {
  unsigned counter = 0;
  for(Edge*e: _edges) {
    Vertex* u_v = e->_v1;
    Vertex* v_v = e->_v2;
    unsigned u = u_v->_ID;
    unsigned v = v_v->_ID;
    if( findSet(u_v->_ID) == findSet(v_v->_ID) )
      continue;
    vector<Vertex*> w_vs;
    for(Edge* nbr_e: u_v->_edges) {
      Vertex* w_v = nbr_e->get_the_other_node(u_v);
      w_v->_used = false;
      w_vs.push_back(w_v);
    }

    for(Edge* nbr_e: v_v->_edges) {
      Vertex* w_v = nbr_e->get_the_other_node(u_v);
      w_v->_used = false;
      w_vs.push_back(w_v);
    }

    for(Vertex* w_v:w_vs) {
      if (w_v == v_v || w_v == u_v || w_v->_used)
        continue;
      w_v->_used = true;
      unsigned w = w_v->_ID;
      if(findSet(w) == findSet(u))
        _lca_query.push_back({w,u,e->_ID});
      else if (findSet(w) == findSet(v))
        _lca_query.push_back({w,v,e->_ID});
      else {
        _lca_query.push_back({w,v,e->_ID});
        _lca_query.push_back({w,u,e->_ID});
      }
    }
    lca_tree_edge(e->_ID);
    unionSet(u,v);
    e->_selected = true;
    if(++counter == nVertices -1)
      break;
  }
  _MST_cost = 0;
  for(Edge*e : _edges) {
    if(e->_selected)
      _MST_cost += Vertex::dist(e->_v1,e->_v2);
  }
  // cout <<"MST cost: " << _MST_cost << endl;
}

void SteinerTree::removeOverlap() {
  for(Edge* e:_edges)
    e->_selected = true;
  for(Vertex* v:_vertices) {
    vector<Vertex*> check_u(4,NULL);
    vector<Edge*>   check_e(4,NULL);
    for (unsigned i = 0; i < v->_edges.size(); ++i) {
      Edge* e = v->_edges[i];
      Vertex* u = e->get_the_other_node(v);
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
        assert(0);
        e->_selected = false;
      }
    }
    v->_edges.clear();
  }
  auto edges = _edges;
  _edges.clear();
  _edges.reserve(edges.size());
  for(Edge* e:edges) {
    if (e->_selected)
      _graph.addEdge(e);
    else
      delete e;
  }
  auto vertices = _vertices;
  _vertices.clear();
  _vertices.reserve(vertices.size());
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
  _MST.reserve(_MST.size() + newEdgeNum);
  _vertices.reserve(nVertices + newEdgeNum);
  for (unsigned i = 0; i < nEdges; ++i) {
    Edge* e = _edges[i];
    if(e->_selected) {
      double slope;
      slope = Vertex::slope(*e->_v1,*e->_v2);
      if (slope == 0 || slope == numeric_limits<double>::max() 
        || slope == -numeric_limits<double>::max()) {
        addMSTEdge(e);
        continue;
      }
      unsigned top = 0;
      unsigned bottom = 0;
      unsigned left = 0;
      unsigned right = 0;
      Vertex* v1 = e->_v1;
      Vertex* v2 = e->_v2;
      Vertex* addedVertex = NULL;
      if(v1->x() < v2->x())
        swap(v1,v2);
      Vertex* t_v = v1->y() > v2->y() ? v1 : v2;
      Vertex* b_v = v1->y() < v2->y() ? v1 : v2;
      Vertex* l_v = v1->x() < v2->x() ? v1 : v2;
      Vertex* r_v = v1->x() > v2->x() ? v1 : v2;
      if(slope > 0) {
        for(Edge* b_e:b_v->_edges) {
          Vertex* v = b_e->get_the_other_node(b_v);
          if (v->y() == b_v->y() && v->x() > b_v->x())
            bottom = max(bottom,min(v->x(),r_v->x()) - l_v->x());
          else if (v->x() == b_v->x() && v->y() > b_v->y()) 
            left = max(left,min(v->y(),t_v->y()) - b_v->y());
        }
        for(Edge* t_e:t_v->_edges) {
          Vertex* v = t_e->get_the_other_node(t_v);
          if (v->y() == t_v->y() && v->x() < t_v->x())
            top = max(top,r_v->x() - max(l_v->x(),v->x()));
          else if (v->x() == t_v->x() && v->y() < t_v->y())
            right = max(right,t_v->y() - max(v->y(),b_v->y()));
        }
        if(left + top >= bottom + right) {
          addedVertex = new Vertex(l_v->x(),t_v->y());
          if (Vertex::same(addedVertex,v1) || Vertex::same(addedVertex,v2)) {
            delete addedVertex;
            addedVertex = NULL;
          }
          else
            _vertices.push_back(addedVertex);
        }
        else {
          addedVertex = new Vertex(r_v->x(),b_v->y());
          if (Vertex::same(addedVertex,v1) || Vertex::same(addedVertex,v2)) {
            delete addedVertex;
            addedVertex = NULL;
          }
          else
            _vertices.push_back(addedVertex);
        }
      }
      else {
        for(Edge* b_e:b_v->_edges) {
          Vertex* v = b_e->get_the_other_node(b_v);
          if (v->y() == b_v->y() && v->x() < b_v->x())
            bottom = max(bottom,r_v->x() - max(v->x(),l_v->x()));
          else if (v->x() == b_v->x() && v->y() > b_v->y())
            right = max(right,min(v->y(),t_v->y()) - b_v->y());
        }
        for(Edge* t_e:t_v->_edges) {
          Vertex* v = t_e->get_the_other_node(t_v);
          if (v->y() == t_v->y() && v->x() > t_v->x())
            top = max(top,min(v->x(),r_v->x()) - l_v->x());
          else if (v->x() == t_v->x() && v->y() < t_v->y())
            left = max(left,t_v->y() - max(v->y(),b_v->y()));
        }
        if(left + bottom >= top + right) {
          addedVertex = new Vertex(l_v->x(),b_v->y());
          if (Vertex::same(addedVertex,v1) || Vertex::same(addedVertex,v2)) {
            delete addedVertex;
            addedVertex = NULL;
          }
          else
            _vertices.push_back(addedVertex);
        }
        else {
          addedVertex = new Vertex(r_v->x(),t_v->y());
          if (Vertex::same(addedVertex,v1) || Vertex::same(addedVertex,v2)) {
            delete addedVertex;
            addedVertex = NULL;
          }
          else
            _vertices.push_back(addedVertex);
        }
      }
      if(addedVertex) {
        addMSTEdge(new Edge(addedVertex,v1));
        addMSTEdge(new Edge(addedVertex,v2));
      }
    }
  }
}

void SteinerTree::connect() {
  vector<unsigned> _order;
  unsigned nLCA = _lca_query.size();
  _vertices.reserve(nVertices + nLCA);
  _MST.reserve(_MST.size() + 2*nLCA);
  _order.resize(nLCA);
  iota(_order.begin(), _order.end(), 0);
  vector<double> edgeCounter(_edges.size(),0);
  for(auto& q:_lca_query) {
    edgeCounter[q[1]-nVertices] += 1;
    edgeCounter[q[2]-nVertices] += 1;
  }
  double maxC = 0;
  for(double& d:edgeCounter)
    maxC = max(maxC,d);
  maxC += 1;
  sort(_order.begin(),_order.end(),
    [&](unsigned i, unsigned j) {
      double edge_i = edgeCounter[_lca_query[i][1]-nVertices] + edgeCounter[_lca_query[i][2]-nVertices];
      double edge_j = edgeCounter[_lca_query[j][1]-nVertices] + edgeCounter[_lca_query[j][2]-nVertices];
      double gain_i = _lca_query_gain[i] - 10*edge_i/maxC;
      double gain_j = _lca_query_gain[j] - 10*edge_j/maxC;
      return  gain_i > gain_j;
    }
  );

  for(auto& idx:_order) {
    auto& q = _lca_query[idx];
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
      addMSTEdge(new Edge(nbr_v,v));
      x = max_x;
      edgeOrientation = 1;
    }
    else if(x <= min_x) {
      v = new Vertex(min_x,y);
      _vertices.push_back(v);
      addMSTEdge(new Edge(nbr_v,v));
      x = min_x;
      edgeOrientation = 2;
    }
    
    if(y >= max_y) {
      Vertex* u = new Vertex(x,max_y);
      _vertices.push_back(u);
      addMSTEdge(new Edge(u,v?v:nbr_v));
      edgeOrientation = 3;
    }

    else if(y <= min_y) {
      Vertex* u = new Vertex(x,min_y);
      _vertices.push_back(u);
      addMSTEdge(new Edge(u,v?v:nbr_v));
      edgeOrientation = 4;
    }
    assert(edgeOrientation);
    double slope = Vertex::slope(*(subEdge->_v1),*(subEdge->_v2));
    if(slope >= 0) {
      if(edgeOrientation == 1 || edgeOrientation == 4) {
        Vertex* v = new Vertex(max_x,min_y);
        _vertices.push_back(v);
        addMSTEdge(new Edge(v,subEdge->_v1));
        addMSTEdge(new Edge(v,subEdge->_v2));
      }
      if(edgeOrientation == 2 || edgeOrientation == 3) {
        Vertex* v = new Vertex(min_x,max_y);
        _vertices.push_back(v);
        addMSTEdge(new Edge(v,subEdge->_v1));
        addMSTEdge(new Edge(v,subEdge->_v2));
      }
    }
    else {
      if(edgeOrientation == 1 || edgeOrientation == 3) {
        Vertex* v = new Vertex(max_x,max_y);
        _vertices.push_back(v);
        addMSTEdge(new Edge(v,subEdge->_v1));
        addMSTEdge(new Edge(v,subEdge->_v2));
      }
      if(edgeOrientation == 2 || edgeOrientation == 4) {
        Vertex* v = new Vertex(min_x,min_y);
        _vertices.push_back(v);
        addMSTEdge(new Edge(v,subEdge->_v1));
        addMSTEdge(new Edge(v,subEdge->_v2));
      }
    }
  }
}

void SteinerTree::rect() {
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
}