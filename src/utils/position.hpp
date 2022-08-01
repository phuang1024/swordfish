#pragma once

#include <iostream>

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

    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

