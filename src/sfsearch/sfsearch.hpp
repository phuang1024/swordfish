#pragma once

#include <iostream>
#include <map>
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

    UCICommand(std::istream& is);
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
        char depth;  // Depth of search.
        int eval;
        Move best_move;

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
        int used;

        ~TPTable() {
            delete[] table;
        }

        TPTable(int size = 1e6 + 3) {
            this->size = size;
            used = 0;
            table = new TP[size];
        }

        TP* get(ull hash) {
            return &table[hash % size];
        }

        void set(ull hash, const Position& pos, char depth, int eval) {
            // TODO currently best_move is not set.
            TP* tp = get(hash);
            if (tp->depth == -1)
                used++;
            tp->pos = pos;
            tp->depth = depth;
            tp->eval = eval;
        }

        /**
         * UCI hashfull value.
         */
        int get_hashfull() {
            return 1000ULL * used / size;
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
    Move search(Position& pos, int maxdepth, int movetime);

    /**
     * Computes move time from UCI args, e.g. wtime
     */
    int get_movetime(const Position& pos, std::map<std::string, int>& args);
}
