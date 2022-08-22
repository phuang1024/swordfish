#include "sfuci.hpp"


std::string SearchResult::uci() {
    std::string str = "info ";
    for (const auto& [key, value]: data) {
        if (key == "pv")   // PV must be last.
            continue;
        str += key;
        str += " ";
        str += value;
        str += " ";
    }
    if (data.count("pv")) {
        str += "pv ";
        str += data["pv"];
    }
    return str;
}
