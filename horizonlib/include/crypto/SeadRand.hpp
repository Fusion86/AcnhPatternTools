#pragma once

#include <array>
#include <cstring>
#include <stdint.h>

class SeadRand {
  private:
    std::array<uint32_t, 4> mState;

  public:
    SeadRand(uint32_t seed = 0);
    SeadRand(std::array<uint32_t, 4> state);

    uint32_t getU32();
    uint64_t getU64();
    std::array<uint32_t, 4> getContext();
};
