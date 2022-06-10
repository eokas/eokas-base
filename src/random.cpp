
#include "random.h"
#include <ctime>
#include <cstdlib>

_BeginNamespace(eokas)

Random_Fake::Random_Fake()
	:seed(0)
{
	seed = static_cast<unsigned int>(std::time(nullptr)%1024);
	std::srand(seed);
}

Random_Fake::Random_Fake(u32_t seed)
    :seed(seed)
{
    std::srand(seed);
}

f32_t Random_Fake::make() const
{
    return (std::rand() + 0.0f) / (RAND_MAX);
}

f32_t Random_Fake::value()
{
    return globalRandom.make();
}

i32_t Random_Fake::range(i32_t min, i32_t max)
{
    return min + std::floor(globalRandom.make() * (max - min));
}

f32_t Random_Fake::range(f32_t min, f32_t max)
{
    return min + globalRandom.make() * (max - min);
}

f64_t Random_Fake::range(f64_t min, f64_t max)
{
    return min + globalRandom.make() * (max - min);
}

_EndNamespace(eokas)
