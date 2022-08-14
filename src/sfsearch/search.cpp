#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"


namespace Search {


SearchResult search(Position& pos, int depth, bool ret_move) {
    SearchResult res;

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    Move best_move;
    int best_score = -1e9;
    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);

        const int score = Eval::eval(new_pos);
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    res.data["pv"] = best_move.uci();
    return res;
}


}
