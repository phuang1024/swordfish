#include <vector>

#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"


namespace Perft {


int perft_run(Position& pos, int depth) {
    if (depth <= 0)
        return 1;

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    if (depth == 1)
        return moves.size();

    int nodes = 0;
    for (const Move& move: moves) {
        Position new_pos = pos;
        new_pos.push(move);
        nodes += perft_run(new_pos, depth-1);
    }
    return nodes;
}

SearchResult perft(Position& pos, int depth) {
    const ull time_start = Time::time();

    const ull nodes = perft_run(pos, depth);

    SearchResult res;
    res.data["depth"] = depth;
    res.data["nodes"] = nodes;
    res.data["nps"] = Time::nps(nodes, Time::time()-time_start);
    return res;
}


}
