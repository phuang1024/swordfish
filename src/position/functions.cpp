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
 * Helper functions.
 * Also implementations of methods.
 */

#include <iostream>
#include <sstream>
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

char alg2sq(const std::string alg) {
    return (alg[0]-97) + 8 * (alg[1]-49);
}

char promo2char(const char promo) {
    switch (promo) {
        case 0: return 'N';
        case 1: return 'B';
        case 2: return 'R';
        case 3: return 'Q';
    }
    throw Errors::InvalidArg;
}

char char2promo(const char ch) {
    switch (ch) {
        case 'N': return 0;
        case 'B': return 1;
        case 'R': return 2;
        case 'Q': return 3;
    }
    throw Errors::InvalidArg;
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

Position::Position(const std::string fen) {
    wp = wn = wb = wr = wq = wk = bp = bn = bb = br = bq = bk = 0;

    std::istringstream stream(fen);
    std::string section;
    for (int i = 0; i < 6 && std::getline(stream, section, ' '); i++) {
        if (i == 0) {
            int x = 0, y = 7;
            for (char ch: section) {
                if (ch == '/') {
                    x = 0;
                    y--;
                } else if (49 <= ch && ch <= 57) {
                    for (int j = 0; j < ch - 48; j++) {
                        set_at(square(x, y), 0);
                        x++;
                    }
                } else {
                    const char piece = char2piece(ch);
                    set_at(square(x, y), piece);
                    x++;
                }
            }
        } else if (i == 1) {
            meta = (section[0] == 'w') << 4;
        } else if (i == 2) {
            if (section[0] == '-') {
                for (int j = 0; j < 4; j++)
                    meta = bunset(meta, j);
            } else {
                for (char ch: section) {
                    if (ch == 'K') meta = bset(meta, 0);
                    if (ch == 'Q') meta = bset(meta, 1);
                    if (ch == 'k') meta = bset(meta, 2);
                    if (ch == 'q') meta = bset(meta, 3);
                }
            }
        } else if (i == 3) {
            if (section[0] == '-') ep = 0;
            else ep = 64 + alg2sq(section);
        } else if (i == 4) {
            moveclock = std::stoi(section);
        } else if (i == 5) {
            fullmoves = std::stoi(section); 
        }
    }
}

char Position::get_at(const char sq) const {
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

void Position::set_at(const char sq, const char piece) {
    if (piece == EMPTY) {
        wp = bunset(wp, sq);
        wn = bunset(wn, sq);
        wb = bunset(wb, sq);
        wr = bunset(wr, sq);
        wq = bunset(wq, sq);
        wk = bunset(wk, sq);
        bp = bunset(bp, sq);
        bn = bunset(bn, sq);
        bb = bunset(bb, sq);
        br = bunset(br, sq);
        bq = bunset(bq, sq);
        bk = bunset(bk, sq);
    } else {
        if      (piece == WP) wp = bset(wp, sq);
        else if (piece == WN) wn = bset(wn, sq);
        else if (piece == WB) wb = bset(wb, sq);
        else if (piece == WR) wr = bset(wr, sq);
        else if (piece == WQ) wq = bset(wq, sq);
        else if (piece == WK) wk = bset(wk, sq);
        else if (piece == BP) bp = bset(bp, sq);
        else if (piece == BN) bn = bset(bn, sq);
        else if (piece == BB) bb = bset(bb, sq);
        else if (piece == BR) br = bset(br, sq);
        else if (piece == BQ) bq = bset(bq, sq);
        else if (piece == BK) bk = bset(bk, sq);
    }
}

std::string Position::fen() const {
    std::string str = "";

    for (int y = 7; y >= 0; y--) {
        int empty = 0;
        for (int x = 0; x < 8; x++) {
            const char piece = get_at(square(x, y));
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


RespectivePieces::RespectivePieces() {
    SP = SN = SB = SR = SQ = SK = 0;
    OP = ON = OB = OR = OQ = OK = 0;
}

RespectivePieces::RespectivePieces(ULL SP, ULL SN, ULL SB, ULL SR, ULL SQ, ULL SK,
                                   ULL OP, ULL ON, ULL OB, ULL OR, ULL OQ, ULL OK) {
    this->SP = SP;
    this->SN = SN;
    this->SB = SB;
    this->SR = SR;
    this->SQ = SQ;
    this->SK = SK;
    this->OP = OP;
    this->ON = ON;
    this->OB = OB;
    this->OR = OR;
    this->OQ = OQ;
    this->OK = OK;
}


Move::Move() {
}

Move::Move(const UCH from, const UCH to) {
    this->from = from;
    this->to = to;
    promo = 0;
}

Move::Move(const UCH from, const UCH to, const UCH promo) {
    this->from = from;
    this->to = to;
    this->promo = PROMO_YES + promo;
}

std::string Move::uci() const {
    std::string repr = sq2alg(from) + sq2alg(to);
    if (PROMO_YES & promo)
        repr += promo2char(promo & PROMO);
    return repr;
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
            const char piece = pos.get_at(y*8 + x);
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
