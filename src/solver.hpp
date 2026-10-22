#pragma once

#include <limits>
#include <vector>

#include "types.hpp"

constexpr Value NEG_INF = std::numeric_limits<Value>::min();
constexpr int BRUTE_FORCE_MAX_BALLS = 20;

using SolverFunction = Value (*)(const std::vector<Ball>&, Query);

Value SolveOptimized(const std::vector<Ball>& balls, Query query);
Value SolveReference(const std::vector<Ball>& balls, Query query);
Value SolveBruteForce(const std::vector<Ball>& balls, Query query);

std::vector<Value> SolveAll(const Instance& instance, SolverFunction solver);
