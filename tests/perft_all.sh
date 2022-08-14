#!/bin/bash

# Does perft test on positions from here:
# https://www.chessprogramming.org/Perft_Results

set -e

# Args: (fen, depth)
function test_pos() {
    python perft.py --swordfish ../build/swordfish --stockfish $STOCKFISH --maxdepth $1 --fen "$2"
}

test_pos 6 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
test_pos 5 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
test_pos 7 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"
test_pos 5 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
test_pos 5 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"
test_pos 5 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
