#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "experiments.hpp"
#include "solver.hpp"
#include "types.hpp"

constexpr int DEFAULT_VALIDATION_CASES = 2000;
constexpr std::uint64_t DEFAULT_SEED = 1;

static std::string GetOption(const std::vector<std::string>& arguments, const std::string& name, const std::string& fallback)
{
    for (std::size_t index = 0; index + 1 < arguments.size(); ++index)
    {
        if (arguments[index] == name)
        {
            return arguments[index + 1];
        }
    }
    return fallback;
}

static Instance ReadInstance(std::istream& input)
{
    int ballCount = 0;
    int queryCount = 0;
    input >> ballCount >> queryCount;

    Instance instance;
    instance.balls.resize(ballCount);
    instance.queries.resize(queryCount);

    for (Ball& ball : instance.balls)
    {
        input >> ball.value;
    }
    for (Ball& ball : instance.balls)
    {
        input >> ball.color;
    }
    for (Query& query : instance.queries)
    {
        input >> query.a >> query.b;
    }

    return instance;
}

static int RunSolve()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const Instance instance = ReadInstance(std::cin);
    for (Value answer : SolveAll(instance, SolveOptimized))
    {
        std::cout << answer << '\n';
    }
    return 0;
}

static void PrintUsage()
{
    std::cout << "usage: choosing_balls <command> [options]\n"
              << "commands:\n"
              << "  solve            read an instance from stdin and print one answer per query\n"
              << "  validate         compare against brute force and reference (--cases N)\n"
              << "  scale-balls      time over increasing N (--output FILE)\n"
              << "  scale-queries    time over increasing Q (--output FILE)\n"
              << "  compare          optimized vs reference (--output FILE)\n"
              << "  all              run every experiment (--output-dir DIR)\n"
              << "common options: --seed N\n";
}

int main(int argc, char* argv[])
{
    const std::vector<std::string> arguments(argv + 1, argv + argc);
    if (arguments.empty())
    {
        PrintUsage();
        return 1;
    }

    const std::string& command = arguments[0];
    const std::uint64_t seed = std::stoull(GetOption(arguments, "--seed", std::to_string(DEFAULT_SEED)));
    const std::filesystem::path outputDirectory = GetOption(arguments, "--output-dir", "results");

    if (command == "solve")
    {
        return RunSolve();
    }

    if (command == "validate")
    {
        const int caseCount = std::stoi(GetOption(arguments, "--cases", std::to_string(DEFAULT_VALIDATION_CASES)));
        return RunValidation(caseCount, seed) ? 0 : 1;
    }

    if (command == "scale-balls")
    {
        RunBallScaling(GetOption(arguments, "--output", (outputDirectory / "scale_balls.csv").string()), seed);
        return 0;
    }

    if (command == "scale-queries")
    {
        RunQueryScaling(GetOption(arguments, "--output", (outputDirectory / "scale_queries.csv").string()), seed);
        return 0;
    }

    if (command == "compare")
    {
        return RunComparison(GetOption(arguments, "--output", (outputDirectory / "compare.csv").string()), seed) ? 0 : 1;
    }

    if (command == "all")
    {
        const bool validationPassed = RunValidation(DEFAULT_VALIDATION_CASES, seed);
        RunBallScaling(outputDirectory / "scale_balls.csv", seed);
        RunQueryScaling(outputDirectory / "scale_queries.csv", seed);
        const bool comparisonPassed = RunComparison(outputDirectory / "compare.csv", seed);
        return validationPassed && comparisonPassed ? 0 : 1;
    }

    PrintUsage();
    return 1;
}
