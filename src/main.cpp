#include <iostream>

#include "sfmovegen.hpp"
#include "sfutils.hpp"


int main() {
    Position pos;
    pos.setup_std();

    ull attacks;
    ull checkers;
    Movegen::board_info(pos, attacks, checkers);

    Ascii::print(std::cout, pos);
    std::cout << std::endl;
    Ascii::print(std::cout, attacks);
    std::cout << std::endl;
    Ascii::print(std::cout, checkers);
}
