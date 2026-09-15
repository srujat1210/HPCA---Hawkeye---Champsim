# Hawkeye Cache Replacement

Implementation of the Hawkeye cache replacement policy in ChampSim,
based on Jain and Lin, "Back to the Future: Leveraging Belady's
Algorithm for Improved Cache Replacement," ISCA 2016.

## Implementation

The Hawkeye implementation consists of:

- `replacement/hawkeye/optgen.h` and `optgen.cc` — OPTgen
- `replacement/hawkeye/predictor.h` and `predictor.cc` — Hawkeye predictor
- `replacement/hawkeye/rrip.h` and `rrip.cc` — RRIP policy
- `replacement/hawkeye/hawkeye.h` and `hawkeye.cc` — ChampSim integration
- `hawkeye_tests/` — standalone component tests
- `hawkeye_config.json` — ChampSim Hawkeye configuration

Benchmark traces are intentionally excluded from this repository.
