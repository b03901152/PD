#include <algorithm>
#include <set>
#include <boost/range/adaptor/reversed.hpp>
#include "SpanningTree.h"
void SpanningTree::init() {
	initMap();
	initGraph();
}

void SpanningTree::initGraph() {
	_graph._vertices.clear();
	_graph._edges.clear();
	_graph._vertices.reserve(_pins.size());
	_graph._edges.reserve(8*_pins.size());
	for(Vertex* v:_pins)
		_graph.addVertex(v);
}

void SpanningTree::initMap() {
	_sort_vec.clear();
	_sort_reverse_vec.clear();
	_sort_vec.reserve(_pins.size());
	_sort_reverse_vec.reserve(_pins.size());
	for (unsigned i = 0; i < _pins.size(); ++i) {
		_pins[i]->_ID = i;
		_sort_vec.push_back(i);
		_sort_reverse_vec.push_back(i);
	}
	sort(
		_sort_vec.begin(),
		_sort_vec.end(),
		[&](unsigned& v1, unsigned& v2) {
			return _pins[v1]->x() + _pins[v1]->y() >
			_pins[v2]->x() + _pins[v2]->y();
		}
	);
	
	sort(
		_sort_reverse_vec.begin(),
		_sort_reverse_vec.end(),
		[&](unsigned& v1, unsigned& v2) {
			int value1 = _pins[v1]->x() - _pins[v1]->y(); 
			int value2 = _pins[v2]->x() - _pins[v2]->y(); 
			return  value1 > value2;
		}
	);

}

void SpanningTree::construnction() {
	init();
	connect_neighbors_R8();
	connect_neighbors_R1();
	connect_neighbors_R2R7();
}

void SpanningTree::connect_neighbors_R8() {
	auto compareY = [&](unsigned i, unsigned j) {
		return _pins[i]->y() > _pins[j]->y();
	};
	multiset<unsigned,decltype(compareY)> actSet(compareY);
	for(unsigned idx: _sort_reverse_vec) {
		Vertex* v = _pins[idx];
		auto it = actSet.upper_bound(idx);
		if  ( it == actSet.end() ) {
			actSet.emplace(idx);
			continue;
		}
		Vertex* neighbor = _pins[*it];
		assert(neighbor!=v);
		Vertex* nearest = NULL;
		assert(neighbor != v);
		double slope = Vertex::slope(*neighbor,*v); 
		while (slope >= -1 && slope < 0) {
			if( !nearest )
				nearest = neighbor;
			int delta = neighbor->x() - neighbor->y() - nearest->x() + nearest->y();
			if(delta < 0)
				nearest = neighbor;
			auto erase_it = it;
			++it;
			if (it == actSet.end())
				break;

			actSet.erase(erase_it);
			neighbor = _pins[*it];
			if(neighbor == v)
			{
				++it;
				if (it == actSet.end())
					break;
			}
			assert(neighbor != v);
			slope = Vertex::slope(*neighbor,*v);
		}
		if(nearest)
			_graph.addEdge(new Edge(v,nearest));
		actSet.emplace(idx);
	}
}


void SpanningTree::connect_neighbors_R1() {
	auto compareY = [&](unsigned i, unsigned j) {
		return _pins[i]->y() < _pins[j]->y();
	};
	
	multiset<unsigned,decltype(compareY)> actSet(compareY);
	for (unsigned i = 0; i < _sort_vec.size(); ++i) {
		unsigned idx = _sort_vec[i];
		Vertex* v = _pins[idx];
		auto it = actSet.lower_bound(idx);
		if ( it == actSet.end()) {
			actSet.emplace(idx);
			continue;
		}
		Vertex* neighbor = _pins[*it];
		if(neighbor == v) continue;
		Vertex* nearest = NULL;
		double slope = Vertex::slope(*neighbor,*v); 
		while (slope < 1 && slope >= 0) {
			if(slope != 1) {
				if (!nearest || 
					neighbor->x() + neighbor->y() < nearest->x() + nearest->y() ) {
					nearest = neighbor;
				}
				// _graph.addEdge(new Edge(v,neighbor));
			}
			auto erase_it = it;
			++it;
			actSet.erase(erase_it);
			if (it == actSet.end())
				break;
			neighbor = _pins[*it];
			slope = Vertex::slope(*neighbor,*v);
		}
		if(nearest)
			_graph.addEdge(new Edge(v,nearest));
		actSet.emplace(idx);
	}

}

void SpanningTree::connect_neighbors_R2R7() {
	auto compareX = [&](unsigned i, unsigned j) {
		return _pins[i]->x() < _pins[j]->x();
	};

	//R2
	multiset<unsigned,decltype(compareX)> actSet(compareX);
	for (unsigned i = 0; i < _sort_vec.size(); ++i) {
		unsigned idx = _sort_vec[i];
		Vertex* v = _pins[idx];
		auto it = actSet.upper_bound(idx);
		if ( it == actSet.end()) {
			actSet.emplace(idx);
			continue;
		}
		Vertex* neighbor = _pins[*it];
		Vertex* nearest = NULL;
		while (Vertex::slope(*neighbor,*v) >= 1) {
			if (!nearest || nearest->x() + nearest->y() > neighbor->x() + neighbor->y() )
				nearest = neighbor;
			auto erase_it = it;
			++it;
			actSet.erase(erase_it);
			if (it == actSet.end())
				break;
			neighbor = _pins[*it];
		}
		if(nearest)
			_graph.addEdge(new Edge(v,nearest));
		actSet.emplace(idx);
	}

	//R7
	actSet.clear();
	for (unsigned i = 0; i < _sort_reverse_vec.size(); ++i) {
		unsigned idx = _sort_reverse_vec[i];
		Vertex* v = _pins[idx];
		auto it = actSet.lower_bound(idx);
		if ( it == actSet.end()) {
			actSet.emplace(idx);
			continue;
		}
		Vertex* neighbor = _pins[*it];
		Vertex* nearest = NULL;
		while ( Vertex::slope(*neighbor,*v) < -1) {
			if(!nearest)
				nearest = neighbor;
			int delta = - nearest->x() + nearest->y() + neighbor->x() - neighbor->y();
			if(delta < 0)
				nearest = neighbor;
			auto erase_it = it;
			++it;
			actSet.erase(erase_it);
			if (it == actSet.end())
				break;
			neighbor = _pins[*it];
		}
		if(nearest)
			_graph.addEdge(new Edge(v,nearest));
		actSet.emplace(idx);
	}

}

void SpanningTree::plot() {
	GnuplotLivePlotter gp;
	gp.setRange(0,_FP._W,0,_FP._H);
	_graph.plot(gp);
}