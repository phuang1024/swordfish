#pragma once

#include <map>
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
    std::string uci();
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

    UCICommand(std::istream& is);
};

