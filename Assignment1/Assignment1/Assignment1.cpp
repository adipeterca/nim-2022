#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>
#include "cec19_func.h"
#include "Utils.h"
#include "jDE100v2.h"
#include "PSO.h"
#pragma warning(disable:4996)

using namespace std;

 extern double* OShift, * M, * y, * z, * x_bound;
 extern int ini_flag, n_flag, func_flag, * SS;

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
	
	// For rastrigin:
	// cognitive coef : 1.49445
	// social coef : 1.49445
	// global coef : 0.3645
	// inertia weight : 0.729
	// maxFE: 1e8 (neatins)
	vector<double> result;
	for (double i = 0; i <= 3; i += 0.3) {

		PSO psoTest(functions[4], 100, 100, 1e8);
		psoTest.setSocialCoefficient(i);
		cout << "Running test with SOCIAL COEFICENT = " << i << "\n";
		result = psoTest.run(false);
		cout << "Result: " << functions[4](result) << "\n";
		cout << "Correct digits: " << countCorrectDigits(functions[4](result)) << "\n\n";
	}
	
	return 0;
}