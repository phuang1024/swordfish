#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"

using Transposition::TP;
using Transposition::TPTable;


namespace Search {


/**
 * Alpha beta negamax search that can act like:
 * * Root node: Sets best move as well.
 * * Normal search: Normal.
 * * Quiescence search: Evaluates when position is quiet.
 *
 * Algorithms implemented using pseudocode from https://chessprogramming.org
 *
 * @param pos  Position to search on.
 * @param maxdepth  Max depth of normal search (quiesce if exceeds).
 * @param mydepth  Depth of this node.
 * @param r_eval  Eval of this node relative to position's turn.
 * @param r_bestmove  Best move. Only set if root search.
 */
static void unified_search(
        Position& pos, int maxdepth, int mydepth,
        int alpha, int beta,
        bool is_root, bool is_quiesce,
        int& r_eval, Move& r_bestmove)
{
    std::vector<Move> legal_moves;
    Movegen::get_legal_moves(pos, legal_moves);
    const int remain_depth = maxdepth - mydepth;
    const int static_eval = Eval::eval_rel(pos, legal_moves.size());

    Move best_move(0, 0);

    // End of game.
    if (legal_moves.size() == 0)
        goto exit;

    // Quiesce search is started in exit.
    if (!is_quiesce && remain_depth <= 0)
        goto exit;

    // Only used in quiesce.
    ull t_pieces;
    if (is_quiesce) {
        t_pieces = pos.relative_bb(pos.turn).t_pieces;
    }

    // If no captures, return static eval.
    if (is_quiesce)
        alpha = std::max(alpha, static_eval);

    for (int i = legal_moves.size() - 1; i >= 0; i--) {
        const Move& move = legal_moves[i];

        // Check if quiesce and capture move.
        if (is_quiesce && !Bit::get(t_pieces, move.to))
            continue;

        Position new_pos = pos;
        new_pos.push(move);

        // Get eval of new position.
        int curr_eval;
        unified_search(
                new_pos, maxdepth, mydepth + 1,
                -beta, -alpha,
                false, is_quiesce,
                curr_eval, r_bestmove);
        curr_eval = -curr_eval;

        // Check alpha beta.
        if (curr_eval >= beta)
            goto exit;
        if (curr_eval > alpha) {
            alpha = curr_eval;
            best_move = move;
        }
    }

exit:
    // Common exit procedures.
    if (is_root) {
        r_bestmove = best_move;
    }
    if (legal_moves.size() == 0) {
        r_eval = static_eval;
    } else {
        if (is_quiesce) {
            r_eval = alpha;
        } else {
            unified_search(
                    pos, maxdepth, mydepth + 1,
                    alpha, beta,
                    false, true,
                    r_eval, r_bestmove);
        }
    }
}


Move search(Position& pos, int maxdepth) {
    Transposition::init();
    TPTable tptable;

    Move best_move(0, 0);
    int best_eval = 0;
    for (int depth = 1; depth <= maxdepth; depth++) {
        const ull time_start = Time::time();

        /*
        // Aspiration window.
        int curr_bestscore;   // Best score for this depth.
        int lower = 10, upper = 10;
        while (true) {
            int alpha = r_bestscore - lower, beta = r_bestscore + upper;
            root_search(tptable, pos, depth, 0, alpha, beta, r_bestmove, curr_bestscore);
            if (curr_bestscore <= alpha)
                lower *= 2;
            else if (curr_bestscore >= beta)
                upper *= 2;
            else
                break;
        }
        r_bestscore = curr_bestscore;
        */
        unified_search(
                pos, depth, 0,
                -1e9, 1e9,
                true, false,
                best_eval, best_move);

        const ull elapse = Time::elapse(time_start);
        SearchResult res;
        res.data["depth"] = std::to_string(depth);
        //res.data["seldepth"] = std::to_string(depth+max_quie_depth);
        res.data["pv"] = best_move.uci();
        res.data["score cp"] = std::to_string(best_eval);
        //res.data["nodes"] = std::to_string(nodes);
        //res.data["nps"] = std::to_string(Time::nps(nodes, elapse));
        res.data["time"] = std::to_string(elapse);
        std::cout << res.uci() << std::endl;
    }

    return best_move;
}


/**
 * From https://www.chessprogramming.org/Quiescence_Search
 */
static int quiesce_search(Position& pos, int alpha, int beta) {
    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    // Return static eval if EOG.
    int stand_pat = Eval::eval(pos, moves.size()) * (pos.turn ? 1 : -1);
    if (moves.size() == 0)
        return stand_pat;
    if (stand_pat >= beta)
        return beta;
    if (stand_pat > alpha)
        alpha = stand_pat;

    const ull t_pieces = pos.relative_bb(pos.turn).t_pieces;

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
static int score_search(TPTable& tptable, Position& pos, int maxdepth, int mydepth,
        int alpha, int beta) {
    const int remain_depth = maxdepth - mydepth;

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    // Return static eval if EOG.
    int stand_pat = Eval::eval(pos, moves.size()) * (pos.turn ? 1 : -1);
    if (moves.size() == 0)
        return stand_pat;

    const ull hash = Transposition::hash(pos);
    TP* tp = tptable.get(hash);
    bool tp_equal = tp->depth != -1 && pos == tp->pos;

    // Check if position has been searched before.
    if (tp_equal && tp->depth >= remain_depth)
        return tp->score;

    // Quiescence search if depth remaining is 0.
    if (remain_depth <= 0) {
        const int score = quiesce_search(pos, alpha, beta);
        return score;
    }

    /* Move ordering
    if (tp_equal) {
        moves.push_back(tp->bestmove);
    }*/

    // Alpha beta search.
    Move bestmove(0, 0);
    for (int i = moves.size() - 1; i >= 0; i--) {
        const Move& move = moves[i];
        Position new_pos = pos;
        new_pos.push(move);

        const int score = -score_search(tptable, new_pos, maxdepth, mydepth+1, -beta, -alpha);
        if (score >= beta)
            return beta;
        if (score > alpha) {
            bestmove = move;
            alpha = score;
        }
    }

    // Write to transposition table.
    if (bestmove.from != 0 && bestmove.to != 0 && remain_depth > tp->depth) {
        tp->pos = pos;
        tp->depth = mydepth;
        tp->score = alpha;
        tp->bestmove = bestmove;
    }
    return alpha;
}


static void root_search(TPTable& tptable, Position& pos, int maxdepth, int mydepth,
        int alpha, int beta,
        Move& r_bestmove, int& r_bestscore) {
    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);

        const int score = -score_search(tptable, new_pos, maxdepth, mydepth+1, -beta, -alpha);
        if (score >= beta) {
            r_bestscore = beta;
            r_bestmove = move;
            return;
        }
        if (score > alpha) {
            alpha = score;
            r_bestmove = move;
        }
    }

    r_bestscore = alpha;
}


}
