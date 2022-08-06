#pragma once


constexpr bool WHITE = true,
               BLACK = false;

// Piece types and empty
constexpr int EMPTY = 0,
              WP = 1,
              WN = 2,
              WB = 3,
              WR = 4,
              WQ = 5,
              WK = 6,
              BP = 7,
              BN = 8,
              BB = 9,
              BR = 10,
              BQ = 11,
              BK = 12;


/**
 * Convert piece code (e.g. WP) into char representation (e.g. 'P')
 */
inline char piece_char(int piece) {
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
        default: return ' ';
    }
}
