#pragma once
#include <vector>
#include "Utils.h"

using namespace std;

class Individual
{
private:
	vector<double> x;
public:
	/// <summary>
	/// Constructor for randomly initialized point representing an individual
	/// </summary>
	/// <param name="dimensions">point dimensions</param>
	/// <param name="min">function minima</param>
	/// <param name="max">function maxima</param>
	Individual(int dimensions, double min, double max) {
		for (int i = 0; i < dimensions; ++i) {
			x.push_back(getRandomDouble(min, max));
		}
	}

	double* get() {
		return x.data(); // doesnt work for bool
	}
};

