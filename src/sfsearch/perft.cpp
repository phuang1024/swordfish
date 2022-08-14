#include <vector>

#include "sfmovegen.hpp"
#include "sfsearch.hpp"
#include "sfutils.hpp"


namespace Perft {


int perft_run(Position& pos, int depth, bool print_each_move = false) {
    if (depth <= 0)
        return 1;

    std::vector<Move> moves;
    Movegen::get_legal_moves(pos, moves);

    if (depth == 1) {
        // Print out nodes for each move
        if (print_each_move) {
            for (int i = 0; i < (int)moves.size(); i++) {
                const Move& move = moves[i];
                SearchResult res;
                res.data["currmove"] = move.uci();
                res.data["currmovenumber"] = std::to_string(i + 1);
                res.data["nodes"] = "1";
                std::cout << res.uci() << std::endl;
            }
        }

        return moves.size();
    }

    ull nodes = 0;
    for (int i = 0; i < (int)moves.size(); i++) {
        const Move& move = moves[i];

        Position new_pos = pos;
        new_pos.push(move);
        const ull curr_nodes = perft_run(new_pos, depth-1);
        nodes += curr_nodes;

        if (print_each_move) {
            SearchResult res;
            res.data["currmove"] = move.uci();
            res.data["currmovenumber"] = std::to_string(i + 1);
            res.data["nodes"] = std::to_string(curr_nodes);
            std::cout << res.uci() << std::endl;
        }
    }
    return nodes;
}

SearchResult perft(Position& pos, int depth) {
    const ull time_start = Time::time();

    const ull nodes = perft_run(pos, depth, true);

    const ull elapse = Time::elapse(time_start);
    SearchResult res;
    res.data["depth"] = std::to_string(depth);
    res.data["nodes"] = std::to_string(nodes);
    res.data["nps"] = std::to_string(Time::nps(nodes, elapse));
    res.data["time"] = std::to_string(elapse);
    return res;
}


}
