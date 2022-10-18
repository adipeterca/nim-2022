#pragma once
#include "cec19_func.h"
#include <string>

/// <summary>
/// Contains all data needed for a function
/// </summary>
class Function
{
private:
	int id, dimensions;
	double min, max;
	std::string name;
public:
	/// <summary>
	/// Function constructor
	/// </summary>
	/// <param name="id">function id</param>
	/// <param name="D">number of dimensions</param>
	/// <param name="min">function minima</param>
	/// <param name="max">function maxima</param>
	Function(const char * name, int id, int D, double min, double max);

	/// <summary>
	/// Calls function on input
	/// </summary>
	/// <param name="x">x coordinates</param>
	/// <returns>f(x) (function value)</returns>
	double operator ()(double* x);

	int getDimensions();

	double getMin();

	double getMax();

	std::string& getName();
};

