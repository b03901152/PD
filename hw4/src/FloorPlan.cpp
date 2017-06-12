#include "FloorPlan.h"
#include <fstream>
#include <vector>

void FloorPlan::parse(string& inputPath) {
	ifstream ifs;
  ifs.open( inputPath.c_str(), ifstream::in );
  if ( !ifs.is_open() ) {
       cout << "Error occur when opening file " << (string)inputPath  << endl;
       exit(1);
  }
  else
      cout << "Read file " << inputPath << " successfully..." << endl; 
  string buf_str;
  unsigned buf_int;
  ifs >> buf_str >> buf_str;
  // cout << "buf_str:" << buf_str << endl;
  ifs.ignore();
  ifs.ignore();
  ifs >> buf_int;
  assert(!buf_int);
  ifs.ignore();
  ifs >> buf_int;
  assert(!buf_int);
  ifs >> buf_str;
  ifs.ignore();
  ifs.ignore();
  ifs >> _W;
  ifs.ignore();
  ifs >> _H;
  ifs >> buf_str >> buf_str >> buf_str;
  unsigned numPins;
  ifs.ignore();
  ifs >> numPins;
  _pins.reserve(2*numPins);
  _names.reserve(numPins);
  unsigned x;
  unsigned y;
  for (unsigned i = 0; i < numPins; ++i) {
    ifs >> buf_str >> buf_str;
  	_names.push_back(buf_str);
    ifs.ignore();
    ifs.ignore();
    ifs >> x;
    ifs.ignore();
    ifs >> y;
    ifs.ignore();
    _pins.emplace_back(new Vertex(x,y));
    // cout << _pins.back()->x() << " "
    // << " " << _pins.back()->y() << _names.back() << endl;
  }


}

void FloorPlan::output(string& outputPath) {
  ofstream ofs;
  ofs.open( outputPath, ofstream::out );
  ofs << "NumRoutedPins = " << _pins.size() << endl;
  for(Vertex* v:_graph._vertices) {
    vector<bool> check_orientation(4, false);
    for(Edge* e: v->_edges) {
      Vertex* u = e->get_the_other_node(v);
      assert(v->x() != u->x() || v->y() != u->y());
      assert(v->x() == u->x() || v->y() == u->y());
      if(v->x() > u->x()) {
        assert(!check_orientation[3]);
        check_orientation[3] = true;
      }
      
      if(v->x() < u->x()) {
        assert(!check_orientation[0]);
        check_orientation[0] = true;
      }
      
      if(v->y() > u->y()) {
        assert(!check_orientation[1]);
        check_orientation[1] = true;
      }
      
      if(v->y() < u->y()) {
        assert(!check_orientation[2]);
        check_orientation[2] = true;
      }
    }
  }
  unsigned wireLength = 0;
  for(Edge* e: _graph._edges)
    wireLength += Vertex::dist(e->_v1,e->_v2);
  ofs << "WireLength = " << wireLength << endl;
  cout << "MSRT cost: " << wireLength << endl << endl;
  for(Edge* e: _graph._edges) {
    Vertex* v = e->_v1;
    Vertex* u = e->_v2;
    if (u->x() == v->x())
      ofs << "V-line ";
    else
      ofs << "H-line ";
    ofs << " (" << v->x() << "," << v->y() << ") ("
    << u->x() << "," << u->y() << ")" << endl;
  }
  cout << "output:" << outputPath << endl; 
}