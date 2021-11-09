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

#include <cstring>
#include <iostream>
#include <stdio.h>

#include "config.hpp"
#include "position.hpp"
#include "uci.hpp"
#include "utils.hpp"


void print_info() {
    printf("Swordfish v%d.%d.%d: A free and open source chess engine.\n", VMAJOR, VMINOR, VPATCH);
    printf("Swordfish is licensed under the GNU GPL v3 license.\n");
}

void test_stuff() {
    Position::Position pos("8/8/8/4n3/8/3K4/8/8 w - - 0 1");
    //pos.meta &= ~Position::TURN;
    std::vector<Position::Move> moves;
    Position::legal_moves(moves, pos);

    Position::print(std::cout, pos);
    std::cout << moves.size() << " moves\n";
    for (int i = 0; i < (int)moves.size(); i++)
        std::cout << moves[i].uci() << std::endl;
}

void loop() {
    bool run = true;

    while (run) {
        std::vector<std::string> cmds;
        if (!Uci::get_cmds(cmds))
            break;
        int num = cmds.size();

        if (num >= 1) {
            if (cmds[0] == "quit") {
                run = false;
            } else if (cmds[0] == "isready") {
                std::cout << "readyok" << std::endl;
            } else if (cmds[0] == "uci") {
                // print uci options
                std::cout << "uciok" << std::endl;
            }
        }
    }

    // Deallocation
}


int main(int argc, char** argv) {
    print_info();
    loop();
}
