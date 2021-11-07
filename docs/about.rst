About
=====

Swordfish is a chess playing engine. It communicates through stdin and stdout
using the Universal Chess Interface protocol.

If you would like to use Swordfish through a GUI, please download an external one
such as Cutechess.

Swordfish is designed to be decent at chess, and at the same time, providing
a set of chess libraries which can be used in other projects.

Some components are inspired by Stockfish.

Libraries
---------

Swordfish has a main loop, which calls various functions from sub libraries. These
libraries are designed to be portable.

* ``position``: Chess position representation and move generation.
* ``uci``: UCI chess protocol.
* ``utils``: A header file for common utilities.
