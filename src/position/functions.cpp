//
//  Swordfish
//  A chess engine.
//  Copyright Patrick Huang 2021
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

/**
 * General helper methods and functions.
 */

#include <iostream>
#include <string>

#include "position.hpp"


namespace Position {


char piece2char(const char piece) {
    switch (piece) {
        case WP: return 'P';
        case WN: return 'N';
        case WB: return 'B';
        case WR: return 'R';
        case WQ: return 'Q';
        case WK: return 'K';
        case BP: return 'p';
        case BN: return 'n';
        case BB: return 'b';
        case BR: return 'r';
        case BQ: return 'q';
        case BK: return 'k';
        case EMPTY: return ' ';
    }
    throw Errors::InvalidArg;
}

char char2piece(const char piece) {
    switch (piece) {
        case 'P': return WP;
        case 'N': return WN;
        case 'B': return WB;
        case 'R': return WR;
        case 'Q': return WQ;
        case 'K': return WK;
        case 'p': return BP;
        case 'n': return BN;
        case 'b': return BB;
        case 'r': return BR;
        case 'q': return BQ;
        case 'k': return BK;
        case ' ': return EMPTY;
    }
    throw Errors::InvalidArg;
}

std::string sq2alg(const char sq) {
    const int x = sq & 7, y = sq >> 3;
    const char x_ch = x + 97, y_ch = y + 49;
    return std::string(1, x_ch) + std::string(1, y_ch);
}


Position::Position() {
}

Position::Position(const char code) {
    if (code == INIT_EMPTY) {
        wp = wn = wb = wr = wq = wk = bp = bn = bb = br = bq = bk = 0;
        meta = 0;
        moveclock = 0;
        fullmoves = 1;
    } else if (code == INIT_START) {
        wp = START_WP;
        wn = START_WN;
        wb = START_WB;
        wr = START_WR;
        wq = START_WQ;
        wk = START_WK;
        bp = START_BP;
        bn = START_BN;
        bb = START_BB;
        br = START_BR;
        bq = START_BQ;
        bk = START_BK;
        meta = 31;
        moveclock = 0;
        fullmoves = 1;
    }
}

char Position::piece_at(const char sq) const {
    if (bit(wp, sq)) return WP;
    if (bit(wn, sq)) return WN;
    if (bit(wb, sq)) return WB;
    if (bit(wr, sq)) return WR;
    if (bit(wq, sq)) return WQ;
    if (bit(wk, sq)) return WK;
    if (bit(bp, sq)) return BP;
    if (bit(bn, sq)) return BN;
    if (bit(bb, sq)) return BB;
    if (bit(br, sq)) return BR;
    if (bit(bq, sq)) return BQ;
    if (bit(bk, sq)) return BK;
    return EMPTY;
}

char Position::piece_at(const char x, const char y) const {
    return piece_at(square(x, y));
}

std::string Position::fen() const {
    std::string str = "";

    for (int y = 7; y >= 0; y--) {
        int empty = 0;
        for (int x = 0; x < 8; x++) {
            const char piece = piece_at(x, y);
            if (piece == EMPTY) {
                empty++;
            } else {
                if (empty != 0)
                    str += std::to_string(empty);
                empty = 0;
                str += piece2char(piece);
            }
        }
        if (empty != 0)
            str += std::to_string(empty);
        if (y != 0)
            str += "/";
    }
    str += " ";

    str += (meta & TURN) ? "w" : "b";
    str += " ";

    const bool wk = meta & C_WK;
    const bool wq = meta & C_WQ;
    const bool bk = meta & C_BK;
    const bool bq = meta & C_BQ;
    if (wk || wq || bk || bq) {
        if (wk) str += "K";
        if (wq) str += "Q";
        if (bk) str += "k";
        if (bq) str += "q";
    } else {
        str += "-";
    }
    str += " ";

    if (ep & EP_YES) str += sq2alg(ep & EP_SQ);
    else str += "-";
    str += " ";

    str += std::to_string(moveclock);
    str += " ";
    str += std::to_string(fullmoves);

    return str;
}


void print(std::ostream& fp, const Position& pos) {
    // Design from stockfish.
    const std::string row = " +---+---+---+---+---+---+---+---+";
    const std::string col = " | ";
    const std::string columns = "   a   b   c   d   e   f   g   h";

    for (int y = 7; y >= 0; y--) {
        std::cout << row << '\n';
        std::cout << col;
        for (int x = 0; x < 8; x++) {
            const char piece = pos.piece_at(y*8 + x);
            const char symbol = piece2char(piece);
            std::cout << symbol << col;
        }
        std::cout << y+1 << '\n';
    }
    std::cout << row << '\n' << columns << "\n\n" << std::endl;
    std::cout << "Fen: " << pos.fen() << "\n\n";
}

void print(std::ostream& fp, const ULL board) {
    for (int y = 7; y >= 0; y--) {
        for (int x = 0; x < 8; x++) {
            bool set = bit(board, y*8 + x);
            std::cout << (set ? 'X' : '-') << " ";
        }
        std::cout << '\n';
    }
    std::cout << std::flush;
}


}  // namespace Position
