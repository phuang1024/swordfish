#include <iostream>
#include <vector>

#include "sfmovegen.hpp"
#include "sfutils.hpp"


int main() {
    Position pos;
    pos.setup_fen("3r4/8/8/3K4/8/2r5/8/8 w - - 0 1");

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
