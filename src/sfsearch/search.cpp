#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfuci.hpp"
#include "sfutils.hpp"

//using Transposition::TP;
//using Transposition::TPTable;


namespace Search {


static int negamax_wrapper(Position& pos, int depth, int alpha, int beta);


/**
 * depth: Remaining search depth.
 */
static int negamax(Position& pos, int depth, int alpha, int beta) {
    const int static_eval = Eval::eval(pos);// * (pos.turn ? 1 : -1);

    if (depth == 0) {
        return static_eval;
    }

    std::vector<Move> legal_moves;
    ull attacks;
    Movegen::get_legal_moves(pos, legal_moves, attacks);

    if (legal_moves.empty()) {
        return static_eval;
    }

    int best_eval = -1e9;
    for (int i = 0; i < (int)legal_moves.size(); i++) {
        Position new_pos = pos;
        new_pos.push(legal_moves[i]);

        int score = -negamax_wrapper(new_pos, depth - 1, 0, 0);
        if (score > best_eval) {
            best_eval = score;
        }
    }

    return best_eval;
}


/**
 * Wrapper for debugging purposes.
 */
static int negamax_wrapper(Position& pos, int depth, int alpha, int beta) {
    std::cout << "ENTER" << std::endl;
    std::cout << pos.get_fen() << std::endl;
    std::cout << Eval::eval(pos) << std::endl;
    int ret = negamax(pos, depth, alpha, beta);
    std::cout << "EXIT" << std::endl;
    return ret;
}


/**
 * Search main entry point.
 */
void search(Position& pos, int maxdepth) {
    negamax(pos, maxdepth, -1e9, 1e9);
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
