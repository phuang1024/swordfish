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
        << VERSION_PATCH << std::endl << std::endl;

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
            Movegen::get_legal_moves(pos, moves);
            const int score = Eval::eval(pos, moves.size());
            std::cout << score << " cp (pov white)" << std::endl;
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
                const int maxdepth = cmd.args.count("depth") ? cmd.args["depth"] : 6;
                const Move bestmove = Search::search(pos, maxdepth);
                std::cout << "bestmove " << bestmove.uci() << std::endl;
            }
        }
    }
}
