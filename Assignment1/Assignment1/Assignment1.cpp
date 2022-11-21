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
	functions.push_back(Function("Ackely", 10, 10, -100, 100)); // 1
	functions.push_back(Function("Chebyshev", 1, 9, -8192, 8192)); // 2
	functions.push_back(Function("Weierstrass", 6, 10, -100, 100)); // 3
	functions.push_back(Function("Griewank", 5, 10, -100, 100)); // 4
	functions.push_back(Function("Rastrigin", 4, 10, -100, 100)); // 5
	functions.push_back(Function("Lennard Jones", 3, 18, -4, 4)); // 6
	functions.push_back(Function("Hilbert", 2, 16, -16384, 16384)); // 7
	functions.push_back(Function("Schwefel", 7, 10, -100, 100));// 8
	functions.push_back(Function("Escaffer 6", 8, 10, -100, 100)); // 9
	functions.push_back(Function("Happy Cat", 9, 10, -100, 100)); // 10

	// For rastrigin:
	// cognitive coef : 1.49445
	// social coef : 1.49445
	// global coef : 0.3645
	// inertia weight : 0.729
	// maxFE: 1e8 (neatins)
	vector<double> result;
	int digits = 0;
	for (int functionId = functions.size() - 1; functionId >= 0; --functionId) {
		cout << "Running test for function: " << functions[functionId].getName() << '\n';
		PSO psoTest(functions[functionId], 100, 100, 1e8);
		psoTest.setCognitiveCoefficient(1.5);
		psoTest.setSocialCoefficient(1.5);

		if (functions[functionId].getName() == "Ackley") {
			psoTest.setInertiaWeight(0.1);
			psoTest.setGlobalCoefficient(0.2);
		}
		if (functions[functionId].getName() == "Griewank") {
			psoTest.setInertiaWeight(0.1);
			psoTest.setGlobalCoefficient(0.7);
		}
		if (functions[functionId].getName() == "Happy Cat") {
			psoTest.setInertiaWeight(0.2);
			psoTest.setGlobalCoefficient(0.4);
		}
		if (functions[functionId].getName() == "Escaffer 6") {
			psoTest.setInertiaWeight(0.2);
			psoTest.setGlobalCoefficient(0.2);
		}
		if (functions[functionId].getName() == "Schwefel") {
			psoTest.setInertiaWeight(0.2);
			psoTest.setGlobalCoefficient(0.6);
		}
		if (functions[functionId].getName() == "Lennard Jones") {
			psoTest.setInertiaWeight(1);
			psoTest.setGlobalCoefficient(0);
		}
		if (functions[functionId].getName() == "Chebyshev") {
			psoTest.setInertiaWeight(0.2);
			psoTest.setGlobalCoefficient(0.6);
		}
		if (functions[functionId].getName() == "Hilbert") {
			psoTest.setInertiaWeight(0.2);
			psoTest.setGlobalCoefficient(0.6);
		}
		if (functions[functionId].getName() == "Rastrigin") {
			psoTest.setInertiaWeight(0.7);
			psoTest.setGlobalCoefficient(0.4);
		}
		if (functions[functionId].getName() == "Weierstrass") {
			psoTest.setInertiaWeight(0.1);
			psoTest.setGlobalCoefficient(0.2);
		}

		psoTest.run(true);
	}

	return 0;
}