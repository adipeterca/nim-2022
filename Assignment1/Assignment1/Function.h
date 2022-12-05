#ifndef FUNCTION_H
#define FUNCTION_H

#include "cec19_func.h"
#include "Individual.h"
#include <string>
#include <math.h>
#include <fstream>

using namespace std;

/// <summary>
/// Contains all data needed for a function
/// </summary>
class Function
{
private:
	const unsigned int id;
	const size_t dimensions;
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

	/// <summary>
	/// Calls function on input
	/// </summary>
	/// <param name="x">x vector of coordinates</param>
	/// <returns>f(x) (function value)</returns>
	const double operator()(vector<double>& x);

	// Does not work because of #include recurssion
	// const double operator ()(const Individual& x);

	const size_t getDimensions();

	const double getMin();

	const double getMax();

	const string& getName();
};

class AbstractFunction {
protected:

	size_t dimensions;

	// Shrink parameter used to the orginal search range
	double shrinkRate;

	// Matrix rotation data
	double* M = nullptr;

	// Shifting data
	double* OShift = nullptr;

	void shiftAndRotate(vector<double>& point) {
		if (point.size() != dimensions) {
			throw invalid_argument("Point size and function's dimensions don't match!");
		}
		
		// Shift value
		for (size_t i = 0; i < point.size(); ++i) {
			point[i] -= OShift[i];
		}

		// Shrink to the original size
		for (size_t i = 0; i < point.size(); ++i) {
			point[i] *= shrinkRate;
		}

		// Rotate value
		vector<double> rotatedPoint;
		rotatedPoint.resize(point.size());
		for (size_t i = 0; i < point.size(); ++i) {
			rotatedPoint[i] = 0;
			for (size_t j = 0; j < point.size(); ++j) {
				rotatedPoint[i] += point[j] * M[i * point.size() + j];
			}
		}

		point = rotatedPoint;
	}
public:
	
	const double INF = 1.0e99;
	const double EPS = 1.0e-14;
	const double E = 2.7182818284590452353602874713526625;
	const double PI = 3.1415926535897932384626433832795029;
	
	AbstractFunction(size_t functionNumber, size_t dimensions, double shrinkRate) {
		if (functionNumber > 10) {
			throw invalid_argument("Function number must be lower that 11.");
		}

		this->dimensions = dimensions;
		this->shrinkRate = shrinkRate;

		// If requiered, initialize M and OShift
		if (functionNumber > 3) {
			M = new double[dimensions * dimensions];
			OShift = new double[dimensions];

			// Read shift data
			ifstream fShift("input_data/shift_data_" + to_string(functionNumber) + ".txt");
			for (size_t i = 0; i < dimensions; ++i) {
				fShift >> OShift[i];
			}

			// Read matrix data
			ifstream fMatrix("input_data/M_" + to_string(functionNumber) + "_D" + to_string(dimensions) + ".txt");
			for (size_t i = 0; i < dimensions * dimensions; ++i) {
				fMatrix >> M[i];
			}
		}
	}

	~AbstractFunction() {
		delete[] M;
		delete[] OShift;
	}

	virtual double operator()(vector<double> point) = 0;
};

class Chebyshev : public AbstractFunction {
public:
	Chebyshev(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {
		// Maybe set ranges?
	}

	double operator() (vector<double> point) final {
		if (point.size() <= 2) {
			throw invalid_argument("Chebyshev function is only defined for Dsize > 2");
		}

		int sample;
		double result = 0.0;
		double a = 1.0;
		double b = 1.2;
		double y = -1.0;
		double px;
		long double dx, dy;

		for (int i = 0; i < point.size() - 2; ++i) {
			dx = 2.4 * b - a;
			a = b;
			b = dx;
		}

		sample = 32 * point.size();
		dy = 2.0 / (long double)sample;

		for (int i = 0; i <= sample; ++i) {
			px = point[0];
			for (int j = 1; j < point.size(); ++j) {
				px = y * px + point[j];
			}

			if (px < -1 || px > 1) {
				result += (1.0 - fabs(px)) * (1.0 - fabs(px));
			}
			y += dy;
		}

		for (int i = -1; i <= 1; i += 2) {
			px = point[0];

			for (int j = 1; j < point.size(); ++j) {
				px = 1.2 * px + point[j];
			}

			if (px < dx) {
				result += px * px;
			}
		}

		// We add one to guarantee that the global minimum is different from 0 (useful for fitness functions).
		return result + 1;
	}
};

class Rastrigin : public AbstractFunction {
public:
	Rastrigin(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

	}

	double operator()(vector<double> point) final {
		double result = 0.0;
		shiftAndRotate(point);

		for (int i = 0; i < point.size(); ++i) {
			result += (point[i] * point[i] - 10.0 * cos(2.0 * PI * point[i]) + 10.0);
		}

		return result + 1;
	}
};

#endif