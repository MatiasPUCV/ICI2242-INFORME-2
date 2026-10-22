#pragma once

#include "solver.hpp"
#include "types.hpp"

struct TimingResult
{
    double medianMilliseconds;
    Value checksum;
};

TimingResult MeasureMedian(const Instance& instance, SolverFunction solver, int repetitions);
