#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"


namespace Search {


/**
 * Returns best score.
 */
static int score_search(Position& pos, int depth) {
    if (depth == 0) {
        const int score = Eval::eval(pos);
        return pos.turn ? score : -score;
    }

    int max_score = -1e9;

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);
    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);
        const int score = -score_search(new_pos, depth-1);
        if (score > max_score)
            max_score = score;
    }

    return max_score;
}


SearchResult search(Position& pos, int depth) {
    SearchResult res;

    const int score = score_search(pos, depth);
    res.data["cp"] = std::to_string(score);
    res.data["depth"] = std::to_string(depth);
    return res;

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
