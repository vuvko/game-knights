#include <stdexcept>
#include <iostream>
#include <fstream>

#include "Game.h"

namespace Game
{

Game_rules::Game_rules(const char *filename) : field(ROW_NUM, COL_NUM, '-'),
    tokens(), score(), time_left(), turn(0), player(), player_letter(), 
    winner(), was_ended()
{
    if (!filename) {
        throw std::invalid_argument("Empty filename.");
    }
    std::ifstream in(filename);
    if (in.fail()) {
        throw std::runtime_error("Can't open input file.");
    }
    if (!(in >> player_letter >> turn >> winner >> score.first >> 
        time_left.first >> score.second >> time_left.second)) {
        throw std::runtime_error("Can't read data from input file.");
    }
    if (player_letter == FIRST_LETTER) {
        player = Token::A;
    } else {
        player = Token::B;
    }
    for (int i = 0; i < ROW_NUM; ++i) {
        for (int j = 0; j < COL_NUM; ++j) {
            if (!(in >> field.at(i, j))) {
                throw std::runtime_error("Can't read game field from file.");
            }
            switch (field.at(i, j)) {
            case FIRST_LETTER:
                tokens.add(Token(IntCoord(i, j), Token::FIRST));
                break;
            case SECOND_LETTER:
                tokens.add(Token(IntCoord(i, j), Token::SECOND));
                break;
            default:
                break;
            }
        }
    }
    
    if (winner == FIRST_LETTER || winner == SECOND_LETTER || 
        winner == DRAW_LETTER) {
        was_ended = true;
    } else {
        was_ended = false;
    }
    
    in.close();
}

std::ostream &
operator << (std::ostream &s, const Game_rules &game)
{
    s << char(game.player_letter) << char(Game_rules::SEPARATOR);
    s << game.turn << char(Game_rules::SEPARATOR) << game.winner << std::endl <<
        game.score.first << char(Game_rules::SEPARATOR) << 
        game.time_left.first << std::endl <<
        game.score.second << char(Game_rules::SEPARATOR) << 
        game.time_left.second << std::endl;
    for (int i = 0; i < Game_rules::ROW_NUM; ++i) {
        for (int j = 0; j < Game_rules::COL_NUM; ++j) {
            s << char(game.field.at(i, j));
        }
        s << std::endl;
    }
    s << std::endl;
    
    return s;
}

std::ofstream &
operator << (std::ofstream &f, const Game_rules &game)
{
    f << char(game.player_letter) << char(Game_rules::SEPARATOR);
    f << game.turn << char(Game_rules::SEPARATOR) << game.winner << std::endl <<
        game.score.first << char(Game_rules::SEPARATOR) << 
        game.time_left.first << std::endl <<
        game.score.second << char(Game_rules::SEPARATOR) << 
        game.time_left.second << std::endl;
    for (int i = 0; i < Game_rules::ROW_NUM; ++i) {
        for (int j = 0; j < Game_rules::COL_NUM; ++j) {
            f << char(game.field.at(i, j));
        }
        f << std::endl;
    }
    f << std::endl;
    
    return f;
}

void
Game_rules::make_move(const Token &token, char move)
{
    if (time_left.first <= 0 || time_left.second <= 0 ||
        winner == FIRST_LETTER || winner == SECOND_LETTER || 
        winner == DRAW_LETTER || tokens.size() == 0) {
        return;
    }
    if (token.player != player) {
        return;
    }
    int x = token.coord.get_col();
    int y = token.coord.get_row();
    IntCoord old_pos(y, x);
    IntCoord cur_pos(y, x);
    switch (move) {
    case UP_LEFT:
        x -= 1;
        y -= 2;
        break;
    case UP_RIGHT:
        x += 1;
        y -= 2;
        break;
    case RIGHT_UP:
        x += 2;
        y -= 1;
        break;
    case RIGHT_DOWN:
        x += 2;
        y += 1;
        break;
    case DOWN_RIGHT:
        x += 1;
        y += 2;
        break;
    case DOWN_LEFT:
        x -= 1;
        y += 2;
        break;
    case LEFT_DOWN:
        x -= 2;
        y += 1;
        break;
    case LEFT_UP:
        x -= 2;
        y -= 1;
        break;
    default:
        std::runtime_error("Unknow move.");
        break;
    }
    cur_pos = IntCoord(y, x);
    if (check_position(cur_pos)) {
        update_field(old_pos, cur_pos);
        tokens.change(token, cur_pos);
    } else {
        throw std::runtime_error("Illegal move.");
    }
    
    ++turn;
    change_player();
    
    if (end_game()) { // check field for game's end
        if (score.first > score.second) {
            winner = FIRST_LETTER;
        } else if (score.first == score.second) {
            winner = DRAW_LETTER;
        } else {
            winner = SECOND_LETTER;
        }
    }
}

bool
Game_rules::check_position(int y, int x) const
{
    return check_position(IntCoord(y, x));
}

bool
Game_rules::check_position(const IntCoord &pos) const
{
    IntCoord lt(0, 0);
    IntCoord rt(0, COL_NUM);
    IntCoord lb(ROW_NUM, 0);
    IntCoord rb(ROW_NUM, COL_NUM);
    
    return pos.get_col() >= 0 && pos.get_col() < COL_NUM &&
        pos.get_row() >= 0 && pos.get_row() < ROW_NUM &&
        (player != Token::FIRST || field.at(pos) != FIRST_LETTER) &&
        (player != Token::SECOND || field.at(pos) != SECOND_LETTER);
}

void
Game_rules::update_field(const IntCoord &old_pos, const IntCoord &new_pos)
{
    field.at(old_pos) = SPACE_LETTER;
    switch (field.at(new_pos)) {
    case FIRST_LETTER:
        ++score.second;
        tokens.remove(new_pos); // delete token
        break;
    case SECOND_LETTER:
        ++score.first;
        tokens.remove(new_pos); // delete token
        break;
    case Mark::MARK1:
        if (player == Token::FIRST) {
            score.first += 1;
        } else {
            score.second += 1;
        }
        break;
    case Mark::MARK2:
        if (player == Token::FIRST) {
            score.first += 2;
        } else {
            score.second += 2;
        }
        break;
    default:
        break;
    }
    
    field.at(new_pos) = player_letter;
}

bool
Game_rules::end_game(void)
{
    if (time_left.first <= 0) {
        winner = SECOND_LETTER;
        return true;
    }
    if (time_left.second <= 0) {
        winner = FIRST_LETTER;
        return true;
    }
    std::pair<std::map<IntCoord, char>, std::map<IntCoord, char> > 
        sets = tokens.split();
    if (sets.first.empty()) {
        winner = SECOND_LETTER;
        return true;
    }
    if (sets.second.empty()) {
        winner = FIRST_LETTER;
        return true;
    }
    
    bool line_fr = true, line_fc = true;
    bool line_sr = true, line_sc = true;
    std::map<IntCoord, char>::const_iterator token = sets.first.begin();
    int row = token->first.get_row();
    int col = token->first.get_col();
    for (++token; token != sets.first.end() && (line_fr || line_fc); 
      ++token) {
        if (token->first.get_row() != row) {
            line_fr = false;
        }
        if (token->first.get_col() != col) {
            line_fc = false;
        }
    }
    
    token = sets.second.begin();
    row = token->first.get_row();
    col = token->first.get_col();
    for (++token; token != sets.second.end() && (line_sr || line_sc); 
      ++token) {
        if (token->first.get_row() != row) {
            line_sr = false;
        }
        if (token->first.get_col() != col) {
            line_sc = false;
        }
    }
    
    if (line_fr || line_fc) {
        score.first += 3;
    }
    if (line_sr || line_sc) {
        score.second += 3;
    }
    
    return line_fr || line_sr || line_fc || line_sc;
}

void
Game_rules::change_player(void)
{
    if (player == Token::FIRST) {
        player = Token::SECOND;
        player_letter = SECOND_LETTER;
    } else {
        player = Token::FIRST;
        player_letter = FIRST_LETTER;
    }
}

bool
Game_rules::check_move(const Token &token, char move) const
{
    int x = token.coord.get_col();
    int y = token.coord.get_row();
    switch (move) {
    case UP_LEFT:
        x -= 1;
        y -= 2;
        break;
    case UP_RIGHT:
        x += 1;
        y -= 2;
        break;
    case RIGHT_UP:
        x += 2;
        y -= 1;
        break;
    case RIGHT_DOWN:
        x += 2;
        y += 1;
        break;
    case DOWN_RIGHT:
        x += 1;
        y += 2;
        break;
    case DOWN_LEFT:
        x -= 1;
        y += 2;
        break;
    case LEFT_DOWN:
        x -= 2;
        y += 1;
        break;
    case LEFT_UP:
        x -= 2;
        y -= 1;
        break;
    default:
        std::runtime_error("Unknow move.");
        break;
    }
    IntCoord cur_pos(y, x);
    return check_position(cur_pos);
}

std::vector<std::pair<Token, char> >
Game_rules::generate_moves(void) const
{
    std::map<IntCoord, char> set;
    std::vector<std::pair<Token, char> > moves(0);
    if (time_left.first <= 0 || time_left.second <= 0 ||
        winner == FIRST_LETTER || winner == SECOND_LETTER || 
        winner == DRAW_LETTER || tokens.size() == 0) {
        return moves;
    }
    if (player == Token::FIRST) {
        set = tokens.get_first();
    } else {
        set = tokens.get_second();
    }
    
    std::map<IntCoord, char>::const_iterator token = set.begin();
    for ( ; token != set.end(); ++token) {
        for (int i = UP_LEFT; i <= LEFT_UP; ++i) {
            if (check_move(Token(token->first, token->second), i)) {
                moves.push_back(std::pair<Token, char>(Token(token->first, 
                    token->second), i));
            }
        }
    }
    
    return moves;
}

void
Game_rules::change_time(int time)
{
    if (time_left.first <= 0 || time_left.second <= 0 || was_ended) {
        return;
    }
    if (player == Token::SECOND) {
        time_left.first -= double(time) / CLOCKS_PER_SEC;
    } else {
        time_left.second -= double(time) / CLOCKS_PER_SEC;
    }
}

char
Game_rules::get_player(void) const
{
  return player;
}

std::string
Game_rules::get_field_string(void) const
{
    std::string str(ROW_NUM * COL_NUM, '-');
    for (int i = 0; i < ROW_NUM; ++i) {
        for (int j = 0; j < COL_NUM; ++j) {
            str[i * COL_NUM + j] = field.at(i, j);
        }
    }
    
    return str;
}

int
Game_rules::estimate(void) const
{
    if (time_left.first <= 0 || time_left.second <= 0 ||
        winner == FIRST_LETTER || winner == SECOND_LETTER || 
        winner == DRAW_LETTER || tokens.size() == 0) {
        if (winner == FIRST_LETTER || time_left.second <= 0) {
            return -WIN_NUM + turn;
        } else if (winner == SECOND_LETTER || time_left.first <= 0) {
            return WIN_NUM - turn;
        } else if (winner == DRAW_LETTER) {
            return 0;
        }
    }
    if (score.second > score.first) {
        return (score.second - score.first) * 100 - turn;
    }
    return (score.second - score.first) * 100 + turn;
}

} // end of Game namespace
