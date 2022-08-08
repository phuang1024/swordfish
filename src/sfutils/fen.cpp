#include "sfutils.hpp"


void Position::setup_fen(const std::string& fen) {
}


std::string Position::get_fen() const {
    std::string fen;

    // Pieces
    for (int y = 7; y >= 0; y--) {
        int empty_count = 0;
        for (int x = 0; x < 8; x++) {
            const int piece = piece_at(square(x, y));
            if (piece == EMPTY) {
                empty_count++;
            } else {
                if (empty_count > 0) {
                    fen += empty_count;
                }
                empty_count = 0;

                const char ch = Ascii::piece_char(piece);
                fen += ch;
            }
        }

        if (empty_count > 0) {
            fen += '0' + empty_count;
        }
        if (y != 0) {
            fen += '/';
        }
    }
    fen += ' ';

    // Turn
    fen += (turn ? 'w' : 'b');
    fen += ' ';

    // Castling TODO
    fen += '-';
    fen += ' ';

    // EP TODO
    fen += '-';
    fen += ' ';

    // Moves TODO
    fen += "0 1";

    return fen;
}
