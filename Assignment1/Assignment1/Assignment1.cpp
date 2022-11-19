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
	//functions.push_back(Function("Chebyshev", 1, 9, -8192, 8192)); // 2
	functions.push_back(Function("Hilbert", 2, 16, -16384, 16384)); // 7
	//functions.push_back(Function("Lennard Jones", 3, 18, -4, 4)); // 6
	//functions.push_back(Function("Rastrigin", 4, 10, -100, 100)); // 5
	//functions.push_back(Function("Griewank", 5, 10, -100, 100)); // 4
	//functions.push_back(Function("Weierstrass", 6, 10, -100, 100)); // 3
	//functions.push_back(Function("Schwefel", 7, 10, -100, 100));// 8
	//functions.push_back(Function("Escaffer 6", 8, 10, -100, 100)); // 9
	//functions.push_back(Function("Happy Cat", 9, 10, -100, 100)); // 10
	//functions.push_back(Function("Ackely", 10, 10, -100, 100)); // 1

	// For rastrigin:
	// cognitive coef : 1.49445
	// social coef : 1.49445
	// global coef : 0.3645
	// inertia weight : 0.729
	// maxFE: 1e8 (neatins)
	vector<double> result;
	int digits = 0;
	for (int functionID = functions.size() - 1; functionID >= 0; --functionID) {
		cout << "Running test for function: " << functions[functionID].getName() << '\n';
		PSO psoTest(functions[functionID], 100, 100, 1e8);
		psoTest.setSocialCoefficient(1.5);
		psoTest.setCognitiveCoefficient(1.5);
		int best = 0;
		for (double inertia = 0; inertia <= 1; inertia += 0.2) {
			cout << "Running test with INERTIA WEIGHT = " << inertia << "\n";
			psoTest.setInertiaWeight(inertia);
			for (double global = 0; global <= 1; global += 0.2) {
				cout << "Running test with GLOBAL COEFICENT = " << global << "\n";
				int sum = 0;
				for (int i = 0; i < 10; ++i) {
					psoTest.setGlobalCoefficient(global);
					result = psoTest.run(false);
					cout << "Result: " << functions[functionID](result) << "\n";
					digits = countCorrectDigits(functions[functionID](result));
					sum += digits;
					if (digits != 10) {
						break;
					}
				}
				if (sum == 100) {
					cout << "!!PERFECT SCORE!!" << '\n';
					if (sum > best)
						best = sum;
					break;
				}
			}
			cout << "Ten run best found: " << best << '\n';
		}
	}

	return 0;
}