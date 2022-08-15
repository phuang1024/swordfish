#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"


namespace Search {


/**
 * Returns best score.
 */
static int score_search(Position& pos, int depth, int alpha, int beta, ull& r_nodes) {
    if (depth == 0) {
        const int score = Eval::eval(pos);
        return pos.turn ? score : -score;
    }

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);
    r_nodes += moves.size();

    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);

        const int score = -score_search(new_pos, depth-1, -beta, -alpha, r_nodes);
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }

    return alpha;
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

        const int score = -score_search(new_pos, depth, -1e9, 1e9, nodes);
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
