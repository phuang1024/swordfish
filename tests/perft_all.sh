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
