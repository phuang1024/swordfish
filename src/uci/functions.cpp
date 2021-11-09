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

/**
 * Helper functions.
 */

#include <iterator>
#include <sstream>
#include <string>

#include "uci.hpp"


namespace Uci {


bool get_cmds(std::vector<std::string>& cmds) {
    cmds.clear();
    std::string input;
    if (!std::getline(std::cin, input))
        return false;

    std::istringstream iss(input);
    std::string word;
    while (std::getline(iss, word, ' ')) {
        cmds.push_back(word);
    }

    return true;
}


}  // namespace Uci
