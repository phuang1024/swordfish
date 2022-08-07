#include <iostream>

#include "sfmovegen.hpp"
#include "sfutils.hpp"


int main() {
    Position pos;
    pos.setup_std();

    const ull attacks = Movegen::attacked_sqs(pos, WHITE, true);
    Ascii::print(std::cout, pos);
    Ascii::print(std::cout, attacks);
}
