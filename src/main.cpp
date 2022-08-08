#include <iostream>

#include "sfmovegen.hpp"
#include "sfutils.hpp"


int main() {
    Position pos;
    pos.setup_fen("8/2k5/8/3N4/8/2R5/8/K7 w - - 0 1");

    ull attacks;
    ull checkers;
    Movegen::board_info(pos, attacks, checkers);

    Ascii::print(std::cout, pos);
    std::cout << std::endl;
    Ascii::print(std::cout, attacks);
    std::cout << std::endl;
    Ascii::print(std::cout, checkers);
}
