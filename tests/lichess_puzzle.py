# Downloads and solves https://lichess.org daily puzzle.

import os

import chess
import chess.pgn
import chess.engine
import requests
import termcolor

ROOT = os.path.dirname(os.path.realpath(__file__))
ENGINE_PATH = os.path.join(ROOT, "..", "build", "swordfish")
ENDPOINT = "https://lichess.org/api/puzzle/daily"
LIMIT = chess.engine.Limit(time=6)


def get_puzzle():
    print("Getting puzzle: ", flush=True, end="")

    data = requests.get(ENDPOINT).json()
    board = chess.Board()
    for move in data["game"]["pgn"].split():
        board.push_san(move)

    solution = [chess.Move.from_uci(uci) for uci in data["puzzle"]["solution"]]

    print("Done")
    return (board, solution)


def solve_puzzle(board, solution):
    engine = chess.engine.SimpleEngine.popen_uci(ENGINE_PATH)
    print(f"Solving puzzle ({len(solution)} moves)")
    print(f"Starting position is {board.fen()}")

    for i, move in enumerate(solution):
        if i % 2 == 1:  # Only solve first side moves.
            board.push(move)
            continue

        print(termcolor.colored("", "white"), end="")
        print(f"Move {i+1}: ", end="", flush=True)
        result = engine.play(board, LIMIT, info=chess.engine.INFO_ALL)
        if result.move == move:
            board.push(move)
            print(termcolor.colored("Correct: move={}, eval={}, depth={}, nodes={}".format(move.uci(),
                result.info["score"].relative, result.info["depth"], result.info["nodes"]), "green"))
        else:
            print(termcolor.colored("Wrong: ans={}, attempt={}".format(move.uci(), result.move.uci()), "red"))
            break

    print(termcolor.colored("", "white"), end="")
    engine.quit()


def main():
    board, solution = get_puzzle()
    solve_puzzle(board, solution)


if __name__ == "__main__":
    main()
