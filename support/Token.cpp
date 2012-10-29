#include <stdexcept>

#include "Token.h"

namespace Game 
{
/*

#include <iostream>

void
Token_set::print(void) const
{
    std::map<IntCoord, char>::const_iterator token = tokens.begin();
    for ( ; token != tokens.end(); ++token) {
        std::cerr << "(" << token->first.get_row() << ", " << token->first.get_col() << ")" << std::endl;
    }
    std::cerr << "-------------" << std::endl;
}*/

void
Token_set::add(const Token &token)
{
    tokens[token.coord] = token.player;
}

void
Token_set::remove(const Token &token)
{
    tokens.erase(token.coord);
}

void
Token_set::remove(const IntCoord &coord)
{
    tokens.erase(coord);
}

void
Token_set::change(const Token &token, const IntCoord &new_coord)
{
    tokens.erase(token.coord);
    tokens[new_coord] = token.player;
}

int
Token_set::size(void) const
{
    return tokens.size();
}

bool
Token_set::empty(void) const
{
    return tokens.empty();
}

std::pair<std::map<IntCoord, char>, std::map<IntCoord, char> > 
Token_set::split(void) const
{
    std::map<IntCoord, char>::const_iterator token = tokens.begin();
    std::pair<std::map<IntCoord, char>, std::map<IntCoord, char> > sets;
    for ( ; token != tokens.end(); ++token) {
        if (token->second == Token::FIRST) {
            //sets.first.add(Token(token->first, token->second));
            sets.first[token->first] = token->second;
        } else {
            //sets.second.add(Token(token->first, token->second));
            sets.second[token->first] = token->second;
        }
    }
    
    return sets;
}

std::map<IntCoord, char>
Token_set::get_first(void) const
{
    std::map<IntCoord, char> set;
    std::map<IntCoord, char>::const_iterator token = tokens.begin();
    for ( ; token != tokens.end(); ++token) {
        if (token->second == Token::FIRST) {
            set[token->first] = token->second;
        }
    }
    
    return set;
}

std::map<IntCoord, char>
Token_set::get_second(void) const
{
    std::map<IntCoord, char> set;
    std::map<IntCoord, char>::const_iterator token = tokens.begin();
    for ( ; token != tokens.end(); ++token) {
        if (token->second == Token::SECOND) {
            set[token->first] = token->second;
        }
    }
    
    return set;
}

std::pair<int, int>
Token_set::size_pair(void) const
{
  std::pair<int, int> sizes;
  std::map<IntCoord, char>::const_iterator token = tokens.begin();
  for ( ; token != tokens.end(); ++token) {
      if (token->second == Token::SECOND) {
        ++sizes.second;
      } else {
        ++sizes.first;
      }
  }
  
  return sizes;
}

const char &
Token_set::operator [](const IntCoord &coord) const
{
    std::map<IntCoord, char>::const_iterator elem = tokens.find(coord);
    if (elem == tokens.end()) {
        throw std::range_error("Invalid coordinates.");
    }
    return elem->second;
}

char &
Token_set::operator [](const IntCoord &coord)
{
    return tokens[coord];
}

} // end of Game namespace
