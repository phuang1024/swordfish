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
    bool beta_cutoff = false;

    // End of game.
    if (legal_moves.size() == 0) {
        r_eval = static_eval;
        return;
    }

    // Start quie search.
    if (!is_quiesce && remain_depth <= 0) {
        unified_search(
                pos, maxdepth, mydepth + 1,
                alpha, beta,
                false, true,
                r_eval, r_bestmove);
        return;
    }

    // Only used in quiesce.
    ull t_pieces;
    if (is_quiesce) {
        t_pieces = pos.relative_bb(pos.turn).t_pieces;
    }

    // Start at static eval in case no captures for quie.
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
        if (curr_eval >= beta) {
            beta_cutoff = true;
            break;
        }
        if (curr_eval > alpha) {
            alpha = curr_eval;
            best_move = move;
        }
    }

    // Exit procedures.
    if (is_root) {
        r_bestmove = best_move;
    }
    r_eval = beta_cutoff ? beta : alpha;
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


}
