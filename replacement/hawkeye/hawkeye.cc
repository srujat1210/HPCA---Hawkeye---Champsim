#include "hawkeye.h"
#include "cache.h"
#include <algorithm>

hawkeye::hawkeye(CACHE* cache)
    : hawkeye(cache, cache->NUM_SET, cache->NUM_WAY)
{}


hawkeye::hawkeye(CACHE* cache, long sets, long ways)
    : replacement(cache),
      

      optgen( sets, ways),

      predictor(),

      // RRIP uses values 0 to 7.
      // Initially every way is set as cache averse, so RRPV = 7
      rrpv(sets,std::vector<int>(ways,7)),

      // One address : previous-PC map for every set
      last_pc((sets))
{}


long hawkeye::find_victim(uint32_t triggering_cpu,uint64_t instr_id,long set,const champsim::cache_block* current_set,champsim::address ip,champsim::address full_addr,access_type type)
{
    return ::find_victim(rrpv[set]);
}

void hawkeye::replacement_cache_fill(uint32_t triggering_cpu,long set,long way,champsim::address full_addr,champsim::address ip,champsim::address victim_addr,access_type type)
{
    uint64_t pc = ip.to<uint64_t>();

    Classification cls = predictor.predict(pc)? Classification::CACHE_FRIENDLY: Classification::CACHE_AVERSE;

    update_rrpv(rrpv[set],way,cls,false);
}
void hawkeye::update_replacement_state(uint32_t triggering_cpu,long set,long way,champsim::address full_addr,champsim::address ip,champsim::address victim_addr,access_type type,
    uint8_t hit)
{
   
    

    std::size_t set_idx = static_cast<std::size_t>(set);

    uint64_t curr_pc = ip.to<uint64_t>();

    // Cache block address (64-byte blocks)
    uint64_t block_addr = full_addr.to<uint64_t>() >> 6;

    auto it = last_pc[set_idx].find(block_addr);

    bool had_previous_access = (it != last_pc[set_idx].end());

    uint64_t previous_pc = 0;

    if (had_previous_access)
        previous_pc = it->second;

    bool opt_hit = optgen.access(set_idx, block_addr);

    // Train the PC responsible for the previous reuse interval.
    if (had_previous_access)
        predictor.train(previous_pc, opt_hit);

    // Current access becomes the previous access for next time.
    last_pc[set_idx][block_addr] = curr_pc;

    // Miss insertion is handled by replacement_cache_fill().
    if (hit)
    {
        Classification cls =predictor.predict(curr_pc)? Classification::CACHE_FRIENDLY: Classification::CACHE_AVERSE;

        update_rrpv(rrpv[set_idx],way,cls,true);
    }
}