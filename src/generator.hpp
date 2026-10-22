#pragma once

#include <cstdint>

#include "types.hpp"

struct GeneratorConfig
{
    int ballCount = 0;
    int colorCount = 1;
    int queryCount = 1;
    Value maxMagnitude = 100000;
    std::uint64_t seed = 1;
};

Instance GenerateInstance(const GeneratorConfig& config);
