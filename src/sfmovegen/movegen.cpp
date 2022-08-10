#include <iostream>

#include "sfmovegen.hpp"

namespace Movegen {


static inline ull attacks_pawn(int x, int y, bool side) {
    if (y == 0 || y == 7)
        return 0;

    const int dy = (side ? 1 : -1);

    ull attacks = 0;
    if (x > 0)
        attacks = Bit::set(attacks, square(x-1, y+dy));
    if (x < 7)
        attacks = Bit::set(attacks, square(x+1, y+dy));

    return attacks;
}

/**
 * Knight or king.
 */
static inline ull attacks_offsets(int x, int y, const int offsets[8][2]) {
    ull attacks = 0;
    for (int i = 0; i < 8; i++) {
        const int cx = x + offsets[i][0];
        const int cy = y + offsets[i][1];
        if (in_board(cx, cy))
            attacks = Bit::set(attacks, square(cx, cy));
    }

    return attacks;
}

/**
 * Rook, bishop, or queen.
 */
static inline ull attacks_sliding(int x, int y, const int offsets[4][2], ull all_pieces) {
    const int start = square(x, y);
    all_pieces = Bit::unset(all_pieces, start);

    ull attacks = 0;
    for (int i = 0; i < 4; i++) {
        attacks |= bb_sequence(start, offsets[i][0], offsets[i][1], all_pieces, false, true);
    }

    return attacks;
}

void board_info(bool turn, const RelativeBB& relbb, ull& r_attacked, ull& r_checkers, ull& r_pinned) {
    r_attacked = r_checkers = r_pinned = 0;

    const ull t_pieces_nok = relbb.t_pieces & ~*relbb.tk;
    const ull a_pieces_nok = relbb.m_pieces | t_pieces_nok;
    const int tkpos = Bit::first(*relbb.tk);

    ull sliding_attacks = 0;  // Used to compute pins.
    for (int i = 0; i < 64; i++) {
        const int x = i % 8, y = i / 8;
        ull attacks = 0;

        if (Bit::get(*relbb.mp, i)) {
            attacks |= attacks_pawn(x, y, turn);
        } else if (Bit::get(*relbb.mn, i)) {
            attacks |= attacks_offsets(x, y, KNIGHT_OFFSETS);
        } else if (Bit::get(*relbb.mk, i)) {
            attacks |= attacks_offsets(x, y, KING_OFFSETS);
        }
        if (Bit::get(*relbb.mb, i) || Bit::get(*relbb.mq, i)) {
            attacks |= attacks_sliding(x, y, BISHOP_OFFSETS, a_pieces_nok);
            sliding_attacks |= attacks;
        }
        if (Bit::get(*relbb.mr, i) || Bit::get(*relbb.mq, i)) {
            attacks |= attacks_sliding(x, y, ROOK_OFFSETS, a_pieces_nok);
            sliding_attacks |= attacks;
        }

        r_attacked |= attacks;
        if (attacks & *relbb.tk)
            r_checkers = Bit::set(r_checkers, i);
    }

    // Compute sliding attacks from enemy king.
    ull king_sliding = 0;
    for (int i = 0; i < 4; i++) {
        king_sliding |= bb_sequence(tkpos, KING_OFFSETS[i][0], KING_OFFSETS[i][1], a_pieces_nok,
            false, true);
    }

    r_pinned = sliding_attacks & king_sliding & t_pieces_nok;
}


/**
 * Doesn't add if to & m_pieces
 */
static inline void add_move(std::vector<Move>& moves, int from, int to, ull m_pieces) {
    if (Bit::get(m_pieces, to))
        return;
    moves.push_back(Move(from, to));
}

static inline void get_king_moves(const RelativeBB& relbb, int kx, int ky, ull danger,
        std::vector<Move>& r_moves) {
    const int start = square(kx, ky);
    for (int i = 0; i < 8; i++) {
        const int x = kx + KING_OFFSETS[i][0], y = ky + KING_OFFSETS[i][1];
        if (in_board(x, y)) {
            const int sq = square(x, y);
            if (!Bit::get(danger, sq))
                add_move(r_moves, start, sq, relbb.m_pieces);
        }
    }
}

static inline void get_knight_moves(const RelativeBB& relbb, int x, int y, ull mask,
        std::vector<Move>& r_moves) {
    const int start = square(x, y);
    for (int i = 0; i < 8; i++) {
        const int nx = x + KNIGHT_OFFSETS[i][0], ny = y + KNIGHT_OFFSETS[i][1];
        if (in_board(nx, ny)) {
            const int sq = square(nx, ny);
            if (Bit::get(mask, sq))
                add_move(r_moves, start, sq, relbb.m_pieces);
        }
    }
}

static inline void get_sliding_moves(const RelativeBB& relbb, int x, int y, const int offsets[4][2],
        ull mask, std::vector<Move>& r_moves) {
    const int start = square(x, y);
    // End sequence at all pieces except this.
    const ull stop_at = relbb.a_pieces & ~Bit::mask(start);

    for (int i = 0; i < 4; i++) {
        // Possible destinations: In sequence, not same side, in pin or capture mask.
        const ull dests = bb_sequence(start, offsets[i][0], offsets[i][1],
                stop_at, false, true) & ~relbb.m_pieces & mask;
        if (dests == 0)
            continue;

        for (int sq = 0; sq < 64; sq++)
            if (Bit::get(dests, sq))
                add_move(r_moves, start, sq, relbb.m_pieces);
    }
}

void get_legal_moves(Position& pos, std::vector<Move>& r_moves) {
    RelativeBB relbb = pos.relative_bb(pos.turn);
    ull attacked, checkers, pinned;
    board_info(!pos.turn, relbb.swap_sides(), attacked, checkers, pinned);
    const int num_checkers = Bit::popcnt(checkers);
    const int kpos = Bit::first(*relbb.mk);
    const int kx = kpos % 8, ky = kpos / 8;

    std::cerr << "attacked\n"; Ascii::print(std::cerr, attacked); std::cerr << std::endl;
    std::cerr << "checkers\n"; Ascii::print(std::cerr, checkers); std::cerr << std::endl;
    std::cerr << "pinned\n"; Ascii::print(std::cerr, pinned); std::cerr << std::endl;

    get_king_moves(relbb, kx, ky, attacked, r_moves);

    if (num_checkers >= 2) {
        // Only king moves.
        return;
    }

    ull capture_mask = 0xffffffffffffffff;
    ull push_mask = 0xffffffffffffffff;
    ull all_mask = 0xffffffffffffffff;

    if (num_checkers == 1) {
        // Calculate push and capture mask.
        const int checker_pos = Bit::first(checkers);
        const int checker_x = checker_pos % 8, checker_y = checker_pos / 8;
        const int dx = (checker_x == kx ? 0 : (checker_x > kx ? 1 : -1)),
                  dy = (checker_y == ky ? 0 : (checker_y > ky ? 1 : -1));
        push_mask = bb_sequence(kpos, dx, dy, checkers, false, false);
        capture_mask = checkers;
        all_mask = push_mask | capture_mask;
    }

    for (int sq = 0; sq < 64; sq++) {
        const int x = sq % 8, y = sq / 8;

        // Knight
        if (!Bit::get(pinned, sq) && Bit::get(*relbb.mn, sq)) {
            get_knight_moves(relbb, x, y, all_mask, r_moves);
            continue;
        }

        // Sliding
        ull pin_mask = 0xffffffffffffffff;
        if (Bit::get(pinned, sq)) {
            const int dx = x == kx ? 0 : (x > kx ? 1 : -1),
                      dy = y == ky ? 0 : (y > ky ? 1 : -1);
            pin_mask = bb_sequence(kpos, dx, dy, relbb.t_pieces, false, false);
        }

        const ull sliding_mask = all_mask & pin_mask;
        if (Bit::get(*relbb.mb, sq) || Bit::get(*relbb.mq, sq))
            get_sliding_moves(relbb, x, y, BISHOP_OFFSETS, sliding_mask, r_moves);
        if (Bit::get(*relbb.mr, sq) || Bit::get(*relbb.mq, sq))
            get_sliding_moves(relbb, x, y, ROOK_OFFSETS, sliding_mask, r_moves);
    }
}


}  // namespace Movegen
