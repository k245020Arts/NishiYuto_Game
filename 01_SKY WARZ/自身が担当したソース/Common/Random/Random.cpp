#include "Random.h"

namespace {
    unsigned int seed_=0;
    std::mt19937 engine_;
}

void Random::Init(const unsigned int seed)
{
	seed_ = seed;
	engine_.seed(seed_);
}

const double Random::GetReal()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(engine_);
}

const int Random::GetInt(const int min, const int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine_);
}

const float Random::GetFloat(const float min, const float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(engine_);
}

const int Random::GetWeightedIndex(const std::vector<double>& weights)
{
    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return dist(engine_);
}

const double Random::GetNormal(const double mean, const double stddev)
{
    std::normal_distribution<double> dist(mean, stddev);
    return dist(engine_);
}

const bool Random::GetBernoulli(const double p)
{
    std::bernoulli_distribution dist(p);
    return dist(engine_);
}

const double Random::GetExponential(const double lambda)
{
    std::exponential_distribution<double> dist(lambda);
    return dist(engine_);
}
const unsigned int Random::GetSeed()
{
    return seed_;
}
