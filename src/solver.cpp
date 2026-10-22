#include "solver.hpp"

#include <algorithm>
#include <stdexcept>

struct ColorBest
{
    Value value = NEG_INF;
    int color = -1;
};

static int CountColors(const std::vector<Ball>& balls)
{
    int maxColor = -1;
    for (const Ball& ball : balls)
    {
        maxColor = std::max(maxColor, ball.color);
    }
    return maxColor + 1;
}

static void UpdateBests(ColorBest& first, ColorBest& second, int color, Value candidate)
{
    if (first.color == color)
    {
        first.value = candidate;
    }
    else if (second.color == color)
    {
        if (candidate > first.value)
        {
            second = first;
            first = {candidate, color};
        }
        else
        {
            second.value = candidate;
        }
    }
    else if (candidate > first.value)
    {
        second = first;
        first = {candidate, color};
    }
    else if (candidate > second.value)
    {
        second = {candidate, color};
    }
}

Value SolveOptimized(const std::vector<Ball>& balls, Query query)
{
    std::vector<Value> bestByColor(CountColors(balls), NEG_INF);
    ColorBest first;
    ColorBest second;

    for (const Ball& ball : balls)
    {
        Value candidate = ball.value * query.b;

        Value sameColor = bestByColor[ball.color];
        if (sameColor != NEG_INF)
        {
            candidate = std::max(candidate, sameColor + ball.value * query.a);
        }

        Value otherColor = first.color != ball.color ? first.value : second.value;
        if (otherColor != NEG_INF)
        {
            candidate = std::max(candidate, otherColor + ball.value * query.b);
        }

        if (candidate > sameColor)
        {
            bestByColor[ball.color] = candidate;
            UpdateBests(first, second, ball.color, candidate);
        }
    }

    return std::max<Value>(0, first.value);
}

Value SolveReference(const std::vector<Ball>& balls, Query query)
{
    std::vector<Value> bestByColor(CountColors(balls), NEG_INF);

    for (const Ball& ball : balls)
    {
        Value candidate = ball.value * query.b;

        Value sameColor = bestByColor[ball.color];
        if (sameColor != NEG_INF)
        {
            candidate = std::max(candidate, sameColor + ball.value * query.a);
        }

        for (int color = 0; color < static_cast<int>(bestByColor.size()); ++color)
        {
            if (color != ball.color && bestByColor[color] != NEG_INF)
            {
                candidate = std::max(candidate, bestByColor[color] + ball.value * query.b);
            }
        }

        bestByColor[ball.color] = std::max(sameColor, candidate);
    }

    Value best = 0;
    for (Value value : bestByColor)
    {
        best = std::max(best, value);
    }
    return best;
}

Value SolveBruteForce(const std::vector<Ball>& balls, Query query)
{
    const int ballCount = static_cast<int>(balls.size());
    if (ballCount > BRUTE_FORCE_MAX_BALLS)
    {
        throw std::invalid_argument("too many balls for brute force");
    }

    Value best = 0;
    const unsigned int subsetCount = 1u << ballCount;

    for (unsigned int mask = 1; mask < subsetCount; ++mask)
    {
        Value total = 0;
        int previousColor = -1;

        for (int index = 0; index < ballCount; ++index)
        {
            if ((mask & (1u << index)) == 0)
            {
                continue;
            }

            const Ball& ball = balls[index];
            const Value multiplier = ball.color == previousColor ? query.a : query.b;
            total += ball.value * multiplier;
            previousColor = ball.color;
        }

        best = std::max(best, total);
    }

    return best;
}

std::vector<Value> SolveAll(const Instance& instance, SolverFunction solver)
{
    std::vector<Value> answers;
    answers.reserve(instance.queries.size());

    for (const Query& query : instance.queries)
    {
        answers.push_back(solver(instance.balls, query));
    }

    return answers;
}
