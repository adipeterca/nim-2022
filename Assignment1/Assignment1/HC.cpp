#include "HC.h"

Individual HC::bestImprovement(Individual &individual) {
	vector<double> improvedSolution = individual.getVector(), bestSolution = individual.getVector();
	double mutationResult, bestResult = function(bestSolution.data());
	for (unsigned i = 0; i < function.getDimensions(); ++i) {
		double original = improvedSolution[i];
		improvedSolution[i] = improvedSolution[i] + pow(2, i) * epsilon;
		mutationResult = function(improvedSolution.data());
		if (mutationResult < bestResult) {
			bestSolution = improvedSolution;
			bestResult = mutationResult;
		}
		improvedSolution[i] = original;

		improvedSolution[i] = improvedSolution[i] - pow(2, i) * epsilon;
		mutationResult = function(improvedSolution.data());
		if (mutationResult < bestResult) {
			bestSolution = improvedSolution;
			bestResult = mutationResult;
		}
		improvedSolution[i] = original;
	}
	return bestSolution;
}

HC::HC(Individual& individual, Function& function, const double& epsilon, const unsigned& MAX_ITER) : individual(individual), function(function), epsilon(epsilon), MAX_ITER(MAX_ITER) {}

double HC::improve() {
	double bestResult = function(individual.get()), result;
	for (unsigned i = 0; i < MAX_ITER; ++i) {
		bool foundBetter = false;
		Individual better(individual);
		do {
			better = bestImprovement(better);
			result = function(better.get());
			if (result < bestResult) {
				bestResult = result;
				individual = better;
			}
			else {
				foundBetter = true;
			}
			cout << "aici2\n";
		} while (!foundBetter);
		epsilon *= 10;
	}
	return bestResult;
}