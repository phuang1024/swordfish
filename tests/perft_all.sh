#!/bin/bash

# Does perft test on positions from here:
# https://www.chessprogramming.org/Perft_Results

set -e

python perft.py --swordfish ../build/swordfish --stockfish $STOCKFISH --maxdepth 5 --fen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
