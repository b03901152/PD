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
        sort_module_order(0.5);
        init();
        int nRow = (int)_placement.numRows();
        bool forward_order = 1;
        int _o_idx = forward_order ? 0 : _ordered_moduleIDs.size() - 1;
        for (; _o_idx < (int)_ordered_moduleIDs.size() && _o_idx >= 0; _o_idx += (forward_order?1:-1) ) {
            unsigned moduleID = _ordered_moduleIDs[_o_idx];
            double best_delta_cost = numeric_limits<double>::max();
            double y_g = _cLegal.m_globalLocations[moduleID].y;
            if (y_g<_placement.boundaryBottom()) y_g = _placement.boundaryBottom();
            unsigned y_begin_idx = floor( (y_g - _placement.boundaryBottom()) / _placement.getRowHeight() );
            bool row_increase = true;
            unsigned rowCounter = 0;
            int rowIdx;
            int best_row_Idx = -1;
            while(1) {
                rowIdx = y_begin_idx + ( row_increase ? rowCounter : -rowCounter );
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
                forward_order ? placeRow(rowIdx, moduleID) : placeRow_reverse(rowIdx, moduleID);
                double d_cost = forward_order ? deltarowCost(rowIdx) : deltarowCost_reverse(rowIdx);
                if ( d_cost < best_delta_cost ) {
                    best_delta_cost = d_cost;
                    best_row_Idx = rowIdx;
                }
                restoreRow(rowIdx);
            }
            assert(best_row_Idx>=0);
            forward_order ? placeRow(best_row_Idx, moduleID) : placeRow_reverse(best_row_Idx, moduleID);
            _remain_widths[best_row_Idx] -= module(moduleID).width();
            if ( _o_idx == (int)_ordered_moduleIDs.size() - 1 && forward_order ){
                forward_order = false;
                _o_idx += 1;
                _forward_clusters_sol = _clusters;
                setBestPosition();
                _cLegal.setLegalResult();
                _forward_displacement = _cLegal.totalDisplacement();
                init();
            }

        }
        setBestPosition_reverse();
        _cLegal.setLegalResult();
        double _reverse_displacement = _cLegal.totalDisplacement();
        cout << "_reverse_displacement:" << _reverse_displacement << endl;
        cout << "_forward_displacement:" << _forward_displacement << endl;
        if ( _forward_displacement < _reverse_displacement )
        {
            _clusters = _forward_clusters_sol;
            setBestPosition();
            _cLegal.setLegalResult();
        }
    };
    ~Abucus(){};
    Module& module(unsigned i){ return _placement.module(i); }
    Row& row(unsigned i ) { return _placement.row(i); }
    void sort_module_order(double x) {
        _ordered_moduleIDs.clear();
        for (unsigned i = 0; i < _placement.numModules(); ++i)
            _ordered_moduleIDs.emplace_back(i);
        sort(_ordered_moduleIDs.begin(), _ordered_moduleIDs.end(),
        [&](unsigned i, unsigned j) { return _cLegal.m_globalLocations[i].x + module(i).width()*x < 
            _cLegal.m_globalLocations[j].x + module(j).width()*x; }    );            
    }
    void init() {
        unsigned n = _placement.numRows();
        _remain_widths.clear();
        _remain_widths.resize(n);
        _clusters.clear();
        _clusters.resize(n);
        double w = row(0).width();
        for (unsigned i = 0; i < n; ++i)
            _remain_widths[i] = w;
        _boundaryRight = _placement.boundaryRight();
    }
    void placeRow_reverse ( unsigned rowIdx, unsigned moduleID ) {
        vector<Cluster>& c = _clusters[rowIdx]; 
        if ( c.empty() || ( c.back()._x + c.back()._w <
         _boundaryRight - module(moduleID).width() - _cLegal.m_globalLocations[moduleID].x ) ) {
            c.emplace_back( _clusters[rowIdx].size() );
            c.back()._x = _boundaryRight - module(moduleID).width() - _cLegal.m_globalLocations[moduleID].x;
            addCell_reverse(c.back(), moduleID);
        }
        else {
            addCell_reverse(c.back(), moduleID);
            collapseRow_reverse(rowIdx);
        }
    }
    void placeRow( unsigned rowIdx, unsigned moduleID ) {
        vector<Cluster>& c = _clusters[rowIdx]; 
        if ( c.empty() || ( c.back()._x + c.back()._w < _cLegal.m_globalLocations[moduleID].x ) ) {
            c.emplace_back( _clusters[rowIdx].size() );
            c.back()._x = _cLegal.m_globalLocations[moduleID].x;
            addCell(c.back(), moduleID);
        }
        else {
            addCell(c.back(), moduleID);
            collapseRow(rowIdx);
        }
    }
    void addCell( Cluster& c, unsigned moduleID) {
        c._moduleIDs.push_back( moduleID );
        c._e += 1;
        c._q += _cLegal.m_globalLocations[moduleID].x - c._w;
        c._w += module( moduleID ).width();
    }
    void addCell_reverse( Cluster& c, unsigned moduleID ) {
        // cout << " addCell_reverse" << endl;
        c._moduleIDs.push_back( moduleID );
        c._e += 1;
        c._q += _boundaryRight - module(moduleID).width() - _cLegal.m_globalLocations[moduleID].x - c._w;
        c._w += module( moduleID ).width();
        double w = 0;
        for(auto&& i: c._moduleIDs)
            w+=module(i).width();
        assert(w-c._w<0.0001);
    }
    void addCluster( Cluster& c1, Cluster& c2 ) {
        for( unsigned moduleID: c2._moduleIDs )
            c1._moduleIDs.push_back( moduleID );
        c1._e += c2._e;
        c1._q += c2._q - c2._e*c1._w;
        c1._w += c2._w;
    }
    void addCluster_reverse( Cluster& c1, Cluster& c2 ) {
        for( unsigned moduleID: c2._moduleIDs )
            c1._moduleIDs.push_back( moduleID );
        c1._e += c2._e;
        c1._q += c2._q - c2._e*c1._w;
        c1._w += c2._w;
    }
    void collapseRow( unsigned rowIdx ) {

        Cluster& c = _clusters[rowIdx].back();
        c._x = c._q/c._e;
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
    void collapseRow_reverse( unsigned rowIdx ) {
        Cluster& c = _clusters[rowIdx].back();
        assert(c._e - c._moduleIDs.size() < 0.00001);
        double q = 0;
        double w = 0;
        for (auto&& idx : c._moduleIDs) {
            q += _boundaryRight - module(idx).width() - _cLegal.m_globalLocations[idx].x - w;
            w += module(idx).width();
        }
        assert(c._q - q < 0.0000001);
        c._x = c._q/c._e;
        if ( c._x < 0 )
            c._x = 0;
        else if (c._x > _placement.boundaryRight() - _placement.boundaryLeft() - c._w )
            c._x =  _placement.boundaryRight() - _placement.boundaryLeft() - c._w;
        if ( c._ID != 0 ) {
            Cluster& pre = _clusters[ rowIdx ][ c._ID - 1];
            if ( pre._x + pre._w <= c._x ) return;
            addCluster_reverse( pre , c );
            _clusters[ rowIdx ].pop_back();
            collapseRow_reverse( rowIdx );
        }
    }
    void saveRow( unsigned rowIdx ) {
        _save_clusters = _clusters[rowIdx];
    }
    void restoreRow( unsigned rowIdx ) {
        unsigned nSave = _save_clusters.size(); 
        unsigned nC = _clusters[rowIdx].size(); 
        if ( nSave == 0 )
            _clusters[rowIdx].clear();
        else if ( nSave > nC ) {
            _clusters[rowIdx].pop_back();
            for (unsigned i = nC-1; i < nSave; ++i) {
                _clusters[rowIdx].push_back( _save_clusters[i] );
            }
        }
        else if ( nSave == nC ) {
            _clusters[rowIdx].back() = _save_clusters.back();
        }
        else if ( nSave < nC ) {
            _clusters[rowIdx].resize(nSave);
        }
        else
        _clusters[rowIdx] = _save_clusters;
    }
    double deltarowCost( unsigned rowIdx ) {
        unsigned nC = _clusters[rowIdx].size();
        unsigned nSave = _save_clusters.size();
        unsigned n = min( nC, nSave );
        double y = row(rowIdx).y();
        double originCost = 0;
        double newCost = 0;
        double newInsertCost = 0;
        double alpha = 1;
        for (unsigned i = n-1; i < nC; ++i) {
            Cluster&c = _clusters[rowIdx][i];
            double x = c._x;
            for(unsigned moduleID: c._moduleIDs)
            {
                newInsertCost = 0;
                newInsertCost += pow(x - _cLegal.m_globalLocations[moduleID].x ,2);
                newInsertCost += pow(y - _cLegal.m_globalLocations[moduleID].y ,2);
                newCost += newInsertCost;
                x += module(moduleID).width();
            }
        }
        for (unsigned i = n-1; i < nSave; ++i) {
            Cluster&c = _save_clusters[i];
            double x = c._x;
            for(unsigned moduleID: c._moduleIDs)
            {
                originCost += pow(x - _cLegal.m_globalLocations[moduleID].x ,2);
                originCost += pow(y - _cLegal.m_globalLocations[moduleID].y ,2);
                x += module(moduleID).width();
            }
        }
        return newCost - originCost + alpha * newInsertCost;
    }
    double deltarowCost_reverse( unsigned rowIdx ) {
        unsigned nC = _clusters[rowIdx].size();
        unsigned nSave = _save_clusters.size();
        unsigned n = min( nC, nSave );
        double y = row(rowIdx).y();
        double originCost = 0;
        double newCost = 0;
        double newInsertCost = 0;
        double alpha = 1;
        for (unsigned i = n-1; i < nC; ++i) {
            Cluster&c = _clusters[rowIdx][i];
            double x = _boundaryRight - c._x;
            for(unsigned moduleID: c._moduleIDs)
            {
                x -= module(moduleID).width();
                newInsertCost = 0;
                newInsertCost += pow(x - _cLegal.m_globalLocations[moduleID].x,2);
                newInsertCost += pow(y - _cLegal.m_globalLocations[moduleID].y,2);
                newCost += newInsertCost;
            }
        }
        for (unsigned i = n-1; i < nSave; ++i) {
            Cluster&c = _save_clusters[i];
            double x = _boundaryRight - c._x;
            for(unsigned moduleID: c._moduleIDs)
            {
                x -= module(moduleID).width();
                originCost += pow(x - _cLegal.m_globalLocations[moduleID].x,2);
                originCost += pow(y - _cLegal.m_globalLocations[moduleID].y,2);
            }
        }
        return newCost - originCost + alpha * newInsertCost;
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
                }
            }        
        }
    }

    void setBestPosition_reverse() {
        for (unsigned rowIdx = 0; rowIdx < _clusters.size(); ++rowIdx) {
            for(Cluster& c: _clusters[rowIdx]) {
                double x = _boundaryRight - c._x;
                double y = row(rowIdx).y();            
                for(unsigned& moduleID: c._moduleIDs) {
                    x -= module(moduleID).width();
                    _cLegal.m_bestLocations[moduleID].x = x;
                    _cLegal.m_bestLocations[moduleID].y = y;
                }
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
    vector<vector<Cluster>>     _clusters;
    vector<vector<Cluster>>     _forward_clusters_sol;
    double                      _forward_displacement;
    vector<CPoint>              _ordered_sol;
    double                      _boundaryRight;
};

bool CLegal::solve()
{
    saveGlobalResult();
    clock_t t0 = clock();
    Abucus abacus( *this, _placement );
    // GnuplotLivePlotter gp;
    // gp.setTitle("Legalization");
    // gp.setAutoFit(1);
    // for (unsigned i = 0; i < _placement.numModules(); i++) {
    //     Module &module = _placement.module( i );
    //     gp.addRectangle( module.rectangle() );
    // }
    // gp.addRectangle( Rectangle(_placement.boundaryLeft(),
    //     _placement.boundaryBottom(),
    //     _placement.boundaryRight(),
    //     _placement.boundaryTop()) );

    // gp.show();
    // cin.get();
    return true;
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
        if(module.isFixed()) continue;
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
        if( module.isFixed() ) continue;

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
            if(mod.isFixed()) continue;
            size_t nextId = j+1;
            while( mod.x() + mod.width() - modules[ nextId ]->x() > 0.01 ){
                Module &modNext = *modules[ nextId ];
                if( mod.x() + mod.width() - modules[ nextId ]->x() > 0.01 ){
                    ++overLap;
                    cout << mod.name() << " overlap with " << modNext.name() << endl;
                }
                ++nextId; if( nextId == modules.size() ) { break; }
            }
        }
    }

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
