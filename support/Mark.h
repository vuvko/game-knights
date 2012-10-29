#pragma once
#ifndef _MARK_H_
#define _MARK_H_

#include <map>

#include "Coord.h"

namespace Game {

//typedef std::pair<IntCoord, char> Mark;
struct Mark
{
    IntCoord coord;
    char value;
    
    Mark(const IntCoord &coord_, char value_) :
        coord(coord_), value(value_) {}
    
    enum 
    {
        MARK1 = '1',
        MARK2 = '2'
    };
};

}

#endif
