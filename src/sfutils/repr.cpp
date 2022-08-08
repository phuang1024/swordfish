// Implementation of printing out things.


#include <iostream>

#include "sfutils.hpp"


namespace Ascii {


void print(std::ostream& os, ull bb) {
    for (int y = 7; y >= 0; y--) {
        for (int x = 0; x < 8; x++) {
            os << (Bit::get(bb, square(x, y)) ? 'X' : '-');
            os << ' ';
        }
        os << std::endl;
    }
}


/**
 * Prints row separator in ascii board diagram.
 */
static inline void print_row(std::ostream& os) {
    os << ' ';
    for (int i = 0; i < 8; i++)
        os << "+---";
    os << "+\n";
}

/**
 * Prints column separator in ascii board diagram.
 */
static inline void print_sep(std::ostream& os) {
    os << " | ";
}

void print(std::ostream& os, const Position& pos) {
    for (int y = 7; y >= 0; y--) {
        print_row(os);
        for (int x = 0; x < 8; x++) {
            print_sep(os);
            char ch = Ascii::piece2char(pos.piece_at(square(x, y)));
            os << ch;
        }
        print_sep(os);
        os << y+1 << '\n';
    }
    print_row(os);
    for (char i = 'a'; i <= 'h'; i++) {
        os << "   " << i;
    }
    os << std::endl;
    os << std::endl;

    os << "Fen: " << pos.get_fen() << std::endl;
}


void print(std::ostream& os, const Move& move) {
    os << move.uci();
}


}  // namespace Ascii
