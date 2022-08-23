#pragma once

#include <iostream>
#include <map>
#include <string>

#include "sfuci.hpp"
#include "sfutils.hpp"


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
        ull hash;
        // Depth of search.
        char depth;
        int eval, alpha, beta;
        uch move_count;
        // 0 <= i < num_moves, search legal_moves[move_order[i]]
        int* move_order;

        ~TP() {
            if (move_order != nullptr)
                delete[] move_order;
        }

        TP() {
            // This means unitialized
            depth = -1;
            move_order = nullptr;
        }
    };

    /**
     * Transposition table.
     */
    class TPTable {
    private:
        TP* table;

    public:
        int size;
        int used;

        ~TPTable() {
            delete[] table;
        }

        TPTable(int size = 1e7 + 3) {
            this->size = size;
            used = 0;
            table = new TP[size];
        }

        TP* get(ull hash) {
            return &table[hash % size];
        }

        /**
         * @param move_order  Same format as TP::move_order.
         *     If nullptr, leaves current move_order.
         */
        void set(ull hash, char depth, int eval, int alpha, int beta, int move_count, int* move_order) {
            TP* tp = get(hash);
            if (tp->depth == -1)
                used++;

            tp->hash = hash;
            tp->depth = depth;
            tp->eval = eval;
            tp->alpha = alpha;
            tp->beta = beta;

            if (move_order != nullptr) {
                if (tp->move_order == nullptr || tp->move_count != move_count) {
                    if (tp->move_order != nullptr)
                        delete[] tp->move_order;
                    tp->move_order = new int[move_count];
                    tp->move_count = move_count;
                    for (int i = 0; i < move_count; i++)
                        tp->move_order[i] = move_order[i];
                }
            }
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
