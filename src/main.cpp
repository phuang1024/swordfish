#include <iostream>

#include "sfutils.hpp"


int main() {
    Position pos;
    pos.setup_std();
    std::cout << pos << std::endl;
    //pos.piece_bb(square(5, 5));

    Move move(square(4, 4), square(4, 5));
    std::cout << move << std::endl;
}
