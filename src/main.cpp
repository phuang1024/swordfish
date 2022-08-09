#include <iostream>

#include "sfmovegen.hpp"
#include "sfutils.hpp"


int main() {
    //Ascii::print(std::cout, Movegen::bb_sequence(0, 1, 1, Bit::mask(27), true));
    //Ascii::print(std::cout, Movegen::bb_sequence(0, 1, 1, Bit::mask(27), false));

    Position pos;
    pos.setup_fen("8/8/8/8/2Q5/8/8/8 w - - 0 1");

    ull attacks;
    ull checkers;
    Movegen::board_info(pos, attacks, checkers);

    Ascii::print(std::cout, pos);
    std::cout << std::endl;
    Ascii::print(std::cout, attacks);
    std::cout << std::endl;
    Ascii::print(std::cout, checkers);
}
