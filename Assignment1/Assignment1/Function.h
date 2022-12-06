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

class Weierstrass : public AbstractFunction {
public:
	Weierstrass(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

	}

	double operator()(vector<double> point) final {
		int k_max;
		double sum, sum2, a, b, result;
		a = 0.5;
		b = 3.0;
		k_max = 20;
		result = 0.0;
		shiftAndRotate(point);

		for (int i = 0; i < point.size(); ++i) {
			sum = 0.0;
			sum2 = 0.0;
			for (int j = 0; j <= k_max; ++j)
			{
				sum += pow(a, j) * cos(2.0 * PI * pow(b, j) * (point[i] + 0.5));
				sum2 += pow(a, j) * cos(2.0 * PI * pow(b, j) * 0.5);
			}
			result += sum;
		}
		result -= point.size() * sum2;

		return result + 1;
	}
};

class Ackley : public AbstractFunction {
public:
	Ackley(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

	}

	double operator()(vector<double> point) final {
		double result = 0.0, sum1 = 0.0, sum2 = 0.0;
		shiftAndRotate(point);

		for (int i = 0; i < point.size(); ++i) {
			sum1 += point[i] * point[i];
			sum2 += cos(2.0 * PI * point[i]);
		}
		sum1 = -0.2 * sqrt(sum1 / point.size());
		sum2 /= point.size();
		result = E - 20.0 * exp(sum1) - exp(sum2) + 20.0;
		return result + 1;
	}
};

class Griewank : public AbstractFunction {
public:
	Griewank(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

	}

	double operator()(vector<double> point) final {
		double result = 0.0, s = 0.0, p = 1.0;
		shiftAndRotate(point);

		for (int i = 0; i < point.size(); ++i) {
			s += point[i] * point[i];
			p *= cos(point[i] / sqrt(1.0 + i));
		}
		result = 1.0 + s / 4000.0 - p;
		return result + 1;
	}
};

class HappyCat : public AbstractFunction {
public:
	HappyCat(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

	}

	double operator()(vector<double> point) final {
		double result = 0.0, alpha = 1.0 / 8.0, r2 = 0.0, sum_z = 0.0;
		shiftAndRotate(point);

		for (int i = 0; i < point.size(); ++i) {
			point[i] = point[i] - 1.0;//shift to orgin
			r2 += point[i] * point[i];
			sum_z += point[i];
		}
		result = pow(fabs(r2 - point.size()), 2 * alpha) + (0.5 * r2 + sum_z) / point.size() + 0.5;
		return result + 1;
	}
};

class Escaffer6 : public AbstractFunction {
public:
	Escaffer6(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

	}

	double operator()(vector<double> point) final {
		double result = 0.0, temp1, temp2;
		shiftAndRotate(point);

		for (int i = 0; i < point.size() - 1; ++i) {
			temp1 = sin(sqrt(point[i] * point[i] + point[i + 1] * point[i + 1]));
			temp1 = temp1 * temp1;
			temp2 = 1.0 + 0.001 * (point[i] * point[i] + point[i + 1] * point[i + 1]);
			result += 0.5 + (temp1 - 0.5) / (temp2 * temp2);
		}
		temp1 = sin(sqrt(point[point.size() - 1] * point[point.size() - 1] + point[0] * point[0]));
		temp1 = temp1 * temp1;
		temp2 = 1.0 + 0.001 * (point[point.size() - 1] * point[point.size() - 1] + point[0] * point[0]);
		result += 0.5 + (temp1 - 0.5) / (temp2 * temp2);
		return result + 1;
	}
};

#endif