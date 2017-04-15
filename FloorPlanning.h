#ifndef FP_H
#define FP_H

#include <string>
#include "hash.h"
#include <vector>

class Terminal;

class Net
{
  public:
    Net( unsigned x, unsigned y, string& n)
    :posx(x), posy(y), name(n) {}
    vector<Terminal*> terminals;
    unsigned          posx;
    unsigned          posy;
    string            name;
};

// enum Type = { BLOCK, TERMINAL };

class Terminal
{
  public:
    Terminal( unsigned x, unsigned y, string& n)
    :posx(x), posy(y), name(n) {}
    unsigned          posx;
    unsigned          posy;
    vector<Net*>      nets;
    string            name;
};

class Block : public Terminal
{
  public:
    Block( unsigned w, unsigned h, string& n )
    :Terminal( 0, 0, n ), W(w), H(h) {}
    ~Block() {}
    unsigned          W;
    unsigned          H;
    string            name;
};


  
class FloorPlanning
{
  public:
  	FloorPlanning() {

    }
  	~FloorPlanning() {}
    void              parse( string&, string& );


    unsigned          W;
    unsigned          H;
    unsigned          nBlocks;
    unsigned          nTerminals;
    unsigned          nNets;
    vector<Block*>    Blocks; 
    vector<Net*>      Nets;
    vector<Terminal*> Terminals;

};


#endif