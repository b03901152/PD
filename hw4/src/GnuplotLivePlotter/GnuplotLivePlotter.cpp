#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <math.h>

#include "GnuplotLivePlotter.h"


#include <fstream>
#include <limits>

GnuplotLivePlotter::GnuplotLivePlotter()
{
	_title = "";
    _fpipe = (FILE *)popen("gnuplot", "w");
    _lines.push_back({{0,0},{0,0}});
}

GnuplotLivePlotter::~GnuplotLivePlotter()
{
    pclose(_fpipe);
}

void GnuplotLivePlotter::reset()
{
	_blocks.clear();
	_lines.clear();
	_points.clear();
	_label_pos.clear();
	_label.clear();
}
void GnuplotLivePlotter::setRange( double left_x, double right_x, double bottom_y, double top_y )
{
    vector<double> range {left_x, right_x, bottom_y, top_y};
    _range = range;
}


void GnuplotLivePlotter::addBlock( double left, double bottom, double right, double top )
{
    vector<double> block{left,bottom,right,top}; 
	_blocks.push_back(block);


}

void GnuplotLivePlotter::addLine( vector< vector<double> >& l ) {
	_lines.push_back(l);
}

void GnuplotLivePlotter::addPoint( double x, double y) {
    vector<double> d = {x,y};
	_points.push_back(d);
}

void GnuplotLivePlotter::setTitle(string title)
{
    _title = title;
}

void GnuplotLivePlotter::show()
{
    // set properties
    fprintf(_fpipe, "set title \"%s\"\n", _title.c_str());
    fprintf(_fpipe, "set size ratio -1\n");
    fprintf(_fpipe, "set nokey\n");
    fprintf(_fpipe, "set tics scale 0\n");
    fprintf(_fpipe, "set clip two\n");
    fprintf(_fpipe, "set term x11 persist\n");
    // fprintf(_fpipe, "\nset label  \"ZZZ\" at 8 , 8;\n");
    unsigned counter = 0;

    //print block
    for ( auto& block: _blocks )
        fprintf(_fpipe, "set object %d rect from %f,%f to %f,%f fc rgb \"gray\"\n",
            ++counter,block[0],block[1],block[2],block[3]);
    // print points
    for ( auto& point: _points ) {
        fprintf(_fpipe, "set object %d circle at  %f,%f  radius char 0.2 fillcolor rgb 'red'\n",
            ++counter, point[0], point[1]);
    }

    // set range and plot
    fprintf(_fpipe, "set xrange[%f:%f]\n", _range[0], _range[1]);
    fprintf(_fpipe, "set yrange[%f:%f]\n", _range[2], _range[3]);

	fprintf(_fpipe, "plot '-' with lines lt 1\n");
    // fprintf(_fpipe, "%f %f\n", _range[0], _range[2]);
    // fprintf(_fpipe, "%f %f\n", _range[0], _range[3]);
    // fprintf(_fpipe, "%f %f\n", _range[1], _range[3]);
    // fprintf(_fpipe, "%f %f\n", _range[1], _range[2]);
    // fprintf(_fpipe, "%f %f\n", _range[0], _range[2]);
    // fprintf(_fpipe, "\n");
    for(auto& line: _lines) {
        for(auto& point: line)
            fprintf(_fpipe, "%f %f\n", point[0], point[1]);
        fprintf(_fpipe, "\n");
    }
    fprintf(_fpipe, "EOF\n");
    fflush(_fpipe);
    cin.get();
}

void GnuplotLivePlotter::debug()
{
	cout<<"debug"<<endl;
	setTitle("test");
    setRange(0,4,3,7);
	
    addBlock(2,3,4,5);
    addPoint( 3,7 );
    addPoint( 4,2 );
	// addBlock(23,32,45,45);
	vector<vector<double> > lines = { {2,3}, {5,7}, {10,3} };
	addLine(lines);

    // fprintf(_fpipe, ",\n");

    // fprintf(_fpipe, "plot '-' with lines\n");
    // fprintf(_fpipe, "%d %d\n", 1, 2);
    // fprintf(_fpipe, "%d %d\n", 2, 2);

    // fprintf(_fpipe, "EOF\n");

    // fprintf(_fpipe, "plot '-' with lines\n");
    // fprintf(_fpipe, "%f %f\n", 2, 1);

    // fprintf(_fpipe, "%f %f\n", 1, 1);
    // fprintf(_fpipe, "EOF\n");

    // fprintf(_fpipe, "plot '-' with points lt 2\n");
    // fprintf(_fpipe, "%f %f\n", 2, 4);

    // fprintf(_fpipe, "%f %f\n", 1, 7);
    // fprintf(_fpipe, "EOF\n");
    show();
}
