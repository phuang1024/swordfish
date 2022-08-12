#include <map>
#include <string>

#include "sfutils.hpp"


/**
 * Map of std::string key to int value, e.g. depth=5
 */
class SearchResult {
public:
    std::map<std::string, int> data;

    /**
     * UCI representation of data.
     * e.g.
     * info depth 5 ...
     */
    inline std::string uci() {
        std::string str = "info ";
        for (const auto& [key, value]: data) {
            str += key;
            str += " ";
            str += std::to_string(value);
            str += " ";
        }
        return str;
    }
};


/**
 * Move generation performance test.
 */
namespace Perft {
    /**
     * nodes: Number of leaf nodes.
     */
    SearchResult perft(Position& pos, int depth);
}
