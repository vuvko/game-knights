#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <climits>
#include <ctime>

#include "support/Coord.h"
#include "support/Matrix.h"
#include "support/Token.h"
#include "support/Mark.h"

namespace Game {

class Game_rules
{    
    Matrix<char> field;
    Token_set tokens;
    std::pair<int, int> score;
    std::pair<double, double> time_left;
    int turn;
    char player, player_letter, winner;
    bool was_ended;
    
    bool check_position(int y, int x) const;
    bool check_position(const IntCoord &pos) const;
    bool check_move(const Token &token, char move) const;
    void update_field(const IntCoord &old_pos, const IntCoord &new_pos);
    void change_player(void);
    bool end_game(void);
public:
    Game_rules() : field(ROW_NUM, COL_NUM, '-'), tokens(), score(), 
        time_left(), turn(0), player(), player_letter(), winner(),
        was_ended() {}
    Game_rules(const char *filename);
    
    void make_move(const Token &token, char move);
    void change_time(int time);
    int estimate(void) const;
    char get_player(void) const;
    std::string get_field_string(void) const; 
    
    std::vector<std::pair<Token, char> > generate_moves(void) const;
    
    friend std::ostream &operator << (std::ostream &s, const Game_rules &game);
    //friend std::istream &operator >> (std::istream &s, const Game_rules &game);
    
    friend std::ofstream &operator << (std::ofstream &f, const Game_rules &game);
    //friend std::ifstream &operator >> (std::ifstream &f, const Game_rules &game);
    
    enum
    {
        FIRST_LETTER = 'A',
        SECOND_LETTER = 'B',
        DRAW_LETTER = 'D',
        UNKNOW_LETTER = 'U',
        SEPARATOR = ' ',
        SPACE_LETTER = '-',
        
        ROW_NUM = 8,
        COL_NUM = 8,
        
        UP_LEFT = 0,
        UP_RIGHT = 1,
        RIGHT_UP = 2,
        RIGHT_DOWN = 3,
        DOWN_RIGHT = 4,
        DOWN_LEFT = 5,
        LEFT_DOWN = 6,
        LEFT_UP = 7,
        
        WIN_NUM = 10000
    };
};

}

#endif
