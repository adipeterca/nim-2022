#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <iostream>
#include "Utils.h"
#include "Function.h"

using namespace std;

// #include reference
class Function;

class Individual
{
private:
	vector<double> x;
	
	double value;

	// An Individual object CAN exist without a function, so
	// we cannot use references.
	Function* function = nullptr;
public:
	/// <summary>
	/// Constructor for non-random initialization
	/// </summary>
	/// <param name="x">point coordinates</param>
	Individual(vector<double>& const x);

	Individual(const Individual& otherIndividual);

	/// <summary>
	/// Ties a function to this point (and also randomize it along the search space).
	/// </summary>
	/// <param name="function">the function to which this object is tied</param>
	Individual(Function& function);

	Individual() {};

	/// <summary>
	/// Resize the point for a given function, while also binding that function to this point.
	/// </summary>
	/// <param name="function">the specified function</param>
	void resize(Function& function);

	const double* get() const;

	vector<double> getVector();

	double& operator[](const unsigned&index);

	bool operator==(const Individual& ind) const;

	/// <summary>
	/// Computes element-wise difference between two individuals
	/// </summary>
	/// <param name="other">other individual</param>
	/// <returns>new individual</returns>
	const Individual operator-(const Individual &other) const;

	/// <summary>
	/// Computes element-wise sum between two individuals
	/// </summary>
	/// <param name="other">other individual</param>
	/// <returns>new individual</returns>
	const Individual operator+(const Individual &other) const;

	/// <summary>
	/// Computes element-wise multiplication with a factor
	/// </summary>
	/// <param name="other">factor</param>
	/// <returns>new individual</returns>
	const Individual operator*(const double &factor) const;

	/// <summary>
	/// Default printing method to console output.
	/// </summary>
	/// <param name="os">usually cout, the output stream.</param>
	/// <param name="individual">the object to be printed</param>
	/// <returns>a stream representation of the given object</returns>
	friend ostream& operator<<(ostream& os, const Individual &individual);
};
#endif