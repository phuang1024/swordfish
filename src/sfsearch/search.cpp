#include <set>

#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfuci.hpp"
#include "sfutils.hpp"

using Transposition::TP;
using Transposition::TPTable;
using Transposition::TPType;


namespace Search {


/**
 * Alpha beta negamax search that can act like:
 * * Root node: Sets r_eval
 * * Normal search: Sets r_eval
 * * Quiescence search: Evaluates as soon as position is quiet.
 *
 * Some algorithms implemented using pseudocode from https://chessprogramming.org
 *
 * @param maxdepth  Max depth of normal search (quiesce if exceeds).
 * @param mydepth  Depth of this node.
 * @param r_eval  Eval of this node relative to position's turn.
 * @param r_pv  PV starting from this node.
 * @param r_maxdepth  Max depth of search.
 */
static void unified_search(
        ull time_start, TPTable& tptable, Position& pos, int maxdepth, int mydepth, int movetime,
        int alpha, int beta,
        bool is_root, bool is_quiesce,
        int& r_eval, std::vector<Move>& r_pv, ull& r_nodes, int& r_maxdepth)
{
    if (maxdepth != 1 && Time::elapse(time_start) > movetime)
        return;

    std::vector<Move> legal_moves;
    ull attacks;
    Movegen::get_legal_moves(pos, legal_moves, attacks);
    // (score, legal_moves index)
    std::multiset<std::pair<int, int>> move_order;

    const int alpha_init = alpha;
    int kpos = Bit::first(*pos.relative_bb(pos.turn).mk);
    const int remain_depth = std::max(maxdepth - mydepth, 0);
    const int static_eval = Eval::eval(pos, legal_moves.size(), attacks, kpos, mydepth)
        * (pos.turn ? 1 : -1);

    const ull hash = tptable.hash(pos);
    TP& tp = *tptable.get(hash);
    const bool tp_good = (tp.type != TPType::NO_INFO && tp.hash == hash);
    const bool tp_moveorder_good = tp_good && tp.move_order[0] != -1;

    // Set statistic variables.
    r_nodes++;
    r_maxdepth = std::max(r_maxdepth, mydepth);

    // End of game.
    if (legal_moves.size() == 0) {
        r_eval = static_eval;
        return;
    }

    // Transposition
    if (tp_good) {
        if (!is_root && tp.depth >= remain_depth) {
            // Do this so don't have to type cast in every std::min
            const int tp_eval = tp.eval;
            if (tp.type == TPType::EXACT) {
                r_eval = std::min(std::max(tp_eval, alpha), beta);
                return;
            } else if (tp.type == TPType::FAIL_LOW) {
                if (tp_eval <= alpha) {
                    r_eval = alpha;
                    return;
                }
                alpha = std::max(alpha, tp_eval);
            } else if (tp.type == TPType::FAIL_HIGH) {
                if (tp_eval >= beta) {
                    r_eval = beta;
                    return;
                }
                beta = std::min(beta, tp_eval);
            } else if (tp.type == TPType::QUIESCE && is_quiesce) {
                r_eval = std::min(std::max(tp_eval, alpha), beta);
                return;
            }
        }
    }

    // Start quie search if remaining depth 0.
    if (!is_quiesce && remain_depth == 0) {
        std::vector<Move> curr_pv;
        unified_search(
                time_start, tptable, pos, maxdepth, mydepth + 1, movetime,
                alpha, beta,
                false, true,
                r_eval, curr_pv, r_nodes, r_maxdepth);
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

    Move best_move(0, 0);
    bool beta_cutoff = false;
    int last_eval = 123456789;
    for (int i = 0; i < (int)legal_moves.size(); i++) {
        const int move_index = tp_moveorder_good ? tp.move_order[i] : i;
        const Move& move = legal_moves[move_index];

        if (is_root) {
            std::cout << "info depth " << remain_depth << " currmove " <<
                move.uci() << " currmovenumber " << i+1 << std::endl;
        }

        // Check if quiesce and capture move.
        if (is_quiesce && !Bit::get(t_pieces, move.to))
            continue;

        // New position.
        Position new_pos = pos;
        new_pos.push(move);

        // Search on new pos.
        int curr_eval;
        std::vector<Move> curr_pv;
        unified_search(
                time_start, tptable, new_pos, maxdepth, mydepth + 1, movetime,
                -beta, -alpha,
                false, is_quiesce,
                curr_eval, curr_pv, r_nodes, r_maxdepth);
        curr_eval = -curr_eval;

        // Add to move ordering
        // If not improve score, write low value instead of prev alpha.
        int tp_score = (curr_eval != last_eval) ? curr_eval : -1e9;
        move_order.insert(std::pair<int, int>(tp_score, move_index));
        last_eval = curr_eval;

        // Check alpha beta.
        if (curr_eval >= beta) {
            beta_cutoff = true;
            r_pv.resize(0);
            break;
        }
        if (curr_eval > alpha) {
            alpha = curr_eval;
            best_move = move;
            r_pv.resize(0);
            r_pv.push_back(move);
            r_pv.insert(r_pv.end(), curr_pv.begin(), curr_pv.end());
        }
    }

    // Set returns.
    r_eval = beta_cutoff ? beta : alpha;

    // Write to TP.
    // We can be this much shallower and write.
    TPType type = TPType::EXACT;
    if (is_quiesce) {
        type = TPType::QUIESCE;
    } else if (alpha <= alpha_init) {
        type = TPType::FAIL_LOW;
    } else if (beta_cutoff) {
        type = TPType::FAIL_HIGH;
    }

    bool write = false;
    write |= type > tp.type;
    const int depth_thres = (int)tptable.search_index - (int)tp.search_index;
    const int deeper = remain_depth + depth_thres - tp.depth;
    write |= deeper > 0;

    if (write) {
        uint8_t* move_order_arr = nullptr;
        // Check reqs to make move order
        if (legal_moves.size() <= 64 && move_order.size() == legal_moves.size()) {
            move_order_arr = new uint8_t[legal_moves.size()];
            int i = 0;
            for (auto it = move_order.rbegin(); it != move_order.rend(); it++) {
                move_order_arr[i] = it->second;
                i++;
            }
        }

        tptable.set(type, hash, remain_depth, r_eval, legal_moves.size(), move_order_arr);
    }
}


Move search(TPTable& tptable, Position& pos, int maxdepth, int movetime) {
    const ull time_start = Time::time();
    ull nodes = 0;

    Move best_move(0, 0);
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
        best_move = curr_pv[0];

        const int elapse = Time::elapse(time_start);
        bool search_done = false;

        SearchResult res;
        res.data["depth"] = std::to_string(depth);
        res.data["seldepth"] = std::to_string(max_search_depth);
        for (const Move& move: curr_pv) {
            res.data["pv"] += move.uci() + " ";
        }
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

    return best_move;
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
