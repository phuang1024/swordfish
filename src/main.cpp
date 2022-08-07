#include <iostream>

#include "sfmovegen.hpp"
#include "sfutils.hpp"


int main() {
    Position pos;
    pos.setup_std();

    Ascii::print(std::cout, pos);

    Movegen::attacked_sqs(pos, WHITE, true);
}
