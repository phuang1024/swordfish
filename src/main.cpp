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

#ifdef WITH_TESTING
    #include "tests.hpp"
#endif


void print_info() {
    printf("Swordfish v%d.%d.%d - Free and open source chess engine.\n", VMAJOR, VMINOR, VPATCH);
    printf("Swordfish is distributed under the GNU GPL v3 license.\n");
}


void loop() {
    print_info();
    printf("\n");
}


int main(int argc, char** argv) {
    if (argc >= 2) {
        if (strcmp(argv[1], "--version") == 0) {
            print_info();
        } else if (strcmp(argv[1], "test") == 0) {
            #ifdef WITH_TESTING
                if (argc >= 3) Tests::test(argv[2]);
                else Tests::testall();
            #else
                std::cerr << "Testing is disabled. Run CMake with -DWITH_TESTING=ON" << std::endl;
                return 1;
            #endif
        }
    } else {
        loop();
    }
}
