#include <math.h>
#include "cec19_func.h"
#pragma warning(disable:4996)

// C++ migration
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

extern double* OShift, * M, * y, * z, * x_bound;
extern int ini_flag, n_flag, func_flag, * SS;


int main()
{
	// Settings for cout
	std::cout << std::fixed << std::showpoint;
	std::cout << std::setprecision(5);

	// Ranges from 1 to 10
	int currentFunction;

	const auto n = 10;
	const auto m = 2;

	double* x = new double[m * n * sizeof(double)];
	double* f = new double[m * sizeof(double)];

	std::ifstream fin;
	std::string filename;

	for (int i = 3; i < 4; i++)
	{
		currentFunction = i + 1;
		filename = "input_data/shift_data_" + std::to_string(currentFunction) + ".txt";
		fin.open(filename);
		if (!fin)
		{
			std::cout << "Error: Cannot open input file for reading: " + filename + "\n";
			return -1;
		}

		// x - vector for shift_data
		for (int k = 0; k < n; k++)
		{
			fin >> x[k];
			std::cout << "Read for shift_data: " << x[k] << std::endl;
		}

		fin.close();

		for (int j = 0; j < n; j++)
		{
			// why is it 1*n?
			// LE: we skip pass the first N numbers because we already read them ^
			//x[1 * n + j] = 0.0;
			//printf("%Lf\n",x[1*n+j]);

			x[n + j] = 0.0;
			std::cout << "second for: " << x[n + j] << std::endl;
		}

		// Why k=0,1?
		for (int k = 0; k < 1; k++)
		{

			cec19_test_func(x, f, n, m, currentFunction);
			for (int j = 0; j < 2; j++)
			{
				printf(" f%d(x[%d]) = %Lf,", currentFunction, j + 1, f[j]);
			}
			std::cout << std::endl;
		}

	}

	return 0;
}


