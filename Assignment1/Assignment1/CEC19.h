#ifndef CEC19_H
#define CEC19_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class AbstractFunction {
protected:

	size_t dimensions;

	// Shrink parameter used to the orginal search range
	double shrinkRate;

	// Matrix rotation data
	double* M = nullptr;

	// Shifting data
	double* OShift = nullptr;

	void shiftAndRotate(vector<double>& point);
public:

	const double INF = 1.0e99;
	const double EPS = 1.0e-14;
	const double E = 2.7182818284590452353602874713526625;
	const double PI = 3.1415926535897932384626433832795029;

	AbstractFunction(size_t functionNumber, size_t dimensions, double shrinkRate);

	~AbstractFunction();

	virtual double operator()(vector<double> point) = 0;
};

class Chebyshev : public AbstractFunction {
public:
	Chebyshev(size_t functionNumber, size_t dimensions, double shrinkRate);

	double operator() (vector<double> point) final;
};

class Rastrigin : public AbstractFunction {
public:
	Rastrigin(size_t functionNumber, size_t dimensions, double shrinkRate);

	double operator()(vector<double> point) final;
};

class Weierstrass : public AbstractFunction {
public:
	Weierstrass(size_t functionNumber, size_t dimensions, double shrinkRate);

	double operator()(vector<double> point) final;
};

class Ackley : public AbstractFunction {
public:
	Ackley(size_t functionNumber, size_t dimensions, double shrinkRate);

	double operator()(vector<double> point) final;
};

class Griewank : public AbstractFunction {
public:
	Griewank(size_t functionNumber, size_t dimensions, double shrinkRate);

	double operator()(vector<double> point) final;
};

class HappyCat : public AbstractFunction {
public:
	HappyCat(size_t functionNumber, size_t dimensions, double shrinkRate);

	double operator()(vector<double> point) final;
};

class Escaffer6 : public AbstractFunction {
public:
	Escaffer6(size_t functionNumber, size_t dimensions, double shrinkRate);

	double operator()(vector<double> point) final;
};

class Schwefel : public AbstractFunction {
public:
	Schwefel(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

	}

	double operator()(vector<double> point) final;
};

class LennardJones : public AbstractFunction {
public:
	LennardJones(size_t functionNumber, size_t dimensions, double shrinkRate);

	double operator()(vector<double> point) final;
};

class Hilbert : public AbstractFunction {
public:
	Hilbert(size_t functionNumber, size_t dimensions, double shrinkRate);

	double operator()(vector<double> point) final;
};

#endif