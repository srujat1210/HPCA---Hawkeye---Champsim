#ifndef RRIP_H
#define RRIP_H

#include <vector>
#include <cstddef>

enum class Classification {
    CACHE_FRIENDLY,
    CACHE_AVERSE
};

// applies update rule to the set's RRPV vector according to the rules given.
void update_rrpv(std::vector<int>& rrpv,
                 std::size_t way,
                 Classification cls,
                 bool is_hit);

// Selects a victim way, aging the set if necessary.
std::size_t find_victim(std::vector<int>& rrpv);

#endif