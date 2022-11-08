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
	
	ParameterConstrains* mutation, * crossover;
	jDE100v2* alg;

	int overallAccuracy;
	int numberOfRuns = 10;
	
	// the first 6 functions are already done
	for (int functionId = 5; functionId < 6; functionId++) {
		overallAccuracy = 0;
		
		// Skip some functions
		// if (functionId != 1 && functionId != 9) continue;

		if (functionId == 3) {
			mutation = new ParameterConstrains(0.2, 1.1, 0.5, 0.1);
			crossover = new ParameterConstrains(0.0, 1.1, 0.9, 0.1);
		}
		else if (functionId == 6) {
			mutation = new ParameterConstrains(0.2, 1.1, 0.5, 0.1);
			crossover = new ParameterConstrains(0.0, 1.1, 0.9, 0.1);
		}
		else if (functionId == 7) {
			mutation = new ParameterConstrains(0.1, 1.1, 0.5, 0.1);
			crossover = new ParameterConstrains(0.1, 1.1, 0.9, 0.1);
		}
		else if (functionId == 8) {
			mutation = new ParameterConstrains(0.001, 1.1, 0.5, 0.1);
			crossover = new ParameterConstrains(1.0, 1.1, 0.9, 0.1);
		}
		else {
			mutation = new ParameterConstrains(0.15, 1.1, 0.5, 0.1);
			crossover = new ParameterConstrains(0.0, 1.1, 0.9, 0.1);
		}

		alg = new jDE100v2(functions[functionId], *mutation, *crossover);
		for (size_t ii = 1; ii <= numberOfRuns; ii++) {
			cout << BLUE_START << "----------------- Starting testing for run " << setw(2) << ii << " with function " << COLOR_END;
			cout << RED_START << functions[functionId].getName() << COLOR_END;
			cout << BLUE_START << " ----------------- " << COLOR_END << "\n";
			
			overallAccuracy += alg->run(ii);
		}
	
		cout << "\nFinally, overall accuracy for " << YELLOW_START << functions[functionId].getName() << COLOR_END << ": ";
		cout << YELLOW_START << overallAccuracy / (double)(numberOfRuns) << "%" << COLOR_END << "\n\n\n\n";

		delete alg;
		delete mutation;
		delete crossover;
	}


	// Idei de improvement:
	// unordered_map pentru functii in loc de vectors
	// problema apare cand sunt prea multe puncte (de obicei dupa 1e10)
	
	return 0;
}