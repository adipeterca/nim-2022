#include "Utils.h"

mt19937 generator(chrono::high_resolution_clock::now().time_since_epoch().count());

double getRandomDouble(const double min, const double max) {
	uniform_real_distribution<> distribution(min, max);
	return distribution(generator);
}

size_t getRandomUnsigned(const size_t min, const size_t max) {
	uniform_int_distribution<> distribution(min, max);
	return distribution(generator);
}