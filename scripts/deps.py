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

import os
import subprocess
import shutil
from typing import Dict

GIT = shutil.which("git")
assert GIT is not None

DEPS = [
    {
        "url": "https://github.com/catchorg/Catch2.git",
        "commit": "c4e3767e265808590986d5db6ca1b5532a7f3d13",
    }
]

PARENT = os.path.dirname(os.path.realpath(__file__))
LIBS_PATH = os.path.join(os.path.dirname(PARENT), "3rdparty")


def setup_lib(lib: Dict[str, str]):
    basename = os.path.basename(lib["url"]).replace(".git", "")
    subprocess.Popen([GIT, "clone", lib["url"]], cwd=LIBS_PATH).wait()
    subprocess.Popen([GIT, "checkout", lib["commit"]], cwd=os.path.join(LIBS_PATH, basename)).wait()


def main():
    os.makedirs(LIBS_PATH, exist_ok=True)
    for dep in DEPS:
        setup_lib(dep)


main()
