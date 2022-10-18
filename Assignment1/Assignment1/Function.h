#pragma once
#include "cec19_func.h"

/// <summary>
/// Contains all data needed for a function
/// </summary>
class Function
{
private:
	int id, dimensions;
	double min, max;
public:
	/// <summary>
	/// Function constructor
	/// </summary>
	/// <param name="id">function id</param>
	/// <param name="D">number of dimensions</param>
	/// <param name="min">function minima</param>
	/// <param name="max">function maxima</param>
	Function(int id, int D, double min, double max) {
		this->id = id;
		this->dimensions = D;
		this->min = min;
		this->max = max;
	}

	/// <summary>
	/// calls function on input
	/// </summary>
	/// <param name="x">x coordinates</param>
	/// <param name="f">result of f(x)</param>
	void operator ()(double* x, double* f) {
		cec19_test_func(x, f, dimensions, 1, id);
	}

	int getDimensions() {
		return dimensions;
	}

	double getMin() {
		return min;
	}

	double getMax() {
		return max;
	}
};

