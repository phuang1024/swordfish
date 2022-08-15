#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"


namespace Search {


/**
 * From https://www.chessprogramming.org/Quiescence_Search
 */
static int quiesce_search(Position& pos, int alpha, int beta, int my_depth, int& r_maxdepth, ull& r_nodes) {
    if (my_depth > r_maxdepth)
        r_maxdepth = my_depth;

    int stand_pat = Eval::eval(pos) * (pos.turn ? 1 : -1);
    if (stand_pat >= beta)
        return beta;
    if (alpha < stand_pat)
        alpha = stand_pat;

    const ull t_pieces = pos.relative_bb(pos.turn).t_pieces;

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    for (const Move& move: moves) {
        if (!Bit::get(t_pieces, move.to))
            continue;

        r_nodes++;

        Position new_pos = pos;
        new_pos.push(move);

        const int score = -quiesce_search(new_pos, -beta, -alpha, my_depth+1, r_maxdepth, r_nodes);
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }

    return alpha;
}


/**
 * Returns best score.
 */
static int score_search(Position& pos, int depth, int alpha, int beta, int& r_maxdepth, ull& r_nodes) {
    if (depth == 0) {
        const int score = quiesce_search(pos, alpha, beta, 0, r_maxdepth, r_nodes);
        return score;
    }

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);
    r_nodes += moves.size();

    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);

        const int score = -score_search(new_pos, depth-1, -beta, -alpha, r_maxdepth, r_nodes);
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }

    return alpha;
}


SearchResult search(Position& pos, int depth) {
    const ull time_start = Time::time();

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    Move best_move(0, 0);
    int best_score = -1e9;
    ull nodes = 0;
    int max_quie_depth = 0;
    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);

        const int score = -score_search(new_pos, depth, -1e9, 1e9, max_quie_depth, nodes);
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    const ull elapse = Time::elapse(time_start);
    SearchResult res;
    res.data["depth"] = std::to_string(depth);
    res.data["seldepth"] = std::to_string(depth+max_quie_depth);
    res.data["pv"] = best_move.uci();
    res.data["score cp"] = std::to_string(best_score);
    res.data["nodes"] = std::to_string(nodes);
    res.data["nps"] = std::to_string(Time::nps(nodes, elapse));
    res.data["time"] = std::to_string(elapse);
    return res;
}


}
