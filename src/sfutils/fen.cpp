#include "sfutils.hpp"


void Position::setup_fen(std::string fen) {
    std::string::iterator it = fen.begin();
    char ch;

    // Pieces
    int y = 7;
    int x = 0;
    while ((ch = *it++) != ' ') {
        if (ch == '/') {
            y--;
            x = 0;
            continue;
        }
        if (ch >= '1' && ch <= '8') {
            x += ch - '0';
            continue;
        }

        int sq = square(x, y);
        int piece = Ascii::char2piece(ch);
        if (piece == EMPTY) {
            std::cerr << "sfutils:Position:setup_fen: Invalid piece FEN: " << fen << std::endl;
            throw 0;
        }
        set_at(sq, piece);
        x++;
    }

    // Turn
    while ((ch = *it++) != ' ') {
        if (ch == 'w') {
            turn = WHITE;
        } else if (ch == 'b') {
            turn = BLACK;
        } else {
            std::cerr << "sfutils:Position:setup_fen: Invalid turn FEN: " << fen << std::endl;
            throw 0;
        }
    }

    // Castling rights
    castling = 0;
    while ((ch = *it++) != ' ') {
        if (ch == '-') continue;
        else if (ch == 'K') castling = Bit::set(castling, 0);
        else if (ch == 'Q') castling = Bit::set(castling, 1);
        else if (ch == 'k') castling = Bit::set(castling, 2);
        else if (ch == 'q') castling = Bit::set(castling, 3);
        else {
            std::cerr << "sfutils:Position:setup_fen: Invalid castling FEN: " << fen << std::endl;
            throw 0;
        }
    }

    // EP
    x = -1, y = -1;
    while ((ch = *it++) != ' ') {
        if (ch == '-')
            continue;
        else if ('a' <= ch && ch <= 'h')
            x = ch - 'a';
        else if ('1' <= ch && ch <= '8')
            y = ch - '1';
        else {
            std::cerr << "sfutils:Position:setup_fen: Invalid EP FEN: " << fen << std::endl;
            throw 0;
        }
    }
    if (x != -1 && y != -1)
        ep = square(x, y);
    else
        ep = -1;

    // 50 move rule
    while ((ch = *it++) != ' ') {
        moves50 = ch - '0';
    }

    // Fullmove number
    move = *it - '0';
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
                    fen += '0' + empty_count;
                }
                empty_count = 0;

                const char ch = Ascii::piece2char(piece);
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

    // Castling
    if (Bit::get(castling, 0)) fen += 'K';
    if (Bit::get(castling, 1)) fen += 'Q';
    if (Bit::get(castling, 2)) fen += 'k';
    if (Bit::get(castling, 3)) fen += 'q';
    if (castling == 0) fen += '-';
    fen += ' ';

    // EP
    if (ep == -1) {
        fen += '-';
    } else {
        fen += Ascii::square2str(ep);
    }
    fen += ' ';

    // Moves
    fen += '0' + moves50;
    fen += ' ';
    fen += '0' + move;

    return fen;
}
