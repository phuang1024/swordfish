#include <iostream>

#include "sfmovegen.hpp"

namespace Movegen {


ull attacked_sqs(const Position& pos, bool side, bool ignore_king) {
    RelativeBB relbb = pos.relative_bb(side);
    ull attacks = 0;

    for (int i = 0; i < 64; i++) {
    }
}


}  // namespace Movegen
