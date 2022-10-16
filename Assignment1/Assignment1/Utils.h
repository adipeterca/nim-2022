#pragma once
#include <random>
#include <chrono>

using namespace std;

double getRandomDouble(double min, double max) {
	int chrono_seed = chrono::system_clock::now().time_since_epoch().count();
	mt19937 mt(chrono_seed);
	int seed = mt();
	mt19937 generator(seed);
	uniform_real_distribution<> distribution(min, max);
	return distribution(generator);
}

int getRandomInteger(int min, int max) {
	int chrono_seed = chrono::system_clock::now().time_since_epoch().count();
	mt19937 mt(chrono_seed);
	int seed = mt();
	mt19937 generator(seed);
	uniform_int_distribution<> distribution(min, max);
	return distribution(generator);
}