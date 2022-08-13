#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "sfutils.hpp"


/**
 * Map of std::string key to string value, e.g. depth="5"
 */
class SearchResult {
public:
    std::map<std::string, std::string> data;

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
            str += value;
            str += " ";
        }
        return str;
    }
};


/**
 * Has base (first word, e.g. "position"), and map of key to int value, e.g. movetime 1000.
 * Also "Position" attr, only set if it's a position command.
 */
class UCICommand {
public:
    std::string mode;
    std::map<std::string, int> args;
    Position pos;

    UCICommand(std::istream& is) {
        std::string line;
        if (!std::getline(is, line))
            mode = "quit";

        std::istringstream iss(line);
        std::string word;

        if (std::getline(iss, word, ' '))
            mode = word;
        else
            return;

        if (mode == "position") {
            // Set up position.
            std::getline(iss, word, ' ');
            if (word == "startpos") {
                pos.setup_std();
            } else if (word == "fen") {
                std::string fen;
                for (int i = 0; i < 6; i++) {
                    std::getline(iss, word, ' ');
                    fen += word + " ";
                }
                pos.setup_fen(fen);
            }

            if (std::getline(iss, word, ' ') && word == "moves") {
                while (std::getline(iss, word, ' ')) {
                    pos.push(Move(word));
                }
            }
        } else {
            // Other args.
            while (std::getline(iss, word, ' ')) {
                std::string word2;
                if (std::getline(iss, word2, ' ')) {
                    args[word] = std::stoi(word2);
                } else {
                    args[word] = 1;
                }
            }
        }
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
