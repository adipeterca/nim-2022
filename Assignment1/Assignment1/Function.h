#pragma once
#include "cec19_func.h"
#include <string>

using namespace std;

/// <summary>
/// Contains all data needed for a function
/// </summary>
class Function
{
private:
	const int id, dimensions;
	const double min, max;
	string name;
public:
	/// <summary>
	/// Function constructor
	/// </summary>
	/// <param name="id">function id</param>
	/// <param name="D">number of dimensions</param>
	/// <param name="min">function minima</param>
	/// <param name="max">function maxima</param>
	Function(const char * name, const int &id, const int &D, const double &min, const double &max);

	/// <summary>
	/// Calls function on input
	/// </summary>
	/// <param name="x">x coordinates</param>
	/// <returns>f(x) (function value)</returns>
	const double operator ()(const double* x);

	const int getDimensions();

	const double getMin();

	const double getMax();

	string& getName();
};

