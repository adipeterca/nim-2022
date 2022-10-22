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


/// <summary>
/// Function used to count the correct digits. The answer should always be 1.000000000
/// </summary>
/// <param name="value">value whose digits are counted</param>
/// <returns>number of correct digits</returns>
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

	ofstream fout("output.txt");
	fout << fixed << showpoint << setprecision(10);

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

	vector<SDE> algorithms;
	algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[0], true)); // 2
	algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[1], true)); // 7
	algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[2], true)); // 6
	algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[3], true)); // 5
	algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[4], true)); // 4
	algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[5], true)); // 3
	algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[6], true)); // 8
	algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[7], true)); // 9
	algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[8], true)); // 10
	algorithms.push_back(SDE(1000, 1000000, 0.4, 0.2, functions[9], true)); // 1

	unsigned overallAccuracy = 0;
	unsigned currentAccuracy;
	auto t0 = chrono::high_resolution_clock::now();
	for (auto alg : algorithms) {
		cout << "Currently evaluating function " << alg.getFunction().getName() << "\n";
		fout << "Currently evaluating function " << alg.getFunction().getName() << "\n";
		
		if (alg.getFunction().getName() == "Chebyshev") {
			cout << "Skipped Chebyshev.\n";
			fout << "Skipped Chebyshev.\n";
			continue;
		}

		auto t1 = chrono::high_resolution_clock::now();
		double runBest = alg.run(fout);
		auto t2 = chrono::high_resolution_clock::now();
		auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
		currentAccuracy = countCorrectDigits(runBest);
		
		long long seconds = ms_int.count() / 1000;
		cout << "Execution time: " << seconds / 60 << " minutes and " << seconds % 60 << " seconds.\n";
		fout << "Execution time: " << seconds / 60 << " minutes and " << seconds % 60 << " seconds.\n";
		cout << "Correct digits count: " << currentAccuracy << "\n\n\n";
		fout << "Correct digits count: " << currentAccuracy << "\n\n\n";

		fout.flush();
		overallAccuracy += currentAccuracy;
	}
	auto t3 = chrono::high_resolution_clock::now();
	long long seconds = chrono::duration_cast<chrono::seconds>(t3 - t0).count();


	cout << "---------------------------------------------------------------------------\n";
	cout << "Overall accuracy: " << setw(10) << overallAccuracy << "/ 100\n";
	cout << "Overall time: " << setw(10) << seconds / 60 << " minutes and " << seconds % 60 << " seconds\n";

	fout << "---------------------------------------------------------------------------\n";
	fout << "Overall accuracy: " << setw(10) << overallAccuracy << "/ 100\n";
	fout << "Overall time: " << setw(10) << seconds / 60 << " minutes and " << seconds % 60 << " seconds\n";
	// Function[3] means rastrigin, just a heads up
	// According to Storn and Price (1997),
	// DE behavior is more sensitive to the choice of F
	// than it is to the choice of CR. The study suggests
	// the values of [0.5, 1] for F, [0.8, 1] for CR and 10D
	// for NP, where D is the number of dimensions of the problem
	//SDE sde = SDE(500, 25000, 0.8, 0.2, functions[3], true);
	//auto t1 = chrono::high_resolution_clock::now();
	//double runBest = sde.run();
	//auto t2 = chrono::high_resolution_clock::now();
	//auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
	//cout << "Execution time: " << ms_int.count() << " ms\n";
	//cout << "Correct digits count: " << countCorrectDigits(runBest) << '\n';

	return 0;
}


