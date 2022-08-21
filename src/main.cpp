#include <iostream>
#include <string>
#include <vector>

#include "config.hpp"
#include "sfeval.hpp"
#include "sfmovegen.hpp"
#include "sfsearch.hpp"


int main() {
    // Print program info.
    std::cerr << "Swordfish v" << VERSION_MAJOR << "." << VERSION_MINOR << "."
        << VERSION_PATCH << std::endl;

    Position pos;
    pos.setup_std();

    // UCI loop
    while (true) {
        UCICommand cmd(std::cin);

        if (cmd.mode == "quit") {
            break;
        } else if (cmd.mode == "d") {
            Ascii::print(std::cout, pos);
        } else if (cmd.mode == "eval") {
            std::vector<Move> moves;
            ull attacks;
            Movegen::get_legal_moves(pos, moves, attacks);
            int kpos = Bit::first(*pos.relative_bb(pos.turn).mk);
            const int score = Eval::eval(pos, moves.size(), attacks, kpos, 0);
            std::cout << score << " cp (pov current turn)" << std::endl;
        } else if (cmd.mode == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (cmd.mode == "uci") {
            std::cout << "uciok" << std::endl;
        } else if (cmd.mode == "ucinewgame") {
            pos.setup_std();
        } else if (cmd.mode == "position") {
            pos = cmd.pos;
        } else if (cmd.mode == "go") {
            if (cmd.args.count("perft")) {
                SearchResult res = Search::perft(pos, cmd.args["perft"]);
                std::cout << res.uci() << std::endl;
            } else {
                const int movetime = Search::get_movetime(pos, cmd.args);
                const int maxdepth = cmd.args.count("depth") ? cmd.args["depth"] : 255;
                const Move bestmove = Search::search(pos, maxdepth, movetime);
                std::cout << "bestmove " << bestmove.uci() << std::endl;
            }
        }
    }
}
