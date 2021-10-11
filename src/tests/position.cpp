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

#include "position.hpp"
#include "utils.hpp"
#include "tests.hpp"


namespace Tests {


void bb_attacks() {
    std::cout << "### Test \"bb_attacks\"\n\n";

    Position::Position pos(Position::Position::INIT_START);
    pos.wp = 0;
    pos.ep = 64 + 12;
    const ULL attacks = Position::attacked(pos, true);

    std::cout << "Board:\n";
    Position::print(std::cout, pos);
    std::cout << "\nWhite attacks:\n";
    Position::print(std::cout, attacks);

    std::cout << std::endl;
}


}  // namespace Tests
