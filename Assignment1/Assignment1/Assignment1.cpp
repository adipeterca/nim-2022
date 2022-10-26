#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>
#include "cec19_func.h"
#include "Utils.h"
#include "jDE100v2.h"
#pragma warning(disable:4996)

using namespace std;

extern double* OShift, * M, * y, * z, * x_bound;
extern int ini_flag, n_flag, func_flag, * SS;

int main()
{
	// Settings for cout
	cout << fixed << showpoint << setprecision(10);

	/*ofstream fout("output.txt");
	fout << fixed << showpoint << setprecision(10);*/

	vector<Function> functions; // difficulty ranking
	functions.push_back(Function("Chebyshev", 1, 9, -8192, 8192)); // 2 -> a gasit MIN
	functions.push_back(Function("Hilbert", 2, 16, -16384, 16384)); // 7 -> a gasit MIN
	functions.push_back(Function("Lennard Jones", 3, 18, -4, 4)); // 6 -> a gasit MIN
	functions.push_back(Function("Rastrigin", 4, 10, -100, 100)); // 5 -> a gasit MIN
	functions.push_back(Function("Griewank", 5, 10, -100, 100)); // 4 -> a gasit MIN
	functions.push_back(Function("Weierstrass", 6, 10, -100, 100)); // 3 -> a gasit MIN
	functions.push_back(Function("Schwefel", 7, 10, -100, 100));// 8
	functions.push_back(Function("Escaffer 6", 8, 10, -100, 100)); // 9
	functions.push_back(Function("Happy Cat", 9, 10, -100, 100)); // 10
	functions.push_back(Function("Ackely", 10, 10, -100, 100)); // 1
	
	ParameterConstrains mutation1(0.2, 1.1, 0.5, 0.1);
	ParameterConstrains mutation2(0.002, 1.1, 0.5, 0.3);
	ParameterConstrains crossover1(0.0, 1.1, 0.5, 0.1);
	ParameterConstrains crossover2(1.0, 1.1, 0.5, 0.3);

	jDE100v2* alg;
	int overallAccuracy;
	for (int functionId = 8; functionId < 10; functionId++) {
		overallAccuracy = 0;

		if (functionId == 9) {
			alg = new jDE100v2(functions[functionId], mutation2, crossover2);
		}
		else {
			alg = new jDE100v2(functions[functionId], mutation1, crossover1);
		}

		for (int ii = 1; ii <= 50; ii++) {
			cout << BLUE_START << "----------------- Starting testing for run " << setw(2) << ii << " with function " << COLOR_END;
			cout << RED_START << functions[functionId].getName() << COLOR_END;
			cout << BLUE_START << " ----------------- " << COLOR_END << "\n";
			ini_flag = 0;
			overallAccuracy += alg->run(ii);
		}
		cout << "\nFinally, overall accuracy for " << YELLOW_START << functions[functionId].getName() << COLOR_END << ": ";
		cout << YELLOW_START << overallAccuracy / 30.0 << "%" << COLOR_END << "\n\n\n\n";

		delete alg;
	}


	// Idei de improvement:
	// unordered_map pentru functii in loc de vectors
	// problema apare cand sunt prea multe puncte (de obicei dupa 1e10)
	
	return 0;
}