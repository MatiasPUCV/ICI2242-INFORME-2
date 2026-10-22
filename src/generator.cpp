#include "generator.hpp"

#include <random>

Instance GenerateInstance(const GeneratorConfig& config)
{
    std::mt19937_64 engine(config.seed);
    std::uniform_int_distribution<int> colorDistribution(0, config.colorCount - 1);
    std::uniform_int_distribution<Value> valueDistribution(-config.maxMagnitude, config.maxMagnitude);

    Instance instance;
    instance.balls.reserve(config.ballCount);
    instance.queries.reserve(config.queryCount);

    for (int index = 0; index < config.ballCount; ++index)
    {
        instance.balls.push_back({colorDistribution(engine), valueDistribution(engine)});
    }

    for (int index = 0; index < config.queryCount; ++index)
    {
        instance.queries.push_back({valueDistribution(engine), valueDistribution(engine)});
    }

    return instance;
}
