#pragma once
#include <chrono>
#include <random>

using namespace std;

/// <summary>
/// Class for retrieving random numbers.
/// </summary>
class Random
{
	mt19937 generator;
public:

	/// <summary>
	/// Default constructor which sets the interval number generator.
	/// Is it thread safe tho?
	/// </summary>
	Random();

	/// <summary>
	/// Generate a random double number using a uniform distribution inside an interval
	/// </summary>
	/// <param name="min">interval lowest value</param>
	/// <param name="max">interval highest value</param>
	/// <returns>random double number</returns>
	double getRandomDouble(const double &min, const double &max);

	/// <summary>
	/// Generate a random integer number using a uniform distribution inside an interval
	/// </summary>
	/// <param name="min">interval lowest value</param>
	/// <param name="max">interval highest value</param>
	/// <returns>random integer number</returns>
	int getRandomUnsigned(const unsigned &min, const unsigned &max);
};

