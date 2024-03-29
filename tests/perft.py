"""
Tests perft results against Stockfish.
Finds where an error occurs.
"""

import argparse
import time
import sys
from subprocess import Popen, STDOUT, PIPE
from collections import namedtuple

import chess
from colorama import Fore


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

    proc.stdin.write(f"position fen {fen}\n".encode())
    proc.stdin.write(f"go perft {depth}\n".encode())
    proc.stdin.flush()
    proc.stdin.close()
    start = time.time()
    proc.wait()
    if proc.returncode != 0:
        print(f"Executable {exe} failed:")
        print(proc.stdout.read().decode())
        raise ValueError

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


def debug_wrong(sword_exe, stock_exe, fen, depth):
    print(f"Wrong result at fen {fen}, depth {depth}")
    sword = read_swordfish(sword_exe, fen, depth)
    stock = read_stockfish(stock_exe, fen, depth)

    if set(sword.submoves.keys()) != set(stock.submoves.keys()):
        return fen
    else:
        for sub in sword.submoves:
            if sword.submoves[sub] != stock.submoves[sub]:
                board = chess.Board(fen)
                board.push_uci(sub)
                return debug_wrong(sword_exe, stock_exe, board.fen(), depth-1)

    raise ValueError("Unknown error.")

def print_move_table(sword_exe, stock_exe, fen):
    sword = read_swordfish(sword_exe, fen, 1).submoves.keys()
    stock = read_stockfish(stock_exe, fen, 1).submoves.keys()

    moves = set(sword) | set(stock)
    for move in moves:
        good = move in sword and move in stock
        sys.stdout.write(Fore.GREEN if good else Fore.RED)
        print(move, end="   ")
        if good:
            print("Correct")
        else:
            if move in sword:
                print("Incorrect")
            else:
                print("Missing")


def main():
    parser = argparse.ArgumentParser(description="Perft test")
    parser.add_argument("--swordfish", help="Path to the swordfish binary")
    parser.add_argument("--stockfish", help="Path to the stockfish binary")
    parser.add_argument("--maxdepth", type=int, default=5)
    parser.add_argument("--fen", type=str, required=True)
    args = parser.parse_args()

    print("Move generation accuracy and performance test:")
    print("- Position:", args.fen)
    print("- Max test depth:", args.maxdepth)
    print()

    for depth in range(1, args.maxdepth+1):
        sword = read_swordfish(args.swordfish, args.fen, depth)
        stock = read_stockfish(args.stockfish, args.fen, depth)
        if (sword.nodes == stock.nodes):
            sys.stdout.write(Fore.GREEN)
            print(f"- Depth {depth}: nodes={sword.nodes}, stockfish_nps={stock.nps/1e6 :.3f}M, swordfish_nps={sword.nps/1e6 :.3f}M")
        else:
            sys.stdout.write(Fore.RED)
            print(f"- Depth {depth}: Incorrect: swordfish_nodes={sword.nodes}, stockfish_nodes={stock.nodes}")

            sys.stdout.write(Fore.RESET)
            print("Finding wrong position...")
            wrong = debug_wrong(args.swordfish, args.stockfish, args.fen, depth)
            print("Wrong position:", wrong)
            print(chess.Board(wrong))
            print()
            print_move_table(args.swordfish, args.stockfish, wrong)
            return 1

    sys.stdout.write(Fore.BLUE)
    print("Test passed.")
    print()
    return 0


if __name__ == "__main__":
    ret = main()
    sys.stdout.write(Fore.RESET)
    sys.exit(ret)
