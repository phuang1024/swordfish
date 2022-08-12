#include <iostream>
#include <vector>

#include "sfsearch.hpp"


int main() {
    Position pos;

    /*
    pos.setup_std();
    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    pos.push(Move(square(4, 1), square(4, 3)));
    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    pos.push(Move(square(4, 6), square(4, 4)));
    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    return 0;
    */

    //pos.setup_fen("5r2/8/8/8/8/8/8/R3K2R w KQ - 0 1");
    pos.setup_std();

    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    for (int depth = 1; depth < 7; depth++)
    std::cerr << "perft, depth=" << depth << ": " << Perft::perft(pos, depth).uci() << std::endl;
}
