#
#  Swordfish
#  A chess engine.
#  Copyright Patrick Huang 2021
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

PY = /usr/bin/env python3
CMAKE_FLAGS = -G"Unix Makefiles"

.PHONY: release debug deps docs

release:
	mkdir -p ./build; \
	cd ./build; \
	cmake ../src -DCMAKE_BUILD_TYPE=Release $(CMAKE_FLAGS); \
	make -j`nproc`

debug: deps
	mkdir -p ./build; \
	cd ./build; \
	cmake ../src -DCMAKE_BUILD_TYPE=Debug -DWITH_TESTING=ON $(CMAKE_FLAGS); \
	make -j`nproc`

deps:
	$(PY) ./scripts/deps.py

docs:
	cd ./docs; \
	make html
