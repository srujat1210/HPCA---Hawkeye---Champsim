#ifndef HAWKEYE_PREDICTOR_H
#define HAWKEYE_PREDICTOR_H

#include <cstddef>
#include <cstdint>
#include <vector>

class HawkeyePredictor {
public:
    // num_entries: size of the PC-indexed table (paper: 8K entries)
    // counter_bits: width of the saturating counte; Acc to paper: 3 bits, range [0, 2^counter_bits - 1]
    HawkeyePredictor(std::size_t num_entries = 8192, int counter_bits = 3);

    // Trains the counter indexed by a hash of pc.
    // opt_hit = true : increment toward cache-friendly
    // opt_hit = false : decrement toward cache-averse
    void train(uint64_t pc, bool opt_hit);

    // Returns predicted classification for pc:
    // true : cache-friendly
    // false : cache-averse
    bool predict(uint64_t pc) const;

    // Returns raw counter value for debugging/testing.
    int get_counter(uint64_t pc) const;

private:

    // PC-indexed table of saturating counters
    std::vector<int> counters_;

    // Hashes raw PC and converts it to a predictor-table index
    std::size_t get_index(uint64_t pc) const;
};

#endif