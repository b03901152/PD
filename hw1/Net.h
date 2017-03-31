#ifndef NET_H
#define NET_H
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "Cell.h"
#include <list>

using namespace std;

class Net
{
  public:
    Net() {}
    ~Net() {}

    vector< vector<Cell*> > cells;
	vector<unsigned> leftNodeNum;
	vector<unsigned> rightNodeNum;
};

#endif
