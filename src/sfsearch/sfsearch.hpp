#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "sfutils.hpp"


/**
 * Map of std::string key to string value, e.g. depth="5"
 */
class SearchResult {
public:
    std::map<std::string, std::string> data;

    /**
     * UCI representation of data.
     * e.g.
     * info depth 5 ...
     */
    inline std::string uci() {
        std::string str = "info ";
        for (const auto& [key, value]: data) {
            str += key;
            str += " ";
            str += value;
            str += " ";
        }
        return str;
    }
};


/**
 * Has base (first word, e.g. "position"), and map of key to int value, e.g. movetime 1000.
 * Also "Position" attr, only set if it's a position command.
 * TODO maybe this should be moved to a new library
 */
class UCICommand {
public:
    std::string mode;
    std::map<std::string, int> args;
    Position pos;

    UCICommand(std::istream& is) {
        std::string line;
        if (!std::getline(is, line))
            mode = "quit";

        std::istringstream iss(line);
        std::string word;

        if (std::getline(iss, word, ' '))
            mode = word;
        else
            return;

        if (mode == "position") {
            // Set up position.
            std::getline(iss, word, ' ');
            if (word == "startpos") {
                pos.setup_std();
            } else if (word == "fen") {
                std::string fen;
                for (int i = 0; i < 6; i++) {
                    std::getline(iss, word, ' ');
                    fen += word + " ";
                }
                pos.setup_fen(fen);
            }

            if (std::getline(iss, word, ' ') && word == "moves") {
                while (std::getline(iss, word, ' ')) {
                    const Move m(word);
                    pos.push(m);
                }
            }
        } else {
            // Other args.
            while (std::getline(iss, word, ' ')) {
                std::string word2;
                if (std::getline(iss, word2, ' ')) {
                    args[word] = std::stoi(word2);
                } else {
                    args[word] = 1;
                }
            }
        }
    }
};


/**
 * Call Transposition::init() before using.
 */
namespace Transposition {
    static bool inited = false;
    static ull HASH_PIECES[12][64],
               HASH_CASTLE[16],
               HASH_EP[8],
               HASH_TURN[2];

    /**
     * Transposition entry.
     */
    struct TP {
        Position pos;
        char depth;
        int score;
        Move bestmove;

        TP() {
            // This means unitialized
            depth = -1;
        }
    };

    /**
     * Transposition table.
     */
    struct TPTable {
        TP* table;
        int size;

        ~TPTable() {
            delete[] table;
        }

        TPTable(int size = 1e6 + 1) {
            this->size = size;
            table = new TP[size];
        }

        TP* get(ull hash) {
            return &table[hash % size];
        }
    };

    inline void init() {
        if (!inited) {
            // Set hash bits.
            for (int i = 0; i < 12; i++)
                for (int j = 0; j < 64; j++)
                    HASH_PIECES[i][j] = Random::randull();
            for (int i = 0; i < 16; i++)
                HASH_CASTLE[i] = Random::randull();
            for (int i = 0; i < 8; i++)
                HASH_EP[i] = Random::randull();
            for (int i = 0; i < 2; i++)
                HASH_TURN[i] = Random::randull();
        }

        inited = true;
    }

    inline ull hash(const Position& pos) {
        ull digest = 0;

        for (int sq = 0; sq < 64; sq++) {
            const int piece = pos.piece_at(sq);
            if (piece != EMPTY) {
                digest ^= HASH_PIECES[piece-1][sq];
            }
        }
        digest ^= HASH_CASTLE[pos.castling];
        digest ^= HASH_EP[pos.ep % 8];
        digest ^= HASH_TURN[pos.turn];
        return digest;
    }
}


/**
 * Move generation performance test.
 */
namespace Search {
    /**
     * nodes: Number of leaf nodes.
     */
    SearchResult perft(Position& pos, int depth);

    /**
     * Minimax.
     * pv: Bestmove.
     */
    Move search(Position& pos, int maxdepth);
}
