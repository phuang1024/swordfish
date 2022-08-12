#include <iostream>
#include <vector>

#include "sfmovegen.hpp"
#include "sfutils.hpp"


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

    pos.setup_fen("k7/8/8/q7/8/R7/8/K7 w - - 0 1");

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    Ascii::print(std::cerr, pos);
    std::cerr << std::endl;
    std::cerr << moves.size() << " moves:\n";
    for (auto move: moves) {
        Ascii::print(std::cerr, move);
        std::cerr << std::endl;
    }
}
