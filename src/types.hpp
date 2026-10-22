#pragma once

#include <cstdint>
#include <vector>

using Value = std::int64_t;

struct Ball
{
    int color;
    Value value;
};

struct Query
{
    Value a;
    Value b;
};

struct Instance
{
    std::vector<Ball> balls;
    std::vector<Query> queries;
};
