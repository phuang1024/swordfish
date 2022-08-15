#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"

using Transposition::TP;
using Transposition::TPTable;


namespace Search {


/**
 * From https://www.chessprogramming.org/Quiescence_Search
 */
static int quiesce_search(Position& pos, int alpha, int beta) {
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

        Position new_pos = pos;
        new_pos.push(move);

        const int score = -quiesce_search(new_pos, -beta, -alpha);
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
static int score_search(TPTable& tptable, Position& pos, int depth, int alpha, int beta) {
    const ull hash = Transposition::hash(pos);
    TP* tp = tptable.get(hash);
    if (tp->depth >= depth) {
        if (pos == tp->pos)
            return tp->score;
    }

    if (depth == 0) {
        const int score = quiesce_search(pos, alpha, beta);
        return score;
    }

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);

        const int score = -score_search(tptable, new_pos, depth-1, -beta, -alpha);
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }

    if (depth > tp->depth) {
        tp->pos = pos;
        tp->depth = depth;
        tp->score = alpha;
    }
    return alpha;
}


static void root_search(TPTable& tptable, Position& pos, int depth,
        Move& r_bestmove, int& r_bestscore) {
    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);

        const int score = -score_search(tptable, new_pos, depth, -1e9, 1e9);
        if (score > r_bestscore) {
            r_bestscore = score;
            r_bestmove = move;
        }
    }
}


Move search(Position& pos, int maxdepth) {
    Transposition::init();
    TPTable tptable;

    Move r_bestmove(0, 0);
    int r_bestscore;
    for (int depth = 1; depth <= maxdepth; depth++) {
        const ull time_start = Time::time();
        r_bestscore = -1e9;
        root_search(tptable, pos, depth, r_bestmove, r_bestscore);
        const ull elapse = Time::elapse(time_start);

        SearchResult res;
        res.data["depth"] = std::to_string(depth);
        //res.data["seldepth"] = std::to_string(depth+max_quie_depth);
        res.data["pv"] = r_bestmove.uci();
        res.data["score cp"] = std::to_string(r_bestscore);
        //res.data["nodes"] = std::to_string(nodes);
        //res.data["nps"] = std::to_string(Time::nps(nodes, elapse));
        res.data["time"] = std::to_string(elapse);
        std::cout << res.uci() << std::endl;
    }

    return r_bestmove;
}


}
