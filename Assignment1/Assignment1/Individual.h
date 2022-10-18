#pragma once
#include <vector>
#include <iostream>
#include "Random.h"
#include "Function.h"

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
	Individual(vector<double>& x);

	/// <summary>
	/// Constructor for randomly initialized point representing an individual
	/// </summary>
	/// <param name="dimensions">point dimensions</param>
	/// <param name="min">function minima</param>
	/// <param name="max">function maxima</param>
	Individual(Function& function, Random& rng);

	double* get();

	double operator[](int index);

	/// <summary>
	/// Computes element-wise difference between two individuals
	/// </summary>
	/// <param name="other">other individual</param>
	/// <returns>new individual</returns>
	Individual operator-(Individual other);

	/// <summary>
	/// Computes element-wise sum between two individuals
	/// </summary>
	/// <param name="other">other individual</param>
	/// <returns>new individual</returns>
	Individual operator+(Individual other);

	/// <summary>
	/// Computes element-wise multiplication with a factor
	/// </summary>
	/// <param name="other">factor</param>
	/// <returns>new individual</returns>
	Individual operator*(double factor);

	/// <summary>
	/// Default printing method to console output.
	/// </summary>
	/// <param name="os">usually std::cout, the output stream.</param>
	/// <param name="individual">the object to be printed</param>
	/// <returns>a stream representation of the given object</returns>
	friend std::ostream& operator<<(std::ostream& os, Individual const& individual);
};

