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