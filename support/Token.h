#pragma once
#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <map>

#include "Coord.h"

namespace Game {

//typedef std::pair<IntCoord, char> Token;
struct Token
{
    IntCoord coord;
    char player;
    Token(const IntCoord &coord_ = IntCoord(), char player_ = char()) :
        coord(coord_), player(player_) {}
    
    enum
    {
        A = 0,
        B = 1,
        FIRST = 0,
        SECOND = 1
    };
};

class Token_set
{
    std::map<IntCoord, char> tokens;
public:
    Token_set() : tokens() {}
    void add(const Token &token);
    void remove(const Token &token);
    void remove(const IntCoord &coord);
    void change(const Token &token, const IntCoord &new_coord);
    
    int size(void) const;
    std::pair<int, int> size_pair(void) const;
    bool empty(void) const;
    std::pair<std::map<IntCoord, char>, std::map<IntCoord, char> > 
        split(void) const;
    std::map<IntCoord, char> get_first(void) const;
    std::map<IntCoord, char> get_second(void) const;
    
    const char &operator [](const IntCoord &coord) const;
    char &operator [](const IntCoord &coord);
    
    //void print(void) const;
};


}

#endif
