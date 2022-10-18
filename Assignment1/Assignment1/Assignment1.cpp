#include <math.h>
#include "cec19_func.h"
#include "SDE.h"
#pragma warning(disable:4996)

// C++ migration
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

extern double* OShift, * M, * y, * z, * x_bound;
extern int ini_flag, n_flag, func_flag, * SS;

/// <summary>
/// Quick testing function to understand the input data for Rastrigin function.
/// </summary>
void testValue()
{
	const int dims = 10;
	double x[dims];

	std::ifstream fin("input_data/shift_data_4.txt");
	for (int i = 0; i < dims; i++)
		fin >> x[i];

	double returnValue;
	cec19_test_func(x, &returnValue, dims, 1, 4);

	std::cout << returnValue << std::endl;
}

/// <summary>
/// Runs the initial given sample code for the specified function.
/// </summary>
/// <param name="currentFunction">the specified function. Values between 1-10, defaults to 4 (Rastrigin).</param>
void testFunction(int currentFunction =4)
{
	const auto n = 10;
	const auto m = 2;

	double* x = new double[m * n * sizeof(double)];
	double* f = new double[m * sizeof(double)];

	std::ifstream fin("input_data/shift_data_" + std::to_string(currentFunction) + ".txt");
	if (!fin)
	{
		std::cout << "Error: Cannot open input file for reading: input_data/shift_data_" + std::to_string(currentFunction) + ".txt\n";
		return;
	}

	std::cout << "Lowest point: ";
	for (int k = 0; k < n; k++)
	{
		fin >> x[k];
		std::cout << x[k] << " ";
	}
	std::cout << std::endl;

	fin.close();

	for (int j = 0; j < n; j++)
	{
		// why is it 1*n?
		// LE: we skip pass the first N numbers because we already read them ^
		//x[1 * n + j] = 0.0;
		//printf("%Lf\n",x[1*n+j]);

		x[n + j] = 0.0;
	}

	cec19_test_func(x, f, n, m, currentFunction);
	std::cout << "Result for lowest point: " << f[0] << std::endl;
	std::cout << "Result for all zero point: " << f[1] << std::endl;
}

int main()
{
	// Settings for cout
	//std::cout << std::fixed << std::showpoint;
	//std::cout << std::setprecision(5);

	//testValue();
	//testFunction();
	Function rastrigin = Function(4, 10, -100, 100);
	SDE sde = SDE(100, 100, 1.0, 0.5, rastrigin);
	sde.run();

	return 0;
}


