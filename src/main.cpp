#include <iostream>
#include <string>
#include <vector>

#include "config.hpp"
#include "sfeval.hpp"
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
            const int score = Eval::eval(pos);
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
                const ull time_start = Time::time();

                SearchResult res;
                for (int depth = 1; ; depth++) {
                    res = Search::search(pos, depth);
                    std::cout << res.uci() << std::endl;

                    if (Time::elapse(time_start) > 1000)
                        break;
                }

                std::cout << "bestmove " << res.data["pv"] << std::endl;
            }
        }
    }


    /*
    pos.setup_std();
    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    pos.push(Move(square(4, 1), square(4, 3)));
    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    pos.push(Move(square(4, 6), square(4, 4)));
    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    return 0;

    //pos.setup_fen("5r2/8/8/8/8/8/8/R3K2R w KQ - 0 1");
    pos.setup_std();

    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    for (int depth = 1; depth < 7; depth++)
    std::cerr << "perft, depth=" << depth << ": " << Perft::perft(pos, depth).uci() << std::endl;
    */
}
