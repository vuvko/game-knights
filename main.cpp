#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <ctime>

#include "./support/Token.h"
#include "Game.h"

using namespace std;
using namespace Game;

enum
{
    MIN_PLAYER = 0,
    MAX_PLAYER = 1,
  
    DEPTH_MAX = 5
};

int alpha_beta(
    Game_rules &game,
    int depth = DEPTH_MAX, 
    int alpha = INT_MIN, 
    int beta = INT_MAX);

const char *input = "matrix.txt";
const char *output = "matrix.txt";
  
int
alpha_beta(Game_rules &game, int depth, int alpha, int beta)
{
    if (depth == 0) {
        return game.estimate();
    }
    vector<pair<Token, char> > moves = game.generate_moves();
    if (!moves.size()) {
        return game.estimate();
    }
    Game_rules backup = game;
    int best_move = 0;
    int best = 0;
  
    if (game.get_player() == MAX_PLAYER) {
        best = alpha;
        for (unsigned i = 0; i < moves.size(); ++i) {
            game.make_move(moves[i].first, moves[i].second);
            int tmp = alpha_beta(game, depth - 1, best, beta);
            if (tmp > best) {
                best = tmp;
                best_move = i;
            }
            game = backup;
            if (beta <= best) {
                break;
            }
        }
    } else {
        best = beta;
        for (unsigned i = 0; i < moves.size(); ++i) {
            game.make_move(moves[i].first, moves[i].second);
            int tmp = alpha_beta(game, depth - 1, alpha, best);
            if (tmp < best) {
                best = tmp;
                best_move = i;
            }
            game = backup;
            if (best <= alpha) {
                break;
            }
        }
    }
    if (depth == DEPTH_MAX) {
        game.make_move(moves[best_move].first, moves[best_move].second);
    }
    return best;
}

int
main(void)
{
    Game_rules knights(input);
    
    int start = clock();
         
    alpha_beta(knights);
    
    int stop = clock();
    knights.change_time(stop - start);
    
    ofstream out(output);
    out << knights;
    out.close();
    
    return 0;
}
