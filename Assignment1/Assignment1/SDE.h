#pragma once
#include <vector>
#include <tuple>
#include "Individual.h"
#include "Function.h"
#include "Random.h"


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

	/// <summary>
	/// Generates three random distinct positions from population vector with first != second != third != index
	/// </summary>
	/// <param name="index">index</param>
	/// <returns>tuple containing positions</returns>
	tuple<int, int, int> generateIndexes(int index) {
		int first, second, third;
		do {
			first = Random::getRandomInteger(0, POP_SIZE);
		} while (first == index);
		do {
			second = Random::getRandomInteger(0, POP_SIZE);
		} while (second == index || second == first);
		do {
			third = Random::getRandomInteger(0, POP_SIZE);
		} while (third == index || third == first || third == second);
		return make_tuple(first, second, third);
	}

	/// <summary>
	/// Passes as reference three random distinct positions from population vector with first != second != third != index
	/// </summary>
	/// <param name="index">index</param>
	/// <param name="first">first position</param>
	/// <param name="seccond">second position</param>
	/// <param name="third">third position</param>
	void sample(int index, int& first, int& seccond, int& third) {
		tuple<double, double, double> x = generateIndexes(index);
		tie(first, seccond, third) = x;
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
		int first, second, third;
		initializePopulation();
		for (int CFES = 0; CFES < MAXFES; CFES += POP_SIZE) {
			for (int i = 0; i < POP_SIZE; ++i) {
				//sampling
				sample(i, first, second, third);
				//mutation
				Individual v = population[first] - (population[second] - population[third]) * MUTATION_FACTOR;
				//crossOver
				//evaluate
			}
		}
	}

};

