//
//  Swordfish
//  A chess engine.
//  Copyright Patrick Huang 2021
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <iostream>
#include <vector>

#include "utils.hpp"


namespace Uci {

// Constants

// What to output in info.output()
constexpr ULL I_DEPTH = 1ULL << 0;
constexpr ULL I_NODES = 1ULL << 1;
constexpr ULL I_NPS   = 1ULL << 2;
constexpr ULL I_TIME  = 1ULL << 3;
constexpr ULL I_NONE  = 0ULL;
constexpr ULL I_ALL   = (1ULL << 32) - 1;

/**
 * Read space separated tokens from std::cin.
 * Store in cmds argument.
 */
bool get_cmds(std::vector<std::string>& cmds);


/**
 * UCI search information.
 */
struct Info {
    int depth;
    int nodes;
    int nps;
    int time;

    /**
     * Print info in UCI protocol to out stream.
     * info depth 21 nodes 29320 nps 0
     * @param what what to print e.g. I_DEPTH | I_TIME | I_NPS
     */
    void output(std::ostream& fp, const ULL what);
};


}  // namespace Uci
