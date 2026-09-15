#include "rrip.h"

void update_rrpv(std::vector<int>& rrpv,
                 std::size_t way,
                 Classification cls,
                 bool is_hit)
{
    // Cache-averse:
    // Whether hit or miss, make it an immediate eviction candidate.
    if (cls == Classification::CACHE_AVERSE) {
        rrpv[way] = 7;
        return;
    }

    // Cache-friendly hit:
    // Give the accessed line highest priority.
    if (is_hit) {
        rrpv[way] = 0;
        return;
    }

    // Cache-friendly miss/insertion:
    //
    // Rules: RRPV = 0
    // age all other lines, then insert new line with highest priority (RRPV = 0).  
    for (std::size_t i = 0; i < rrpv.size(); ++i) {
        if (i != way && rrpv[i] < 6) {
            ++rrpv[i];
        }
    }

    rrpv[way] = 0;
}


std::size_t find_victim(std::vector<int>& rrpv)
{
    while (true) {

        // Prefer any cache-averse / maximum-RRPV line.
        for (std::size_t i = 0; i < rrpv.size(); ++i) {
            if (rrpv[i] == 7) {
                return i;
            }
        }

        // No RRPV = 7 exists.
        // Age the whole set until some line reaches 7.
        for (std::size_t i = 0; i < rrpv.size(); ++i) {
            if (rrpv[i] < 7) {
                ++rrpv[i];
            }
        }
    }
}