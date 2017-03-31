#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>
#include <iostream>
#include "Net.h"
#include <list>


using namespace std;

class Cell
{
  public:
  	Cell( unsigned id ) {
      gain = 0;
      ID = id;
      pre = post = this;
      locked = false;
  	}
  	~Cell() {}

    void addCell( Cell* addedCell ) {
      addedCell->post = post;
      addedCell->pre = this;
      post->pre = addedCell;
      post = addedCell;
    }

    vector<unsigned> netIDs;
  	int gain;
    unsigned ID;
    bool isLeftPart;
    Cell* pre;
    Cell* post;
    vector< list<Cell*>::iterator > it;
    bool locked;
};

#endif