#pragma once
#include <vector>
#include <tuple>
#include "Individual.h"
#include "Function.h"
#include "Utils.h"


using namespace std;

/// <summary>
/// Standard Differential Evolution Algorithm
/// </summary>
class SDE
{
private:
	int POP_SIZE, MAXFES;
	double  MUTATION_FACTOR, CROSS_OVER;
	vector<Individual> population;
	Function& function;

	/// <summary>
	/// Initializes population
	/// </summary>
	void initializePopulation() {
		for (int i = 0; i < POP_SIZE; ++i) {
			population.push_back(Individual(function.getDimensions(), function.getMin(), function.getMax()));
		}
	}

	tuple<int, int, int> sample(int index) {
		int first, second, third;
		do {
			first = getRandomInteger(0, POP_SIZE);
		} while (first != index);
		do {
			second = getRandomInteger(0, POP_SIZE);
		} while (second != index && second != first);
		do {
			third = getRandomInteger(0, POP_SIZE);
		} while (third != index && third != first && third != second);
		return make_tuple(first, second, third);
	}

public:
	/// <summary>
	/// SDE Constructor
	/// </summary>
	/// <param name="NP">population size</param>
	/// <param name="MAXFES">maximum number of function evaluations</param>
	/// <param name="MAXFES">maximum number of function evaluations</param>
	/// <param name="F">mutation factor</param>
	/// <param name="CR">crossover probability</param>
	/// <param name="function">function to explore</param>
	SDE(int NP, int MAXFES, double F, double CR, Function& function) : function(function) {
		this->POP_SIZE = NP;
		this->MAXFES = MAXFES;
		this->MUTATION_FACTOR = F;
		this->CROSS_OVER = CR;
	}
	/// <summary>
	/// Runs SDE using specified parameters
	/// </summary>
	void run() {
		initializePopulation();
		for (int CFES = 0; CFES < MAXFES; CFES += POP_SIZE) {
			for (int i = 0; i < POP_SIZE; ++i) {
				tuple<double, double, double> x = sample(i);
				int first, second, third;
				tie(first, second, third) = x;
				//mutation
				//crossOver
				//evaluate
			}
		}
	}

};

