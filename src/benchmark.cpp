#include "benchmark.hpp"

#include <algorithm>
#include <chrono>
#include <numeric>
#include <vector>

TimingResult MeasureMedian(const Instance& instance, SolverFunction solver, int repetitions)
{
    std::vector<double> timings;
    timings.reserve(repetitions);
    Value checksum = 0;

    for (int repetition = 0; repetition < repetitions; ++repetition)
    {
        const auto start = std::chrono::steady_clock::now();
        const std::vector<Value> answers = SolveAll(instance, solver);
        const auto end = std::chrono::steady_clock::now();

        const std::chrono::duration<double, std::milli> elapsed = end - start;
        timings.push_back(elapsed.count());
        checksum = std::accumulate(answers.begin(), answers.end(), Value{0});
    }

    std::sort(timings.begin(), timings.end());
    return {timings[timings.size() / 2], checksum};
}
