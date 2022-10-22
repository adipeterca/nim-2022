#include "Random.h"

Random::Random() {
	int chrono_seed = chrono::system_clock::now().time_since_epoch().count();
	generator.seed(chrono_seed);
	generator.seed(generator());
}

double Random::getRandomDouble(const double &min, const double &max) {
	uniform_real_distribution<> distribution(min, max);
	return distribution(generator);
}

int Random::getRandomUnsigned(const unsigned&min, const unsigned&max) {
	uniform_int_distribution<> distribution(min, max);
	return distribution(generator);
}