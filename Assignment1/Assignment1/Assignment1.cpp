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

	class Rastrigin r(4, 10, 5.12 / 100.0);
	vector<double> point;
	point.resize(10);
	int count = 0;
	for (int i = 0; i < 100000; i++) {
		for (int j = 0; j < 10; j++)
			point[j] = getRandomDouble(-100, 100);
		
		double a = functions[4](point);
		double b = r(point);
		if (a - b == 0) {
			count++;
		}
		else {
			cout << a << " " << b << "\n";
		}
	}
	cout << count;
	return 0;
}