"""
Visualize the move tree.

Reads output of swordfish debug dump.
"""

import argparse

import pygame

pygame.init()


class Node:
    def __init__(self, parent, pos, static_eval, score):
        self.parent = parent
        self.pos = pos
        self.static_eval = static_eval
        self.score = score
        self.children = []


def read_tree(path):
    # Read nodes as a DFS tree.
    stack = [None]
    nodes_by_depth = [[] for _ in range(100)]

    with open(path, "r") as f:
        lines = f.readlines()

        for line in lines:
            parts = line.split(";")

            if line.startswith("ENTER"):
                node = Node(stack[-1], parts[1], parts[2], None)
                if node.parent is not None:
                    node.parent.children.append(node)

                stack.append(node)
                nodes_by_depth[len(stack) - 1].append(node)

            elif line.startswith("EXIT"):
                stack[-1].score = parts[1]
                stack.pop()

    # Remove empty depth levels
    nodes_by_depth = [nodes for nodes in nodes_by_depth if nodes]

    # Create list of same shape, where each element is a int, meaning
    # the index of the node in the previous depth level that is the parent.
    parents = [[] for _ in range(len(nodes_by_depth))]
    parents[0] = [None]
    for d in range(len(nodes_by_depth) - 1):
        for i in range(len(nodes_by_depth[d])):
            parents[d + 1].extend([i] * len(nodes_by_depth[d][i].children))

    return nodes_by_depth, parents


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("file", help="Path to engine debug dump.")
    args = parser.parse_args()

    nodes, parents = read_tree(args.file)


if __name__ == "__main__":
    main()
