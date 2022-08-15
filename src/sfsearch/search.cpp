#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"


namespace Search {


/**
 * Returns best score.
 */
static int score_search(Position& pos, int depth, ull& r_nodes) {
    if (depth == 0) {
        const int score = Eval::eval(pos);
        return pos.turn ? score : -score;
    }

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);
    r_nodes += moves.size();

    int max_score = -1e9;
    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);
        const int score = -score_search(new_pos, depth-1, r_nodes);
        if (score > max_score)
            max_score = score;
    }

    return max_score;
}


SearchResult search(Position& pos, int depth) {
    const ull time_start = Time::time();

    SearchResult res;
    res.data["depth"] = std::to_string(depth);

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    Move best_move;
    int best_score = -1e9;
    ull nodes = 0;
    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);

        const int score = -score_search(new_pos, depth, nodes);
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    const ull elapse = Time::elapse(time_start);
    res.data["pv"] = best_move.uci();
    res.data["cp"] = std::to_string(best_score);
    res.data["nodes"] = std::to_string(nodes);
    res.data["nps"] = std::to_string(Time::nps(nodes, elapse));
    res.data["time"] = std::to_string(elapse);
    return res;
}


}
