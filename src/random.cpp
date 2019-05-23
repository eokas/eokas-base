
#include "random.h"
#include <ctime>
#include <cstdlib>

_BeginNamespace(eokas)

Random::Random()
	:mSeed(0)
{
	mSeed = static_cast<unsigned int>(std::time(nullptr)%1024);
	std::srand(mSeed);
}

Random::~Random()
{}

i32_t Random::next()
{
	return std::rand();
}

i32_t Random::next(i32_t minValue, i32_t maxValue)
{
	return minValue + (static_cast<i32_t>((maxValue-minValue)*(std::rand()*1.0f/RAND_MAX)));
}

f32_t Random::next(f32_t minValue, f32_t maxValue)
{
	return minValue+((maxValue-minValue)*(std::rand()*1.0f/RAND_MAX));
}

f64_t Random::next(f64_t minValue, f64_t maxValue)
{
	return minValue+((maxValue-minValue)*(std::rand()*1.0/RAND_MAX));
}

_EndNamespace(eokas)
