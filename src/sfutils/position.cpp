#include <iostream>

#include "position.hpp"


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

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    for (int y = 7; y >= 0; y--) {
        print_row(os);
        for (int x = 0; x < 8; x++) {
            print_sep(os);
            // TODO print piece char
            os << ' ';
        }
        print_sep(os);
        os << y+1 << '\n';
    }
    print_row(os);
    for (char i = 'a'; i <= 'h'; i++) {
        os << "   " << i;
    }
    return os;
}
