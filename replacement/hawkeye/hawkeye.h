#ifndef HAWKEYE_H
#define HAWKEYE_H

#include <vector>
#include <unordered_map>
#include "cache.h"
#include "modules.h"
#include "optgen.h"
#include "predictor.h"
#include "rrip.h"

struct hawkeye : public champsim::modules::replacement
{
private:
    

    OPTgen optgen;
    HawkeyePredictor predictor;

    // One RRPV vector per set
    std::vector<std::vector<int>> rrpv;
    //last_pc map: for section 3.2 paragaprh 3: If OPTgen determines that a line X would be a cache hit under the OPT policy, then the PC that last
    // accessed X is trained positively; otherwise, the PC that last
    // accessed X is trained negatively.
    std::vector<std::unordered_map<uint64_t, uint64_t>> last_pc;

public:
    explicit hawkeye(CACHE* cache); //Normal champsim constructor
    hawkeye(CACHE* cache, long sets, long ways);

    long find_victim(uint32_t triggering_cpu, //when a cache miss occurs, this function is called to find a victim line to evict
    uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip, champsim::address full_addr, access_type type);

    void replacement_cache_fill(uint32_t triggering_cpu, //this is called when a new line is inserted into cache
    long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr, access_type type);

    void update_replacement_state(uint32_t triggering_cpu, //this is called accesses to a cache line, either a hit or a miss
    long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr, access_type type, uint8_t hit);
};

#endif