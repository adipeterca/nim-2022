#pragma once
#include <vector>
#include "Random.h"

using namespace std;

class Individual
{
private:
	vector<double> x;

public:
	/// <summary>
	/// Constructor for non-random initialization
	/// </summary>
	/// <param name="x">point coordinates</param>
	Individual(vector<double> x) {
		this->x = x;
	}

	/// <summary>
	/// Constructor for randomly initialized point representing an individual
	/// </summary>
	/// <param name="dimensions">point dimensions</param>
	/// <param name="min">function minima</param>
	/// <param name="max">function maxima</param>
	Individual(int dimensions, double min, double max) {
		for (int i = 0; i < dimensions; ++i) {
			x.push_back(Random::getRandomDouble(min, max));
		}
	}

	double* get() {
		return x.data(); // doesnt work for bool
	}

	/// <summary>
	/// get value at index
	/// </summary>
	/// <param name="index">position of interest</param>
	/// <returns>value at index</returns>
	double get(int index) {
		return x[index];
	}

	/// <summary>
	/// Computes element-wise difference between two individuals
	/// </summary>
	/// <param name="other">other individual</param>
	/// <returns>new individual</returns>
	Individual operator-(Individual other) {
		vector<double> diff;
		for (int i = 0; i < x.size(); ++i) {
			diff.push_back(x[i] - other.get(i));
		}
		return Individual(diff);
	}

	/// <summary>
	/// Computes element-wise sum between two individuals
	/// </summary>
	/// <param name="other">other individual</param>
	/// <returns>new individual</returns>
	Individual operator+(Individual other) {
		vector<double> sum;
		for (int i = 0; i < x.size(); ++i) {
			sum.push_back(x[i] + other.get(i));
		}
		return Individual(sum);
	}

	/// <summary>
	/// Computes element-wise multiplication with a factor
	/// </summary>
	/// <param name="other">factor</param>
	/// <returns>new individual</returns>
	Individual operator*(double factor) {
		vector<double> result;
		for (int i = 0; i < x.size(); ++i) {
			result.push_back(x[i] * factor);
		}
		return Individual(result);
	}
};

