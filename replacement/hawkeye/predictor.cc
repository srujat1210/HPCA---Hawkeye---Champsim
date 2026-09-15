#include "predictor.h"

HawkeyePredictor::HawkeyePredictor(std::size_t num_entries, int counter_bits):
      
      counters_(num_entries, 4)
{
}

std::size_t HawkeyePredictor::get_index(uint64_t pc) const
{
    uint64_t hashed_pc = pc ^ (pc >> 12);

    // num_entries is expected to be a power of 2
    return hashed_pc & (counters_.size()- 1);
}

void HawkeyePredictor::train(uint64_t pc, bool opt_hit)
{
    std::size_t index = get_index(pc);

    if (opt_hit) {
        // Train toward cache-friendly
        if (counters_[index] < 7) {
            {counters_[index]++;}
        }
        }
    else {
        // Train toward cache-averse
        if (counters_[index] > 0) {
            {counters_[index]--;}}
        }
}

bool HawkeyePredictor::predict(uint64_t pc) const
{
    std::size_t index = get_index(pc);

    return counters_[index] >= 4;
}

int HawkeyePredictor::get_counter(uint64_t pc) const
{
    std::size_t index = get_index(pc);

    return counters_[index];
}