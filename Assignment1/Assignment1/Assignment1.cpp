#include <math.h>
// C++ migration
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "cec19_func.h"
#include "SDE.h"
#pragma warning(disable:4996)

using namespace std;

extern double* OShift, * M, * y, * z, * x_bound;
extern int ini_flag, n_flag, func_flag, * SS;

int main()
{
	// Settings for cout
	cout << fixed << showpoint << setprecision(10);

	vector<Function> functions;
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
	// According to Storn and Price (1997),
	// DE behavior is more sensitive to the choice of F
	// than it is to the choice of CR. The study suggests
	// the values of [0.5, 1] for F, [0.8, 1] for CR and 10D
	// for NP, where D is the number of dimensions of the problem
	SDE sde = SDE(100, 5000, 0.75, 0.9, functions[3]);
	sde.run();

	return 0;
}


