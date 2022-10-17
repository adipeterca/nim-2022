#pragma once
#include <random>
#include <chrono>
#include "Generator.h"

using namespace std;

class Random
{
public:
	/// <summary>
	/// Generate a random double number using a uniform distribution inside an interval
	/// </summary>
	/// <param name="min">interval lowest value</param>
	/// <param name="max">interval highest value</param>
	/// <returns>random double number</returns>
	static double getRandomDouble(double min, double max) {
		int chrono_seed = chrono::system_clock::now().time_since_epoch().count();
		mt19937 mt(chrono_seed);
		int seed = mt();
		mt19937 generator(seed);
		uniform_real_distribution<> distribution(min, max);
		return distribution(generator);
	}

	/// <summary>
	/// Generate a random integer number using a uniform distribution inside an interval
	/// </summary>
	/// <param name="min">interval lowest value</param>
	/// <param name="max">interval highest value</param>
	/// <returns>random integer number</returns>
	static int getRandomInteger(int min, int max) {
		int chrono_seed = chrono::system_clock::now().time_since_epoch().count();
		mt19937 mt(chrono_seed);
		int seed = mt();
		mt19937 generator(seed);
		uniform_int_distribution<> distribution(min, max);
		return distribution(generator);
	}
};

