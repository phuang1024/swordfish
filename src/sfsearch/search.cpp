#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"

using Transposition::TP;
using Transposition::TPTable;


namespace Search {


/**
 * Alpha beta negamax search that can act like:
 * * Root node: Sets both r_eval and r_bestmove.
 * * Normal search: Sets r_eval, ignores r_bestmove.
 * * Quiescence search: Evaluates as soon as position is quiet.
 *
 * Some algorithms implemented using pseudocode from https://chessprogramming.org
 *
 * @param maxdepth  Max depth of normal search (quiesce if exceeds).
 * @param mydepth  Depth of this node.
 * @param r_eval  Eval of this node relative to position's turn.
 * @param r_maxdepth  Max depth of search.
 */
static void unified_search(
        ull time_start, TPTable& tptable, Position& pos, int maxdepth, int mydepth, int movetime,
        int alpha, int beta,
        bool is_root, bool is_quiesce,
        int& r_eval, std::vector<Move>& r_pv, ull& r_nodes, int& r_maxdepth)
{
    const int alpha_init = alpha;
    std::vector<Move> legal_moves;
    ull attacks;
    int kpos = Bit::first(*pos.relative_bb(pos.turn).mk);
    Movegen::get_legal_moves(pos, legal_moves, attacks);
    const int remain_depth = std::max(maxdepth - mydepth, 0);
    const int static_eval = Eval::eval(pos, legal_moves.size(), attacks, kpos, mydepth)
        * (pos.turn ? 1 : -1);
    const ull hash = Transposition::hash(pos);
    TP& tp = *tptable.get(hash);
    const bool tp_good = (tp.depth != -1 && tp.hash == hash);

    // Set statistic variables.
    r_nodes++;
    r_maxdepth = std::max(r_maxdepth, mydepth);

    // End of game.
    if (legal_moves.size() == 0) {
        r_eval = static_eval;
        return;
    }

    // Transposition
    int tp_skip_ind = -1;
    if (tp_good) {
        // Check if beta cutoff.
        /*
        if (!is_root && tp.depth >= remain_depth) {
            if (tp.eval >= beta && tp.alpha < beta) {
                r_eval = beta;
                return;
            }
        }
        */

        // Move ordering.
        if (!tp.best_move.is_null()) {
            // Skip matching move already in vector.
            for (int i = 0; i < (int)legal_moves.size(); i++) {
                if (legal_moves[i] == tp.best_move) {
                    tp_skip_ind = i;
                    break;
                }
            }
            if (tp_skip_ind == -1)
                throw 1;

            legal_moves.push_back(tp.best_move);
        }
    }

    // Start quie search if remaining depth 0.
    if (!is_quiesce && remain_depth == 0) {
        unified_search(
                time_start, tptable, pos, maxdepth, mydepth + 1, movetime,
                alpha, beta,
                false, true,
                r_eval, r_pv, r_nodes, r_maxdepth);
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

    bool beta_cutoff = false;
    for (int i = legal_moves.size() - 1; i >= 0; i--) {
        if (remain_depth > 3 && maxdepth != 1 && Time::elapse(time_start) > movetime)
            return;
        if (i == tp_skip_ind)
            continue;

        // Return TP score if current alpha-beta bounds are good enough.
        // TP alpha-beta should be outside current alpha-beta.
        if (tp_good) {
            if (!is_root && tp.depth >= remain_depth) {
                if (tp.alpha <= alpha && tp.beta >= beta) {
                    r_eval = std::min(tp.eval, beta);
                    return;
                }
            }
        }

        const Move& move = legal_moves[i];

        // Check if quiesce and capture move.
        if (is_quiesce && !Bit::get(t_pieces, move.to))
            continue;

        Position new_pos = pos;
        new_pos.push(move);

        // Get eval of new position.
        int curr_eval;
        std::vector<Move> curr_pv;
        unified_search(
                time_start, tptable, new_pos, maxdepth, mydepth + 1, movetime,
                -beta, -alpha,
                false, is_quiesce,
                curr_eval, curr_pv, r_nodes, r_maxdepth);
        curr_eval = -curr_eval;

        // Check alpha beta.
        if (curr_eval >= beta) {
            beta_cutoff = true;
            break;
        }
        if (curr_eval > alpha) {
            alpha = curr_eval;
            r_pv.resize(0);
            r_pv.push_back(move);
            for (const Move& m: curr_pv)
                r_pv.push_back(m);
        }
    }

    // Set returns.
    r_eval = beta_cutoff ? beta : alpha;

    // Write to TP.
    bool write = false;
    write |= remain_depth > tp.depth;
    write |= (remain_depth == tp.depth) && (alpha_init < tp.alpha) && (beta > tp.beta);
    if (write) {
        const Move best_move = r_pv.size() > 0 ? r_pv[0] : Move();
        tptable.set(hash, remain_depth, r_eval, alpha_init, beta, best_move);
    }
}


Move search(Position& pos, int maxdepth, int movetime) {
    const ull time_start = Time::time();
    ull nodes = 0;

    Transposition::init();
    TPTable tptable;

    std::vector<Move> pv;
    int best_eval = 0;

    // Iterative deepening.
    for (int depth = 1; depth <= maxdepth; depth++) {
        int max_search_depth = 0;

        // Aspiration window.
        int curr_best_eval;
        std::vector<Move> curr_pv;
        int lower, upper;
        lower = upper = (depth == 1 ? 1e9 : 10);

        while (true) {
            //const int alpha = best_eval - lower, beta = best_eval + upper;
            //TODO currently window disabled: we can only write to TP if search doesnt fail.
            int alpha = -1e9, beta = 1e9;
            unified_search(
                    time_start, tptable, pos, depth, 0, movetime,
                    alpha, beta,
                    true, false,
                    curr_best_eval, curr_pv, nodes, max_search_depth);

            // Increase window if fail.
            if (curr_best_eval <= alpha)
                lower *= 2;
            else if (curr_best_eval >= beta)
                upper *= 2;
            else
                break;
        }
        if (depth > 1 && Time::elapse(time_start) > movetime)
            break;

        best_eval = curr_best_eval;
        pv = curr_pv;

        const int elapse = Time::elapse(time_start);
        bool search_done = false;

        SearchResult res;
        res.data["depth"] = std::to_string(depth);
        res.data["seldepth"] = std::to_string(max_search_depth);
        res.data["pv"] = Ascii::move_list(pv);
        res.data["nodes"] = std::to_string(nodes);
        res.data["nps"] = std::to_string(Time::nps(nodes, elapse));
        res.data["time"] = std::to_string(elapse);
        res.data["hashfull"] = std::to_string(tptable.get_hashfull());
        if (abs(best_eval) > 1e5) {
            int mate_in = (Eval::MATE_SCORE - abs(best_eval) + 1) / 2;
            res.data["score mate"] = std::to_string(mate_in * (best_eval > 0 ? 1 : -1));
            if (movetime < 1e9 && mate_in <= depth)
                search_done = true;
        } else {
            res.data["score cp"] = std::to_string(best_eval);
        }
        std::cout << res.uci() << std::endl;

        if (search_done)
            break;
    }

    return pv[0];
}


int get_movetime(const Position& pos, std::map<std::string, int>& args) {
    if (args.count("movetime"))
        return args["movetime"];

    int time_left = -1, time_inc = 0;
    if (pos.turn) {
        if (args.count("wtime")) time_left = args["wtime"];
        if (args.count("winc")) time_inc = args["winc"];
    } else {
        if (args.count("btime")) time_left = args["btime"];
        if (args.count("binc")) time_inc = args["binc"];
    }

    if (time_left == -1)
        return 1e9;  // Defaults to inf.

    int moves_left = std::max(50-pos.move, 12);
    int est_time_left = time_left + moves_left*time_inc;
    int move_time = est_time_left / moves_left * 0.7;
    move_time = std::min(move_time, (int)(time_left * 0.6));
    return move_time;
}


}
