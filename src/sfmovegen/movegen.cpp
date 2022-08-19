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
        }
        if (Bit::get(*relbb.mr, i) || Bit::get(*relbb.mq, i)) {
            attacks |= attacks_sliding(x, y, ROOK_OFFSETS, a_pieces_nok);
        }

        r_attacked |= attacks;
        if (attacks & *relbb.tk)
            r_checkers = Bit::set(r_checkers, i);
    }

    // Compute pins
    for (int i = 0; i < 8; i++) {
        const int dx = KING_OFFSETS[i][0], dy = KING_OFFSETS[i][1];
        const ull line = bb_sequence(tkpos, dx, dy, relbb.m_pieces, false, true);
        if (line & relbb.m_pieces) {
            ull poi = *relbb.mq;  // Pieces of interest (can pin this direction).
            if (dx == 0 || dy == 0)
                poi |= *relbb.mr;
            else
                poi |= *relbb.mb;

            if (line & poi) {
                const ull in_middle = line & relbb.t_pieces;
                if (Bit::popcnt(in_middle) == 1) {
                    const int sq = Bit::first(in_middle);
                    r_pinned = Bit::set(r_pinned, sq);
                }
            }
        }
    }
}


/**
 * if (Bit::get(mask, to)): add_move; return true;
 * else: return false;
 */
static inline bool add_move(int from, int to, ull mask, std::vector<Move>& r_moves) {
    if (Bit::get(mask, to)) {
        r_moves.push_back(Move(from, to));
        return true;
    }
    return false;
}

/**
 * Adds promo moves if promo.
 */
static inline bool add_pawn_move(int from, int to, ull mask, bool turn, std::vector<Move>& r_moves) {
    if (!Bit::get(mask, to))
        return false;

    const int y = to / 8;
    const bool is_promo = (turn && y == 7) || (!turn && y == 0);
    if (is_promo) {
        r_moves.push_back(Move(from, to, Promo::KNIGHT));
        r_moves.push_back(Move(from, to, Promo::BISHOP));
        r_moves.push_back(Move(from, to, Promo::ROOK));
        r_moves.push_back(Move(from, to, Promo::QUEEN));
    } else {
        r_moves.push_back(Move(from, to));
    }

    return true;
}

static inline void get_king_moves(const RelativeBB& relbb, int kx, int ky, ull mask,
        std::vector<Move>& r_moves) {
    const int start = square(kx, ky);
    for (int i = 0; i < 8; i++) {
        const int x = kx + KING_OFFSETS[i][0], y = ky + KING_OFFSETS[i][1];
        if (in_board(x, y)) {
            const int sq = square(x, y);
            add_move(start, sq, mask, r_moves);
        }
    }
}

static inline void get_pawn_moves(const RelativeBB& relbb, int x, int y, bool turn, int kpos,
        ull mask, const int ep_square, std::vector<Move>& r_moves) {
    const int start = square(x, y);
    const int pawn_dir = turn ? 1 : -1;
    const int one_sq_dest = start + 8*pawn_dir;

    // Push moves
    const ull push_mask = mask & ~relbb.a_pieces;
    add_pawn_move(start, one_sq_dest, push_mask, turn, r_moves);
    if ((turn && y == 1) || (!turn && y == 6))   // Double push
        if (!Bit::get(relbb.a_pieces, one_sq_dest))
            add_move(start, start + 16*pawn_dir, push_mask, r_moves);

    // Capture moves
    ull capture_dests = relbb.t_pieces;
    if (ep_square != -1) {
        // Check for EP discovered check.
        ull pieces = relbb.a_pieces &
            ~(Bit::mask(start) | Bit::mask(ep_square - 8*pawn_dir) | Bit::mask(kpos));
        ull horiz_attacks = bb_sequence(kpos, 1, 0, pieces, false, true)
                          | bb_sequence(kpos, -1, 0, pieces, false, true);

        if (horiz_attacks & (*relbb.tr | *relbb.tq)) {
            // There is check if we take EP.
            // So no EP move.
        } else {
            capture_dests |= Bit::mask(ep_square);
        }
    }
    int sq;
    if (x > 0 && Bit::get(capture_dests, sq = one_sq_dest - 1))
        add_pawn_move(start, sq, mask, turn, r_moves);
    if (x < 7 && Bit::get(capture_dests, sq = one_sq_dest + 1))
        add_pawn_move(start, sq, mask, turn, r_moves);
}

static inline void get_knight_moves(const RelativeBB& relbb, int x, int y, ull mask,
        std::vector<Move>& r_moves) {
    const int start = square(x, y);
    for (int i = 0; i < 8; i++) {
        const int nx = x + KNIGHT_OFFSETS[i][0], ny = y + KNIGHT_OFFSETS[i][1];
        if (in_board(nx, ny)) {
            const int sq = square(nx, ny);
            add_move(start, sq, mask, r_moves);
        }
    }
}

static inline void get_sliding_moves(const RelativeBB& relbb, int x, int y, const int offsets[4][2],
        ull mask, std::vector<Move>& r_moves) {
    const int start = square(x, y);

    for (int i = 0; i < 4; i++) {
        int cx = x, cy = y;
        while (true) {
            cx += offsets[i][0];
            cy += offsets[i][1];
            if (!in_board(cx, cy))
                break;
            const int sq = square(cx, cy);
            if (Bit::get(relbb.m_pieces, sq))
                break;
            if (Bit::get(mask, sq))
                r_moves.push_back(Move(start, sq));
            if (Bit::get(relbb.t_pieces, sq))
                break;
        }
    }
}

void get_legal_moves(Position& pos, std::vector<Move>& r_moves, ull& r_attacks) {
    RelativeBB relbb = pos.relative_bb(pos.turn);
    ull attacked, checkers, pinned;
    board_info(!pos.turn, relbb.swap_sides(), attacked, checkers, pinned);
    r_attacks = attacked;
    const int num_checkers = Bit::popcnt(checkers);
    const int kpos = Bit::first(*relbb.mk);
    const int kx = kpos % 8, ky = kpos / 8;

    // King moves
    get_king_moves(relbb, kx, ky, ~attacked & ~relbb.m_pieces, r_moves);

    if (num_checkers >= 2) {
        // Double check, only king moves.
        return;
    }

    // Calculate push and capture mask if 1 check.
    ull capture_mask = 0xffffffffffffffff;
    ull push_mask = 0xffffffffffffffff;
    ull all_mask = 0xffffffffffffffff;
    if (num_checkers == 1) {
        if (checkers & (*relbb.tb | *relbb.tr | *relbb.tq)) {
            const int checker_pos = Bit::first(checkers);
            const int checker_x = checker_pos % 8, checker_y = checker_pos / 8;
            const int dx = (checker_x == kx ? 0 : (checker_x > kx ? 1 : -1)),
                      dy = (checker_y == ky ? 0 : (checker_y > ky ? 1 : -1));
            push_mask = bb_sequence(kpos, dx, dy, checkers, false, false);
        } else {
            push_mask = 0;
        }
        capture_mask = checkers;
        all_mask = push_mask | capture_mask;
    }
    all_mask &= ~relbb.m_pieces;

    // EP capture mask, destination square of pawn for EP capture evade check.
    ull ep_capture_mask = 0;
    if (pos.ep != -1) {
        const int pawn_dir = pos.turn ? 1 : -1;
        const int opp_pawn_sq = pos.ep - 8*pawn_dir;
        if (Bit::get(checkers, opp_pawn_sq))
            ep_capture_mask = Bit::mask(pos.ep);
    }

    // Other pieces
    for (int sq = 0; sq < 64; sq++) {
        if (!Bit::get(relbb.m_pieces, sq))
            continue;

        const int x = sq % 8, y = sq / 8;

        // Knight
        if (Bit::get(*relbb.mn, sq)) {
            if (!Bit::get(pinned, sq))
                get_knight_moves(relbb, x, y, all_mask, r_moves);
            continue;
        }

        // Pin mask
        ull pin_mask = 0xffffffffffffffff;
        if (Bit::get(pinned, sq)) {
            const int dx = x == kx ? 0 : (x > kx ? 1 : -1),
                      dy = y == ky ? 0 : (y > ky ? 1 : -1);
            pin_mask = bb_sequence(kpos, dx, dy, relbb.t_pieces, false, true);
        }
        const ull sliding_mask = all_mask & pin_mask & ~relbb.m_pieces;
        const ull pawn_mask = (all_mask | ep_capture_mask) & pin_mask & ~relbb.m_pieces;

        // Pawn
        if (Bit::get(*relbb.mp, sq))
            get_pawn_moves(relbb, x, y, pos.turn, kpos, pawn_mask, pos.ep, r_moves);

        // Sliding
        if (Bit::get(*relbb.mb, sq) || Bit::get(*relbb.mq, sq))
            get_sliding_moves(relbb, x, y, BISHOP_OFFSETS, sliding_mask, r_moves);
        if (Bit::get(*relbb.mr, sq) || Bit::get(*relbb.mq, sq))
            get_sliding_moves(relbb, x, y, ROOK_OFFSETS, sliding_mask, r_moves);
    }

    // Castling
    if (num_checkers == 0) {
        const ull castle_attacked = attacked & ~Bit::mask(square(1, 0)) & ~Bit::mask(square(1, 7));
        const ull castle_danger = (relbb.a_pieces | castle_attacked) & ~Bit::mask(kpos);
        if (pos.turn) {
            if (pos.castling & CASTLE_K && !(castle_danger & CASTLE_SQS_K))
                r_moves.push_back(Move(square(4, 0), square(6, 0)));
            if (pos.castling & CASTLE_Q && !(castle_danger & CASTLE_SQS_Q))
                r_moves.push_back(Move(square(4, 0), square(2, 0)));
        } else {
            if (pos.castling & CASTLE_k && !(castle_danger & CASTLE_SQS_k))
                r_moves.push_back(Move(square(4, 7), square(6, 7)));
            if (pos.castling & CASTLE_q && !(castle_danger & CASTLE_SQS_q))
                r_moves.push_back(Move(square(4, 7), square(2, 7)));
        }
    }
}


}  // namespace Movegen
