#include <math.h>
#include "cec19_func.h"
#include "SDE.h"
#pragma warning(disable:4996)

extern double* OShift, * M, * y, * z, * x_bound;
extern int ini_flag, n_flag, func_flag, * SS;

// C++ migration
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

int main()
{
	// Settings for cout
	std::cout << std::fixed << std::showpoint << std::setprecision(10);

	std::vector<Function> functions;
	functions.push_back(Function("Chebyshev", 1, 9, -8192, 8192));
	functions.push_back(Function("Hilbert", 2, 16, -16384, 16384));
	functions.push_back(Function("Lennard Jones", 3, 18, -4, 4));
	functions.push_back(Function("Rastrigin", 4, 10, -100, 100));
	functions.push_back(Function("Griewank", 5, 10, -100, 100));
	functions.push_back(Function("Weierstrass", 6, 10, -100, 100));
	functions.push_back(Function("Schwefel", 7, 10, -100, 100));
	functions.push_back(Function("Escaffer 6", 8, 10, -100, 100));
	functions.push_back(Function("Happy Cat", 9, 10, -100, 100));
	functions.push_back(Function("Ackely", 10, 10, -100, 100));

	// Function[3] means rastrigin, just a heads up
	SDE sde = SDE(100, 100, 1.0, 0.5, functions[3]);
	sde.run();

	return 0;
}


