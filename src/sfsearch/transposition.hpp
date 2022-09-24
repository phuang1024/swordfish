#include <cstring>


/**
 * Call Transposition::init() before using.
 */
namespace Transposition {
    /**
     * Transposition entry type.
     */
    enum TPType {
        // TP entry is empty.
        NO_INFO = 0,
        // Quiescence node.
        QUIESCE = 1,
        // Beta cut.
        FAIL_HIGH = 2,
        // Did not improve alpha.
        FAIL_LOW = 3,
        // Normal node, score improved alpha but no beta cut.
        EXACT = 4,
    };

    /**
     * Transposition entry.
     */
    struct TP {
        TPType type;
        uint64_t hash;
        // Depth of search
        int8_t depth;
        int16_t eval;
        // Search index specific to this.
        int16_t search_index;
        uint8_t move_count;
        // for (i: 0 to num_moves): search(moves[move_order[i]])
        // At most 64 moves.
        int8_t move_order[64];

        TP() {
            type = TPType::NO_INFO;
            depth = -1;
            search_index = -1;
            move_order[0] = -1;
        }
    };

    /**
     * Transposition table.
     */
    class TPTable {
    public:
        // Which search we are currently doing e.g. 1st, 2nd, 3rd, etc.
        int16_t search_index;

        ~TPTable() {
            delete[] table;
        }

        TPTable(int size = 1e7 + 3) {
            this->size = size;
            used = 0;
            table = new TP[size];
            search_index = 0;

            init_hash();
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

        inline TP* get(ull hash) {
            return &table[hash % size];
        }

        inline void set(TPType type, ull hash, char depth, int eval, int move_count, uint8_t* move_order) {
            TP* tp = get(hash);
            if (tp->type == TPType::NO_INFO)
                used++;

            tp->type = type;
            tp->hash = hash;
            tp->depth = depth;
            tp->eval = eval;
            tp->search_index = search_index;

            // Set move order array
            tp->move_count = move_count;
            if (move_order != nullptr) {
                memcpy(tp->move_order, move_order, move_count);
            } else {
                tp->move_order[0] = -1;
            }
        }

        /**
         * UCI hashfull value.
         */
        inline int get_hashfull() {
            return 1000ULL * used / size;
        }

    private:
        TP* table;
        int size;
        int used;
        ull HASH_PIECES[12][64],
            HASH_CASTLE[16],
            HASH_EP[8],
            HASH_TURN[2];

        void init_hash() {
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
    };
}
