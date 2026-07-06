#include "Random.h"

namespace {
    unsigned int seed_=0;
    std::mt19937 engine_;
}

void Random::Init(unsigned int seed)
{
	seed_ = seed;
	engine_.seed(seed_);
}

double Random::GetReal()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(engine_);
}

int Random::GetInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine_);
}

float Random::GetFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(engine_);
}

int Random::GetWeightedIndex(const std::vector<double>& weights)
{
    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return dist(engine_);
}

double Random::GetNormal(double mean, double stddev)
{
    std::normal_distribution<double> dist(mean, stddev);
    return dist(engine_);
}

bool Random::GetBernoulli(double p)
{
    std::bernoulli_distribution dist(p);
    return dist(engine_);
}

double Random::GetExponential(double lambda)
{
    std::exponential_distribution<double> dist(lambda);
    return dist(engine_);
}
unsigned int Random::GetSeed()
{
    return seed_;
}
