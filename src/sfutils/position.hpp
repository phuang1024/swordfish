#pragma once

#include <iostream>

#include "constants.hpp"

using ull = unsigned long long;


/**
 * Chess position using bitboards.
 */
class Position {
public:
    ull wp, wn, wb, wr, wq, wk, bp, bn, bb, br, bq, bk;

    ~Position() {
    }

    Position() {
        wp = wn = wb = wr = wq = wk = bp = bn = bb = br = bq = bk = 0;
    }

    /**
     * Get piece code at position.
     */
    int piece_at(int pos) const {
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

