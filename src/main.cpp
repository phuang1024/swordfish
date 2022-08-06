#include <iostream>

#include "sfutils.hpp"


int main() {
    Position pos;
    pos.setup_std();
    std::cout << pos << std::endl;

    Move move(square(4, 4), square(4, 5));
    std::cout << move.uci() << std::endl;
}
