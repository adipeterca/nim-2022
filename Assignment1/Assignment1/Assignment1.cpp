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

void test(Function* f1, AbstractFunction* f2) {
	vector<double> point;
	point.resize(f1->getDimensions());

	cout << "Evaluating function " << f1->getName() << ":\n";
	int count = 0;
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < point.size(); j++)
			point[j] = getRandomDouble(f1->getMin(), f1->getMax());

		double a = (*f1)(point);
		double b = (*f2)(point);
		if (a - b == 0) {
			count++;
		}
		else {
			cout << a << " " << b << "\n";
		}
	}
	cout << count << "\n";
}

int main()
{
	// Settings for cout
	cout << fixed << showpoint << setprecision(10);

	vector<Function> functions; // difficulty ranking
	functions.push_back(Function("Ackely", 10, 10, -100, 100)); // 1 done
	functions.push_back(Function("Chebyshev", 1, 9, -8192, 8192)); // 2 done
	functions.push_back(Function("Weierstrass", 6, 10, -100, 100)); // 3 done
	functions.push_back(Function("Griewank", 5, 10, -100, 100)); // 4 done
	functions.push_back(Function("Rastrigin", 4, 10, -100, 100)); // 5 done
	functions.push_back(Function("Lennard Jones", 3, 18, -4, 4)); // 6 done
	functions.push_back(Function("Hilbert", 2, 16, -16384, 16384)); // 7 done
	functions.push_back(Function("Schwefel", 7, 10, -100, 100));// 8 done
	functions.push_back(Function("Escaffer 6", 8, 10, -100, 100)); // 9 done
	functions.push_back(Function("Happy Cat", 9, 10, -100, 100)); // 10 done

	class Ackley ackley(10, 10, 1.0);
	class Chebyshev chebyshev(1, 9, 1.0);
	class Weierstrass weierstrass(6, 10, 0.5 / 100.0);
	class Griewank griewank(5, 10, 600.0 / 100.0);
	class Rastrigin rastrigin(4, 10, 5.12 / 100.0);
	class LennardJones lennardJones(3, 18, 1.0);
	class Hilbert hilbert(2, 16, 1.0);
	class Schwefel schwefel(7, 10, 1000.0 / 100.0);
	class Escaffer6 escaffer6(8, 10, 1.0);
	class HappyCat happyCat(9, 10, 5.0 / 100.0);
	
	//test(&functions[0], &ackley);
	//test(&functions[1], &chebyshev);
	//test(&functions[2], &weierstrass);
	//test(&functions[3], &griewank);
	//test(&functions[4], &rastrigin);
	//test(&functions[5], &lennardJones);
	//test(&functions[6], &hilbert);
	//test(&functions[7], &schwefel);
	//test(&functions[8], &escaffer6);
	test(&functions[9], &happyCat);

	return 0;
}