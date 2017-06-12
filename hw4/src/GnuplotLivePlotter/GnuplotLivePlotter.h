#include <algorithm> 
#ifndef RECTANGLE_H
#define RECTANGLE_H
using namespace std;
class Rectangle
{
public:

    Rectangle(double left = 0, double bottom = 0, double right = 0, double top = 0)
        : _left(left), _bottom(bottom), _right(right), _top(top)
    {}

    /////////////////////////////////////////////
    // get
    /////////////////////////////////////////////
    double left() const {return _left;}
    double bottom() const {return _bottom;}
    double right() const {return _right;}
    double top() const {return _top;}
    double width() const {return _right - _left;}
    double height() const {return _top - _bottom;}
    double centerX() const {return (_left + _right)/2;}
    double centerY() const {return (_bottom + _top)/2;}

    /////////////////////////////////////////////
    // set
    /////////////////////////////////////////////
    void setBounds(double left, double bottom, double right, double top) {
        _left = left;
        _bottom = bottom;
        _right = right;
        _top = top;
    }

    /////////////////////////////////////////////
    // overlap area of two rectangles
    /////////////////////////////////////////////
    static double overlapArea(const Rectangle &rect1, const Rectangle &rect2) {
        double overlapH = min(rect1.right(), rect2.right()) - max(rect1.left(), rect2.left());
        double overlapV = min(rect1.top(), rect2.top()) - max(rect1.bottom(), rect2.bottom());
        if (overlapH < 0) overlapH = 0;
        if (overlapV < 0) overlapV = 0;
        return overlapH * overlapV;
    }

private:
    double _left;
    double _bottom;
    double _right;
    double _top;
};

#endif // RECTANGLE_H

#ifndef GNUPLOTLIVEPLOTTER_H
#define GNUPLOTLIVEPLOTTER_H

#include <vector>
#include <string>
#include <cstdio>
using namespace std;


class GnuplotLivePlotter
{
public:
    GnuplotLivePlotter();
    ~GnuplotLivePlotter();

    void reset();

    /////////////////////////////////////////////
    // set
    /////////////////////////////////////////////
    void setTitle(string title);
    void setRange(double , double , double , double );
    void set(string s) {
        // fprintf(_fpipe, "set arrow 2 from 0,0 to 10,14 lc rgb \"gray\" nohead front\n");
        fprintf(_fpipe,"%s",s.c_str() );
    }
    /////////////////////////////////////////////
    // output result
    /////////////////////////////////////////////
    //void outputPlotFile(string filePathName);
    //void outputPdfFile(string filePathName);

    void show();

    /////////////////////////////////////////////
    // add/clear lines, points, blocks
    /////////////////////////////////////////////
    void addBlock( double, double, double, double );
    void addLine( vector< vector<double> >& );
    void addPoint( double, double );
    void addLabel(double x,double y,string s) {
        int num = stoi(s);
        if(num<=10) {
            fprintf(_fpipe,"set label '%s' at %f , %f\n",s.c_str(),x,y);
        }
        return;
        // fprintf(_fpipe, "\n");
        // _gp << "set label \"" << s << "\" at " << x 
        // << ", " << y << endl;
    };
    // debug
    void debug();

private:
    // pipe
    FILE *_fpipe;

    // plots
    vector< vector<double> >            _blocks; // LLx LLy URx URy
    vector< vector< vector<double> > >  _lines;
    vector< vector<double> >            _points;
    vector< vector<double> >            _label_pos;
    vector<string>                      _label;

    // properties
    string _title;
    vector<double> _range; // x_left x_right y_bottom y_top
};

#endif