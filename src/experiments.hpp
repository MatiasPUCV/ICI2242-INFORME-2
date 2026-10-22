#pragma once

#include <cstdint>
#include <filesystem>

bool RunValidation(int caseCount, std::uint64_t seed);
void RunBallScaling(const std::filesystem::path& outputPath, std::uint64_t seed);
void RunQueryScaling(const std::filesystem::path& outputPath, std::uint64_t seed);
bool RunComparison(const std::filesystem::path& outputPath, std::uint64_t seed);
