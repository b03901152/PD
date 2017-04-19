#ifndef FP_H
#define FP_H

#include <string>
#include "hash.h"
#include <vector>

class Terminal;

class Net
{
  public:
    Net( unsigned i ) { netID = i; }
    ~Net() {}

    unsigned          netID;
    vector<Terminal*> terminals;
    bool              isNet;
};

class Terminal
{
  public:
    Terminal( unsigned x, unsigned y, string& n)
    :posx(x), posy(y), name(n), isTerminal(true) {}
    ~Terminal() {};
    unsigned          posx;
    unsigned          posy;
    vector<Net*>      nets;
    string            name;
    bool              isTerminal;
};

class Block : public Terminal
{
  public:
    Block( unsigned w, unsigned h, string& n )
    :Terminal( 0, 0, n ), W(w), H(h), placed(false) {
      isTerminal = false;
    }
    ~Block() {}
    unsigned          W;
    unsigned          H;
    bool              placed;
};
  
class FloorPlanning
{
  public:
    FloorPlanning() {}
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