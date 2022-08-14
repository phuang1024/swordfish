"""
Tests perft results against Stockfish.
Finds where an error occurs.
"""

import argparse
import time
from subprocess import Popen, STDOUT, PIPE
from collections import namedtuple

import chess


class PerftResult:
    nps: float
    elapse: float
    submoves: dict

    def __init__(self):
        self.submoves = {}

    def __repr__(self):
        return f"PerftResult(nodes={self.nodes})"


def start_perft(exe, fen, depth):
    proc = Popen(exe, stdin=PIPE, stdout=PIPE, stderr=STDOUT)

    start = time.time()
    proc.stdin.write(f"go perft {depth}\n".encode())
    proc.stdin.flush()
    proc.stdin.close()
    proc.wait()
    if proc.returncode != 0:
        print(f"Executable {exe} failed:")
        print(proc.stdout.read().decode())

    elapse = time.time() - start

    return elapse, proc.stdout.read().decode()

def read_swordfish(exe, fen, depth):
    elapse, data = start_perft(exe, fen, depth)
    res = PerftResult()

    for line in data.split("\n"):
        tokens = line.split()
        if "currmove" in tokens:
            move = tokens[tokens.index("currmove") + 1]
            nodes = int(tokens[tokens.index("nodes") + 1])
            res.submoves[move] = nodes
        elif "depth" in tokens:
            res.nodes = int(tokens[tokens.index("nodes") + 1])

    res.nps = res.nodes / elapse
    return res

def read_stockfish(exe, fen, depth):
    elapse, data = start_perft(exe, fen, depth)
    res = PerftResult()

    for line in data.split("\n"):
        tokens = line.split()
        if "searched" in line:
            res.nodes = int(tokens[-1])
        elif ":" in line:
            move = tokens[0][:-1]
            nodes = int(tokens[-1])
            res.submoves[move] = nodes

    res.nps = res.nodes / elapse
    return res


def main():
    parser = argparse.ArgumentParser(description="Perft test")
    parser.add_argument("--swordfish", help="Path to the swordfish binary")
    parser.add_argument("--stockfish", help="Path to the stockfish binary")
    parser.add_argument("--maxdepth", type=int, default=5)
    parser.add_argument("--fen", type=str, required=True)
    args = parser.parse_args()

    print("Move generation accuracy and performance test:")
    print("- Position:", args.fen)
    print("- Depth:", args.maxdepth)
    print()

    for depth in range(1, args.maxdepth+1):
        sword = read_swordfish(args.swordfish, args.fen, depth)
        stock = read_stockfish(args.stockfish, args.fen, depth)
        if (sword.nodes == stock.nodes):
            print(f"- Depth {depth}: nodes={sword.nodes}, stockfish_nps={stock.nps}, swordfish_nps={sword.nps}")
        else:
            print(f"- Depth {depth}: Incorrect: swordfish_nodes={sword.nodes}, stockfish_nodes={stock.nodes}")
            break


if __name__ == "__main__":
    main()
