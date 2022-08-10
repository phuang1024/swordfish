#include <iostream>
#include <vector>

#include "sfmovegen.hpp"
#include "sfutils.hpp"


int main() {
    Position pos;
    pos.setup_std();
    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    pos.push(Move(12, 28));
    Ascii::print(std::cerr, pos); std::cerr << std::endl;
    return 0;

    pos.setup_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

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
