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

void test(Function* f1, AbstractFunction* f2) {
	
	size_t noEvals = 1e5;
	vector<double> point;
	point.resize(f1->getDimensions());

	auto t1 = chrono::high_resolution_clock::now();
	cout << "Starting evaluating " << noEvals << " evaluations using OLD method\n";
	for (int i = 0; i < noEvals; i++) {
		for (int j = 0; j < point.size(); j++) {
			point[j] = getRandomDouble(f1->getMin(), f1->getMax());
		}
		(*f1)(point);
	}
	auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - t1).count();
	cout << "Evaluation took " << nanoseconds << " nanoseconds\n";

	size_t noThreads = 2;
	vector<thread> threads;
	cout << "Starting evaluating " << noEvals << " evaluations using NEW method (using " << noThreads << " threads)\n";
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
	nanoseconds = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - t1).count();
	cout << "Evaluation took " << nanoseconds << " nanoseconds\n";
}

int main()
{
	// Settings for cout
	cout << fixed << showpoint << setprecision(10);

	vector<Function> functions; // difficulty ranking
	functions.push_back(Function("Ackely", 10, 10, -100, 100)); // 1 done
	//functions.push_back(Function("Chebyshev", 1, 9, -8192, 8192)); // 2 done
	//functions.push_back(Function("Weierstrass", 6, 10, -100, 100)); // 3 done
	//functions.push_back(Function("Griewank", 5, 10, -100, 100)); // 4 done
	//functions.push_back(Function("Rastrigin", 4, 10, -100, 100)); // 5 done
	//functions.push_back(Function("Lennard Jones", 3, 18, -4, 4)); // 6 done
	//functions.push_back(Function("Hilbert", 2, 16, -16384, 16384)); // 7 done
	functions.push_back(Function("Schwefel", 7, 10, -100, 100));// 8 done
	//functions.push_back(Function("Escaffer 6", 8, 10, -100, 100)); // 9 done
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

	//Test t(&ackley, 10, -100, 100);
	//t.run(16);

	for (int i = 0; i < 10; i++) {
		//test(&functions[0], &ackley);
		test(&functions[1], &schwefel);
		cout << endl;
	}
	return 0;
}