#include <math.h>
// C++ migration
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>
#include "cec19_func.h"
#include "SDE.h"
#pragma warning(disable:4996)

using namespace std;

extern double* OShift, * M, * y, * z, * x_bound;
extern int ini_flag, n_flag, func_flag, * SS;


int countCorrectDigits(double value) {
	int ivalue = (int)value, correctDigits = 0;
	if (ivalue == 1) {
		correctDigits++;
	}
	value = value - ivalue;
	for (int i = 0; i < 9; ++i) {
		value *= 10;
		ivalue = (int)value;
		if (ivalue == 0) {
			correctDigits++;
		}
	}
	return correctDigits;

}

int main()
{
	// Settings for cout
	cout << fixed << showpoint << setprecision(10);

	vector<Function> functions; // difficulty ranking
	functions.push_back(Function("Chebyshev", 1, 9, -8192, 8192)); // 2
	functions.push_back(Function("Hilbert", 2, 16, -16384, 16384)); // 7
	functions.push_back(Function("Lennard Jones", 3, 18, -4, 4)); // 6
	functions.push_back(Function("Rastrigin", 4, 10, -100, 100)); // 5
	functions.push_back(Function("Griewank", 5, 10, -100, 100)); // 4
	functions.push_back(Function("Weierstrass", 6, 10, -100, 100)); // 3
	functions.push_back(Function("Schwefel", 7, 10, -100, 100));// 8
	functions.push_back(Function("Escaffer 6", 8, 10, -100, 100)); // 9
	functions.push_back(Function("Happy Cat", 9, 10, -100, 100)); // 10
	functions.push_back(Function("Ackely", 10, 10, -100, 100)); // 1

	// Function[3] means rastrigin, just a heads up
	// According to Storn and Price (1997),
	// DE behavior is more sensitive to the choice of F
	// than it is to the choice of CR. The study suggests
	// the values of [0.5, 1] for F, [0.8, 1] for CR and 10D
	// for NP, where D is the number of dimensions of the problem
	SDE sde = SDE(500, 25000, 0.8, 0.2, functions[8], true);
	auto t1 = chrono::high_resolution_clock::now();
	double runBest = sde.run();
	auto t2 = chrono::high_resolution_clock::now();
	auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
	cout << "Execution time: " << ms_int.count() << " ms\n";
	cout << "Correct digits count: " << countCorrectDigits(runBest) << '\n';

	return 0;
}


