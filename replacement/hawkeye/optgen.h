#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

class OPTgen {
public:
    OPTgen(std::size_t num_sets, std::size_t associativity, std::size_t history_multiplier = 8);

    bool access(std::size_t set_idx, uint64_t address);

private:
    // System Cache configuration
    
    std::size_t associativity_;
    std::size_t history_size_;
    // History size is history_multiplier * associativity


    // Logical time per set
    std::vector<std::size_t> current_time_; //1D vector indexed by set_idx

    // Occupancy vector per set, each of size history_size_
    // 2D vector indexed by [set_idx][current_slot]
    // current_slot = t % H : i.e physical slot inside the fixed size occupancy vector for set_idx
    // Each entry represents the number of cache lines whose liveness intervals overlap that time position in set_idx.
    std::vector<std::vector<std::size_t>> occupancy_vector_;

    // Last access time of each address, also per set
    // last_access_[set_idx][address] = absolute/logical time of previous access of address in set_idx
    //vector of unordered_maps
    std::vector<std::unordered_map<uint64_t, std::size_t>> last_access_;
};