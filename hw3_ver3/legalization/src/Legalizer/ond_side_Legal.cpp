#include "Legal.h"
#include "arghandler.h"
#include "GnuplotLivePlotter.h"
#include "GnuplotMatrixPlotter.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <list>
#include <set>
#include <cassert>
#include "GnuplotLivePlotter.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <list>
#include <set>
#include <cassert>

using namespace std;


class Cluster {
public:
    Cluster(){}
    Cluster(unsigned ID): 
    _ID(ID),_w(0),_e(0), _q(0) {}
    unsigned _ID;
    double _w;
    double _e; // size of _modules
    double _x;
    double _q;
    vector<unsigned> _moduleIDs;
};

class Abucus
{
public:
    Abucus(CLegal& c, Placement& p): _cLegal(c), _placement(p) {
        sort_module_order();
        init();
        int nRow = (int)_placement.numRows();
        int counterTest = 0;
        for(unsigned& moduleID:_ordered_moduleIDs ) {
            cout << "counterTest: " << counterTest++ << endl;
            // cout << "moduleID:" << moduleID << endl;
            double best_delta_cost = numeric_limits<double>::max();
            double y_g = _cLegal.m_globalLocations[moduleID].y;
            unsigned y_begin_idx = floor( (y_g - _placement.boundaryBottom()) / _placement.getRowHeight() );
            bool row_increase = true;
            unsigned rowCounter = 0;
            int rowIdx;
            int best_row_Idx = -1;
            while(1) {
                rowIdx = y_begin_idx + ( row_increase ? rowCounter : -rowCounter );
                // cout << "rowIdx" << rowIdx << endl;
                // cout << "best_delta_cost" << best_delta_cost << endl;
                ++rowCounter;
                if ( rowIdx < 0 || rowIdx == nRow ||
                 best_delta_cost < pow( _placement.boundaryBottom() + rowIdx * _placement.getRowHeight() - y_g ,2 ) ) {
                    if ( row_increase )
                    {
                        row_increase = false;
                        rowCounter = 1;
                        continue;
                    }
                    else
                        break;
                }
                if ( _remain_widths[rowIdx] < module(moduleID).width() )
                    continue;
                saveRow(rowIdx);
                placeRow(rowIdx, moduleID);
                double newCost = rowCost(rowIdx);
                if ( newCost - _cost[rowIdx] < best_delta_cost ) {
                    best_delta_cost = newCost - _cost[rowIdx];
                    best_row_Idx = rowIdx;
                }
                restoreRow(rowIdx);
            }
            assert(best_row_Idx>=0);
            placeRow(best_row_Idx, moduleID);
            _remain_widths[best_row_Idx] -= module(moduleID).width();
            _cost[best_row_Idx] += best_delta_cost;
            /////////////////////////////////
            if (! ( _cost[best_row_Idx] - rowCost(best_row_Idx) < 0.001 ) ){
                cout << "_cost[best_row_Idx]" << _cost[best_row_Idx] << endl;
                cout << "rowCost(best_row_Idx)" << rowCost(best_row_Idx) << endl;
                exit(0);
            }
        }
        setBestPosition();
    };
    ~Abucus(){};
    Module& module(unsigned i){ return _placement.module(i); }
    Row& row(unsigned i ) { return _placement.row(i); }
    void sort_module_order() {
        for (unsigned i = 0; i < _placement.numModules(); ++i)
            _ordered_moduleIDs.emplace_back(i);
        sort(_ordered_moduleIDs.begin(), _ordered_moduleIDs.end(),
        [&](unsigned i, unsigned j) { return _cLegal.m_globalLocations[i].x + module(i).width()/2 < 
            _cLegal.m_globalLocations[j].x + module(j).width()/2; }    );            
    }
    void init() {
        unsigned n = _placement.numRows();
        _remain_widths.resize(n);
        _cost.resize(n);
        _clusters.resize(n);
        double w = row(0).width();
        for (unsigned i = 0; i < n; ++i) {
            _cost[i] = 0;
            _remain_widths[i] = w;
        }
    }
    void placeRow( unsigned rowIdx, unsigned moduleID ) {
        // Module& m = module(moduleID);
        vector<Cluster>& c = _clusters[rowIdx]; 
        if ( c.empty() || ( c.back()._x + c.back()._w < _cLegal.m_globalLocations[moduleID].x ) ) {
            c.emplace_back( _clusters[rowIdx].size() );
            assert(_clusters[rowIdx].back()._w == 0 );
            c.back()._x = _cLegal.m_globalLocations[moduleID].x;
            addCell(c.back(),moduleID,rowIdx);
        }
        else {
            assert(c.back()._x < _placement.boundaryRight());
            addCell(c.back(), moduleID, rowIdx);
            collapseRow(rowIdx);
        }
    }
    void addCell( Cluster& c, unsigned moduleID ,unsigned rowIdx) {
        c._moduleIDs.push_back( moduleID );
        c._e += 1;
        assert( c._e - c._moduleIDs.size() < 0.01 );
        c._q += _cLegal.m_globalLocations[moduleID].x -c._w;
        c._w += module( moduleID ).width();
        assert( _clusters[rowIdx].back()._e == c._e);
        double w = 0;
        for(unsigned i: c._moduleIDs)
            w += module(i).width();
        assert( w - c._w < 0.01);
    }
    void addCluster( Cluster& c1, Cluster& c2 ) {
        // cout << "addCluster" << endl;
        for( unsigned moduleID: c2._moduleIDs )
            c1._moduleIDs.push_back( moduleID );
        c1._e += c2._e;
        c1._q += c2._q - c2._e*c1._w;
        c1._w += c2._w;
        double w = 0;
        for (unsigned moduleID: c1._moduleIDs ) {
            w += module(moduleID).width();
        }
    }
    void collapseRow( unsigned rowIdx ) {
        Cluster& c = _clusters[rowIdx].back();
        c._x = c._q/c._e;
        assert( _clusters[rowIdx].back()._x == c._x );
        if ( c._x < _placement.boundaryLeft() )
            c._x = _placement.boundaryLeft();
        else if (c._x > _placement.boundaryRight() - c._w )
            c._x = _placement.boundaryRight() - c._w;
        if ( c._ID != 0 ) {
            Cluster& pre = _clusters[ rowIdx ][ c._ID - 1];
            if ( pre._x + pre._w <= c._x ) return;
            addCluster( pre , c );
            _clusters[ rowIdx ].pop_back();
            collapseRow( rowIdx );
        }
    }
    void placeRow_reverse ( unsigned rowIdx, unsigned moduleID ) {
    }
    void addCell_reverse( Cluster& c, unsigned moduleID ,unsigned rowIdx) {
    }
    void addCluster_reverse( Cluster& c1, Cluster& c2 ) {
    }
    void collapseRow_reverse( unsigned rowIdx ) {
    }
    void saveRow( unsigned rowIdx ) {
        _save_clusters = _clusters[rowIdx];
    }
    void restoreRow( unsigned rowIdx ) {
        _clusters[rowIdx] = _save_clusters;
    }
    double rowCost( unsigned rowIdx ) {
        double cost = 0;
        double y = row(rowIdx).y();            
        for ( Cluster&c: _clusters[rowIdx] ) {
            double x = c._x;
            // cout << "x " << x << endl;
            for(unsigned moduleID: c._moduleIDs)
            {
                // cout << x << endl;
                cost += pow(x - _cLegal.m_globalLocations[moduleID].x ,2);
                cost += pow(y - _cLegal.m_globalLocations[moduleID].y ,2);
                x += module(moduleID).width();
                // cout << "w" << c._w << endl;
                // cout << "x " << x << endl<<endl;
            }
            // cout << "moduleID size " << c._moduleIDs.size() << endl;
            // cout << "moduleID size " << c._e << endl;
            // cout << "x " << x << endl;
            // cout << (c._x +c._w)<< endl;
            assert( fabs(x - (c._x +c._w)) < 1);
        }
        return cost;
    }
    void setBestPosition() {
        for (unsigned rowIdx = 0; rowIdx < _clusters.size(); ++rowIdx) {
            for(Cluster& c: _clusters[rowIdx]) {
                double x = c._x;
                double y = row(rowIdx).y();            
                for(unsigned& moduleID: c._moduleIDs) {
                    _cLegal.m_bestLocations[moduleID].x = x;
                    _cLegal.m_bestLocations[moduleID].y = y;
                    x += module(moduleID).width();
                    // cout << "x" << x <<  endl;
                    // cout << "module(moduleID).width()" << module(moduleID).width() <<  endl;
                }
                assert( fabs(x - (c._x +c._w) ) < 1);
            }        
        }
    }
private:
    vector<Cluster>             _save_clusters;
    double                      _save_remain_width;

    CLegal&                     _cLegal;
    Placement&                  _placement;
    vector<unsigned>            _ordered_moduleIDs;
    vector<double>              _remain_widths;
    vector<double>              _cost;
    vector<vector<Cluster>>     _clusters;
};

bool CLegal::solve()
{
    // TODO: edit your code HERE
    // Note:
    //      1. You should save your legal solution into m_bestLocations, and call setLegalResult() tp upload it into Placement.
    //      2. Run check() to make sure that the solution is lega.
    //      3. Feel free to add any class, function, and/or data member.
    // Good luck!
    saveGlobalResult();
    clock_t t0 = clock();
    Abucus abacus( *this, _placement );
    // exit(1);
    setLegalResult();
    GnuplotLivePlotter gp;
    gp.setTitle("Legalization");
    gp.setAutoFit(1);
    for (unsigned i = 0; i < _placement.numModules(); i++) {
        Module &module = _placement.module( i );
        gp.addRectangle( module.rectangle() );
    }
    // gp.show();
    // cin.get();
    if( check() )
    {
        cout<< "total displacement: " << totalDisplacement() << endl;
        cout << "Legal time: " << (double)(clock()-t0)/CLOCKS_PER_SEC << endl;
        return true;
    }
    return false;

}

bool sortModule( Module* a, Module* b)
{
    return a->x() < b->x();
}

bool CLegal::check()
{
    cout << "start check" << endl;
    int notInSite=0;
    int notInRow=0;
    int overLap=0;

    ///////////////////////////////////////////////////////
    //1.check all standard cell are on row and in the core region
    //////////////////////////////////////////////////////////
    for(unsigned int i=0; i<_placement.numModules(); ++i)
    {
        Module& module = _placement.module(i);
        double curX = module.x();
        double curY = module.y();

        double res = ( curY - m_site_bottom ) / _placement.getRowHeight();
        //cout << curY << " " << res << endl;
        int ires = (int) res;
        if( (m_site_bottom + _placement.getRowHeight() * ires) != curY )
        {
                cerr<<"\nWarning: cell:"<<i<<" is not on row!!";
                ++notInRow;
        }
        if( (curY<_placement.boundaryBottom()) || (curX<_placement.boundaryLeft())||
                ( (curX+module.width())>_placement.boundaryRight()) ||
                ( (curY+module.height())>_placement.boundaryTop()) )
        {
            cerr<<"\nWarning: cell:"<<i<<" is not in the core!!";
            ++notInSite;
        }
    }

    ///////////////////////////////////////////
    //2. row-based overlapping checking
    ///////////////////////////////////////////

    Rectangle chip = _placement.rectangleChip();
    const double &rowHeight = _placement.getRowHeight();
    unsigned numRows = _placement.numRows();
    vector< vector<Module*> > rowModules( numRows, vector<Module*>( 0 ) );
    for(unsigned int i=0; i<_placement.numModules(); ++i)
    {
        Module& module = _placement.module(i);
        double curY = m_bestLocations[i].y;

        if( module.area() == 0 ) continue;

        double yLow = curY - chip.bottom(), yHigh= curY + module.height() - chip.bottom();
        size_t low = floor( yLow / rowHeight ), high = floor(yHigh / rowHeight);
        if( fabs( yHigh - rowHeight * floor(yHigh / rowHeight) ) < 0.01 ){ --high; }

        for( size_t i = low; i <= high; ++i ){ rowModules[ i ].push_back( &module ); }
    }
    for( size_t i = 0; i < numRows; ++i )
    {
        vector<Module*> &modules = rowModules[i];
        sort(modules.begin(), modules.end(), sortModule);
        if( modules.size() < 1 ) { continue; }
        for( size_t j = 0; j < modules.size() - 1; ++j ){
            Module &mod = *modules[ j ];
            size_t nextId = j+1;
            while( mod.x() + mod.width() > modules[ nextId ]->x() ){
                Module &modNext = *modules[ nextId ];
                if( mod.x() + mod.width() > modules[ nextId ]->x() ){
                    ++overLap;
                    cout << mod.name() << " overlap with " << modNext.name() << endl;
                }
                ++nextId; if( nextId == modules.size() ) { break; }
            }
        }
    }

    /*
    ///////////////////////////////////////////
    //3. bin-based overlapping checking
    ///////////////////////////////////////////

    //3.1 build bin

    for(unsigned int k=0; k<_placement.numModules(); ++k)
    {
        int binStartX=(int)(m_pDB->m_modules[k].m_x/m_binWidth);
        int binEndX=(int)( (m_pDB->m_modules[k].m_x+m_pDB->m_modules[k].m_width)/m_binWidth);
        int binStartY=(int)(m_pDB->m_modules[k].m_y/m_binHeight);
        int binEndY=(int)((m_pDB->m_modules[k].m_y+m_pDB->m_modules[k].m_height)/m_binHeight);
        legalBinID(binStartX); legalBinID(binEndX); legalBinID(binStartY); legalBinID(binEndY);

        for(int i=binStartX; i<=binEndX; ++i) {
            for(int j=binStartY; j<=binEndY; ++j) { m_moduleBins[i][j].push_back(k); }
        }
    }
    //cerr<<"\nFinish build bins";

    //3.2 fow all module, check overlapping inside bin
    for(int k=0; k<(int)m_pDB->m_modules.size(); ++k)
    {
        if( m_pDB->m_modules[k].m_isNI ) continue; // (kaie) 2011-01-08

        int binStartX=(int)(m_pDB->m_modules[k].m_x/m_binWidth);
        int binEndX=(int)( (m_pDB->m_modules[k].m_x+m_pDB->m_modules[k].m_width)/m_binWidth);
        int binStartY=(int)(m_pDB->m_modules[k].m_y/m_binHeight);
        int binEndY=(int)((m_pDB->m_modules[k].m_y+m_pDB->m_modules[k].m_height)/m_binHeight);
        legalBinID(binStartX); legalBinID(binEndX); legalBinID(binStartY); legalBinID(binEndY);
        //for all bins
        for(int m=binStartX; m<=binEndX; ++m) {
            for(int n=binStartY; n<=binEndY; ++n) {
                //for all modules in bins
                for(int i=0; i<(int)m_moduleBins[m][n].size(); ++i) {
                    if(m_moduleBins[m][n][i]!=k) {
                        int mID=m_moduleBins[m][n][i];

                        if( m_pDB->m_modules[mID].m_isNI ) continue; // (kaie) 2011-01-08

                        int nBlock1 = max((int)m_pDB->m_modules[k].m_subBlock.size(), 1);
                        int nBlock2 = max((int)m_pDB->m_modules[mID].m_subBlock.size(), 1);

                        double area = 0;

                        for(int b1 = 0; b1 < nBlock1; b1++) {
                            double left1, bottom1, right1, top1;
                            if(m_pDB->m_modules[k].m_isRect)
                            {
                                left1   = m_pDB->m_modules[k].m_subBlock[b1].m_x;
                                bottom1 = m_pDB->m_modules[k].m_subBlock[b1].m_y;
                                right1  = m_pDB->m_modules[k].m_subBlock[b1].m_x + m_pDB->m_modules[k].m_subBlock[b1].m_width;
                                top1    = m_pDB->m_modules[k].m_subBlock[b1].m_y + m_pDB->m_modules[k].m_subBlock[b1].m_height;
                            }else
                            {
                                left1   = m_pDB->m_modules[k].m_x;
                                bottom1 = m_pDB->m_modules[k].m_y;
                                right1  = m_pDB->m_modules[k].m_x + m_pDB->m_modules[k].m_width;
                                top1    = m_pDB->m_modules[k].m_y + m_pDB->m_modules[k].m_height;
                            }
                            for(int b2 = 0; b2 < nBlock2; b2++)
                            {
                                double left2, bottom2, right2, top2;
                                if(m_pDB->m_modules[mID].m_isRect)
                                {
                                    left2   = m_pDB->m_modules[mID].m_subBlock[b2].m_x;
                                    bottom2 = m_pDB->m_modules[mID].m_subBlock[b2].m_y;
                                    right2  = m_pDB->m_modules[mID].m_subBlock[b2].m_x + m_pDB->m_modules[mID].m_subBlock[b2].m_width;
                                    top2    = m_pDB->m_modules[mID].m_subBlock[b2].m_y + m_pDB->m_modules[mID].m_subBlock[b2].m_height;
                                }else
                                {
                                    left2   = m_pDB->m_modules[mID].m_x;
                                    bottom2 = m_pDB->m_modules[mID].m_y;
                                    right2  = m_pDB->m_modules[mID].m_x + m_pDB->m_modules[mID].m_width;
                                    top2    = m_pDB->m_modules[mID].m_y + m_pDB->m_modules[mID].m_height;
                                }
                                area += getOverlapArea(
                                            left1, bottom1, right1, top1, left2, bottom2, right2, top2
                                            );
                            }
                        }
                        if( (abs( area ) > 0.1) && !(m_pDB->m_modules[k].m_isFixed
                                                     && m_pDB->m_modules[mID].m_isFixed)
                                //(m_pDB->m_modules[k].m_isFixed==false || m_pDB->m_modules[mID].m_isFixed==false )
                                )
                        {
                            cout<<"\nWarning: cell:"<<m_pDB->m_modules[k].m_name
                               <<"("<<m_pDB->m_modules[k].m_x<<","<<m_pDB->m_modules[k].m_y
                              <<","<< m_pDB->m_modules[k].m_width
                             <<") overlap with cell "<<m_pDB->m_modules[mID].m_name
                            <<"("<<m_pDB->m_modules[mID].m_x<<","<<m_pDB->m_modules[mID].m_y
                            <<","<< m_pDB->m_modules[mID].m_width<<")!!Area:"<<area<<"";
                            fflush(stdout);
                            ++overLap;
                        }
                    }
                }
            }
        }
    }*/

    cout << endl <<
            "  # row error: "<<notInRow<<
            "\n  # site error: "<<notInSite<<
            "\n  # overlap error: "<<overLap<< endl;
    //cout << "end of check" << endl;

    if( notInRow!=0 || notInSite!=0 || overLap!=0 )
    {
        cout <<"Check failed!!" << endl;
        return false;
    }
    else
    {
        cout <<"Check success!!" << endl;
        return true;
    }
}

double CLegal::totalDisplacement()
{
    double totaldis = 0;
    for( unsigned  moduleId = 0 ; moduleId < _placement.numModules() ; moduleId++ )
    {
        Module& curModule = _placement.module(moduleId);
        double x = curModule.x();
        double y = curModule.y();

        totaldis += CPoint::Distance(m_globalLocations[moduleId] , CPoint( x, y ));
    }
    return totaldis;
}

CLegal::CLegal( Placement& placement  ) :
    _placement( placement )
{

    //Compute average cell width
    int cell_count = 0;
    double total_width = 0;
    //double max_height = 0.0;
    m_max_module_height = 0.0;
    m_max_module_width = 0.0;
    for( unsigned  moduleId = 0 ; moduleId < placement.numModules() ; moduleId++ )
    {
        Module& curModule = placement.module(moduleId);

        m_max_module_height = max( m_max_module_height, curModule.height() );
        m_max_module_width = max( m_max_module_width, curModule.width() );
    //Do not include fixed cells and macros
        if( curModule.isFixed() || curModule.height() > placement.getRowHeight() )
        continue;

        cell_count++;
        total_width += curModule.width();
    }

    m_average_cell_width = total_width / cell_count;

    m_free_sites = placement.m_sites;
    m_site_bottom = m_free_sites.front().y();
    m_site_height = m_free_sites.front().height();

    //initalize m_origLocations and m_bestLocations
    m_bestLocations.resize( placement.numModules() );
    m_globalLocations.resize( placement.numModules() );
    m_chip_left_bound = placement.rectangleChip().left();

}

void CLegal::saveGlobalResult()
{
    for (unsigned moduleId = 0; moduleId < _placement.numModules(); moduleId++)
    {
        Module &curModule = _placement.module(moduleId);
        double x = curModule.x();
        double y = curModule.y();

        m_globalLocations[moduleId] = CPoint( x, y );
        m_bestLocations[moduleId] = CPoint( x, y );
    }
}

void CLegal::setLegalResult()
{
    for (unsigned moduleId = 0; moduleId < _placement.numModules(); moduleId++)
    {
        Module &curModule = _placement.module(moduleId);
        curModule.setPosition(m_bestLocations[moduleId].x,m_bestLocations[moduleId].y);
    }
}
