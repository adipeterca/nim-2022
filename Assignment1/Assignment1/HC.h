#pragma once
#include <vector>
#include "Individual.h"
#include "Function.h"

using namespace std;

class HC {
private:
	Individual individual;
	Function& function;
	double epsilon;
	unsigned MAX_ITER;

	Individual bestImprovement(Individual individual);
public:
	HC(Individual &individual, Function& function, const double epsilon, const unsigned MAX_ITER) : individual(individual), function(function), epsilon(epsilon), MAX_ITER(MAX_ITER) {}

	double improve();
};