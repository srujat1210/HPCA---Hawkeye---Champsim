// replacement/hawkeye/optgen.cc

#include "optgen.h"

OPTgen::OPTgen(std::size_t num_sets, std::size_t associativity, std::size_t history_multiplier)
{
    
    associativity_ = associativity;
    history_size_ = history_multiplier * associativity;
    //Create one time counter per set, initialized to 0
    current_time_.resize(num_sets, 0);
    //Create one occupancy vector per set, each of size history_size_, initialized to 0
    occupancy_vector_.resize(
        num_sets,
        std::vector<std::size_t>(history_size_, 0)
    );
    //Create one last_access map per set, initialized to empty
    last_access_.resize(num_sets);
}


bool OPTgen::access(std::size_t set_idx, uint64_t address)
{
    // current logical time for this set
    std::size_t t = current_time_[set_idx];

    // current position in the occupancy vector
    std::size_t current_slot = t % history_size_;

    // this slot was used at time t - H, so clear stale data
    occupancy_vector_[set_idx][current_slot] = 0;

    bool result = false; // assume miss

    // lookup for previous access to this address using address as key
    auto it = last_access_[set_idx].find(address); 

    if (it != last_access_[set_idx].end()) { //if address is found in the map
        std::size_t t_prev = it->second; //get the previous access time i.e. it->second(value)
        // crevious access must still be within our history
        if (t - t_prev < history_size_) {

            bool can_cache = true;

            // check the usage interval [t_prev, t)
            for (std::size_t time = t_prev; time < t; ++time) {
                std::size_t slot = time % history_size_;

                if (occupancy_vector_[set_idx][slot] >= associativity_) {
                    can_cache = false;
                    break;
                }
            }

            if (can_cache) {
                result = true;

                // this liveness interval is admitted, so add it to the occupancy vector
                for (std::size_t time = t_prev; time < t; ++time) {
                    std::size_t slot = time % history_size_;
                    occupancy_vector_[set_idx][slot]++;
                }
            }
        }
    }

    // Current access becomes the most recent access
    last_access_[set_idx][address] = t;

    // Advance logical time for this set
    current_time_[set_idx]++;

    return result;
}