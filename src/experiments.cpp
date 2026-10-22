#include "experiments.hpp"

#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

#include "benchmark.hpp"
#include "generator.hpp"
#include "solver.hpp"

constexpr int REPETITIONS = 5;
constexpr int SCALING_QUERY_COUNT = 10;
constexpr int SCALING_BALL_COUNT = 20000;
constexpr int VALIDATION_MAX_BALLS = 14;
constexpr int VALIDATION_MAX_COLORS = 5;
constexpr int VALIDATION_QUERY_COUNT = 5;
constexpr Value VALIDATION_MAGNITUDE = 10;
constexpr Value BENCHMARK_MAGNITUDE = 100000;

constexpr std::array<int, 6> BALL_COUNTS = {1000, 5000, 10000, 25000, 50000, 100000};
constexpr std::array<int, 5> QUERY_COUNTS = {10, 50, 100, 250, 500};
constexpr std::array<int, 6> COMPARISON_BALL_COUNTS = {100, 200, 400, 800, 1600, 3200};

static std::ofstream OpenCsv(const std::filesystem::path& path, const char* header)
{
    if (path.has_parent_path())
    {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream file(path);
    file << header << '\n';
    file << std::fixed << std::setprecision(6);
    return file;
}

static bool CheckInstance(const Instance& instance)
{
    const std::vector<Value> expected = SolveAll(instance, SolveBruteForce);
    const std::vector<Value> optimized = SolveAll(instance, SolveOptimized);
    const std::vector<Value> reference = SolveAll(instance, SolveReference);
    return expected == optimized && expected == reference;
}

bool RunValidation(int caseCount, std::uint64_t seed)
{
    std::mt19937_64 engine(seed);
    std::uniform_int_distribution<int> ballDistribution(1, VALIDATION_MAX_BALLS);
    std::uniform_int_distribution<int> colorDistribution(1, VALIDATION_MAX_COLORS);

    int failures = 0;

    Instance emptyBest;
    emptyBest.balls = {{0, 3}, {1, 2}, {0, 5}};
    emptyBest.queries = {{-1, -1}, {0, 0}, {-4, 2}};
    if (!CheckInstance(emptyBest))
    {
        ++failures;
        std::cout << "FAILED: edge case where the empty sequence is optimal\n";
    }

    for (int caseIndex = 0; caseIndex < caseCount; ++caseIndex)
    {
        const GeneratorConfig config = {
            .ballCount = ballDistribution(engine),
            .colorCount = colorDistribution(engine),
            .queryCount = VALIDATION_QUERY_COUNT,
            .maxMagnitude = VALIDATION_MAGNITUDE,
            .seed = engine(),
        };

        if (!CheckInstance(GenerateInstance(config)))
        {
            ++failures;
            std::cout << "FAILED: case " << caseIndex << " (seed " << config.seed << ")\n";
        }
    }

    std::cout << "validation: " << (caseCount + 1 - failures) << "/" << (caseCount + 1) << " cases passed\n";
    return failures == 0;
}

void RunBallScaling(const std::filesystem::path& outputPath, std::uint64_t seed)
{
    std::ofstream file = OpenCsv(outputPath, "ballCount,queryCount,medianMilliseconds,checksum");

    for (int ballCount : BALL_COUNTS)
    {
        const GeneratorConfig config = {
            .ballCount = ballCount,
            .colorCount = ballCount / 4,
            .queryCount = SCALING_QUERY_COUNT,
            .maxMagnitude = BENCHMARK_MAGNITUDE,
            .seed = seed,
        };

        const Instance instance = GenerateInstance(config);
        const TimingResult result = MeasureMedian(instance, SolveOptimized, REPETITIONS);

        file << ballCount << ',' << SCALING_QUERY_COUNT << ',' << result.medianMilliseconds << ',' << result.checksum << '\n';
        std::cout << "balls=" << ballCount << " queries=" << SCALING_QUERY_COUNT << " ms=" << result.medianMilliseconds << '\n';
    }
}

void RunQueryScaling(const std::filesystem::path& outputPath, std::uint64_t seed)
{
    std::ofstream file = OpenCsv(outputPath, "ballCount,queryCount,medianMilliseconds,checksum");

    for (int queryCount : QUERY_COUNTS)
    {
        const GeneratorConfig config = {
            .ballCount = SCALING_BALL_COUNT,
            .colorCount = SCALING_BALL_COUNT / 4,
            .queryCount = queryCount,
            .maxMagnitude = BENCHMARK_MAGNITUDE,
            .seed = seed,
        };

        const Instance instance = GenerateInstance(config);
        const TimingResult result = MeasureMedian(instance, SolveOptimized, REPETITIONS);

        file << SCALING_BALL_COUNT << ',' << queryCount << ',' << result.medianMilliseconds << ',' << result.checksum << '\n';
        std::cout << "balls=" << SCALING_BALL_COUNT << " queries=" << queryCount << " ms=" << result.medianMilliseconds << '\n';
    }
}

bool RunComparison(const std::filesystem::path& outputPath, std::uint64_t seed)
{
    std::ofstream file = OpenCsv(outputPath, "ballCount,optimizedMilliseconds,referenceMilliseconds,resultsMatch");
    bool allMatch = true;

    for (int ballCount : COMPARISON_BALL_COUNTS)
    {
        const GeneratorConfig config = {
            .ballCount = ballCount,
            .colorCount = ballCount,
            .queryCount = 1,
            .maxMagnitude = BENCHMARK_MAGNITUDE,
            .seed = seed,
        };

        const Instance instance = GenerateInstance(config);
        const TimingResult optimized = MeasureMedian(instance, SolveOptimized, REPETITIONS);
        const TimingResult reference = MeasureMedian(instance, SolveReference, REPETITIONS);
        const bool match = optimized.checksum == reference.checksum;
        allMatch = allMatch && match;

        file << ballCount << ',' << optimized.medianMilliseconds << ',' << reference.medianMilliseconds << ',' << (match ? 1 : 0) << '\n';
        std::cout << "balls=" << ballCount << " optimizedMs=" << optimized.medianMilliseconds
                  << " referenceMs=" << reference.medianMilliseconds << " match=" << match << '\n';
    }

    return allMatch;
}
