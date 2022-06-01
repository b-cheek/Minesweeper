#include "Random.h"
#include <ctime>

//Declaring is necessary because it is a static variable
std::mt19937 Random::random(time(0));

int Random::Int(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(random);
}

float Random::Float(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random);
}