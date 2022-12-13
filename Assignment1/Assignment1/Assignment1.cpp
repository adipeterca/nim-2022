#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <thread>
#include "cec19_func.h"
#include "Utils.h"
#include "jDE100v2.h"
#include "PSO.h"
#pragma warning(disable:4996)
#define NO_EVALS 10000000
#define NO_THREADS 16
#define NO_ITERATIONS 10

using namespace std;

class Algorithm {
protected:
	const AbstractFunction* function;
	const double min;
	const double max;
	const size_t dimensions;

	virtual void singleRun(int x) = 0;

public:

	Algorithm(AbstractFunction* function, size_t dimensions, double min, double max) : function(function), dimensions(dimensions), min(min), max(max) {

	}

	virtual void run(size_t noThreads) = 0;
};

class Test : public Algorithm {
private:
	void singleRun(int x) override final {
		for (int i = 0; i < 100; i++) {
			// Construct an output object to guarantee exclusiveness when printing to stdout
			string s = "x = " + to_string(x) + " and i = " + to_string(i) + "\n";
			cout << s;
		}
	}

public:
	Test(AbstractFunction* function, size_t dimensions, double min, double max) : Algorithm(function, dimensions, min, max) {

	}

	void run(size_t noThreads) override final {
		vector<thread> threads;
		for (size_t i = 0; i < noThreads; i++) {
			threads.push_back(thread(&Test::singleRun, this, i));
		}

		for (size_t i = 0; i < noThreads; i++) {
			threads[i].join();
		}
	}
};

void test(Function* f1, AbstractFunction* f2, ofstream & fout) {

	size_t noEvals = NO_EVALS;
	vector<double> point;
	point.resize(f1->getDimensions());

	fout << "Starting evaluating using OLD method\n";
	auto t1 = chrono::high_resolution_clock::now();
	for (int i = 0; i < noEvals; i++) {
		for (int j = 0; j < point.size(); j++) {
			point[j] = getRandomDouble(f1->getMin(), f1->getMax());
		}
		(*f1)(point);
	}
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - t1).count();
	fout << "Evaluation took " << milliseconds << " milliseconds\n";

	size_t noThreads = NO_THREADS;
	vector<thread> threads;
	fout << "Starting evaluating using NEW method\n";
	t1 = chrono::high_resolution_clock::now();

	auto func = [&f1, &f2](int iters) {
		vector<double> point;
		point.resize(f1->getDimensions());

		for (int i = 0; i < iters; i++) {
			for (int j = 0; j < point.size(); j++) {
				point[j] = getRandomDouble(f1->getMin(), f1->getMax());
			}

			(*f2)(point);
		}
	};

	for (int i = 0; i < noThreads; i++) {
		threads.push_back(thread(func, noEvals / noThreads));
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}
	milliseconds = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - t1).count();
	fout << "Evaluation took " << milliseconds << " milliseconds\n";
}

void test2(Function* f1, AbstractFunction* f2, long double& _old, long double& _new) {
	// How many function evaluations to take into account
	size_t noEvals = NO_EVALS;
	vector<vector<double>> points;
	points.resize(noEvals);
	for (auto& point : points) {
		for (int j = 0; j < f1->getDimensions(); j++) {
			point.push_back(getRandomDouble(f1->getMin(), f1->getMax()));
		}
	}
	size_t noThreads = NO_THREADS;
	vector<thread> threads;

	auto t1 = chrono::high_resolution_clock::now();
	for (auto& point : points) {
		(*f1)(point);
	}
	_old = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - t1).count();

	t1 = chrono::high_resolution_clock::now();
	auto func = [&f1, &f2, &points](int start, int len) {
		for (int i = start; i < start + len; i++) {
			(*f2)(points[i]);
		}
	};

	// How many evaluations should each thread take
	int evals = noEvals / noThreads;

	for (int i = 0; i < noThreads; i++) {
		threads.push_back(thread(func, i * evals, evals));
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}
	_new = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - t1).count();
}

int main()
{
	// Settings for cout
	//cout << fixed << showpoint << setprecision(10);

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

	long double old_sum = 0, new_sum = 0, _old = 0, _new = 0;
	ofstream fout("results_" + to_string(NO_THREADS) + ".txt");
	fout << fixed << showpoint << setprecision(10);
	AbstractFunction* f = nullptr;
	fout << "Number of evaluations: " << NO_EVALS << "\n" << "Number of threads: " << NO_THREADS << "\n" << "Number of iterations: " << NO_ITERATIONS << "\n\n";
	for (int j = 0; j < 1; ++j) {
		if (j == 0) {
			f = &ackley;
			fout << "Testing function Ackley" << '\n';
		}
		else if (j == 1) {
			f = &chebyshev;
			fout << "Testing function Chebyshev" << '\n';
		}
		else if (j == 2) {
			f = &weierstrass;
			fout << "Testing function Weierstrass" << '\n';
		}
		else if (j == 3) {
			f = &griewank;
			fout << "Testing function Griewank" << '\n';
		}
		else if (j == 4) {
			f = &rastrigin;
			fout << "Testing function Rastrigin" << '\n';
		}
		else if (j == 5) {
			f = &lennardJones;
			fout << "Testing function Lennard Jones" << '\n';
		}
		else if (j == 6) {
			f = &hilbert;
			fout << "Testing function Hilbert" << '\n';
		}
		else if (j == 7) {
			f = &schwefel;
			fout << "Testing function Schwefel" << '\n';
		}
		else if (j == 8) {
			f = &escaffer6;
			fout << "Testing function Escaffer 6" << '\n';
		}
		else if (j == 9) {
			f = &happyCat;
			fout << "Testing function Happy Cat" << '\n';
		}
		old_sum = 0;
		new_sum = 0;
		for (int i = 0; i < 10; i++) {
			test2(&functions[j], f, _old, _new);
			old_sum += _old;
			new_sum += _new;
		}
		fout << "Evaluation OLD took " << old_sum / NO_ITERATIONS << " milliseconds\n";
		fout << "Evaluation NEW took " << new_sum / NO_ITERATIONS << " milliseconds\n";
	}
	return 0;
}