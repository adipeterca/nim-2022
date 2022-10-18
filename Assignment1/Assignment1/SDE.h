#pragma once
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <iomanip>
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
	const int POP_SIZE;
	const int MAXFES;
	const double MUTATION_FACTOR;
	const double CROSS_OVER;

	Random rng;

	vector<Individual> population;
	Function& function;


	/// <summary>
	/// Creates the starting population, overriding the existing one (if any).
	/// </summary>
	void initializePopulation();

	/// <summary>
	/// Generates three random distinct positions from population vector with first != second != third != index
	/// </summary>
	/// <param name="index">index</param>
	/// <returns>tuple containing positions</returns>
	const tuple<int, int, int> generateIndexes(const int& index);

	/// <summary>
	/// Passes as reference three random distinct positions from population vector with first != second != third != index
	/// </summary>
	/// <param name="index">index</param>
	/// <param name="first">first position</param>
	/// <param name="second">second position</param>
	/// <param name="third">third position</param>
	void sample(const int& index, int& first, int& second, int& third);

	/// <summary>
	/// Generates individual using a mutation scheme
	/// </summary>
	/// <param name="first">first individual</param>
	/// <param name="second">second individual</param>
	/// <param name="third">third individual</param>
	/// <returns>new individual</returns>
	const Individual mutation(const int& first, const int& second, const int& third);

	/// <summary>
	/// Generates a vector of individuals using a cross-over scheme
	/// </summary>
	/// <param name="v">donor vector</param>
	/// <returns>vector of individuals</returns>
	vector<Individual> crossOver(const Individual& v);

	/// <summary>
	/// Compares vector of individuals to existing population element-wise
	/// </summary>
	/// <param name="u">vector of individuals</param>
	void evaluate(vector<Individual>& u);

public:
	/// <summary>
	/// SDE Constructor
	/// </summary>
	/// <param name="NP">population size</param>
	/// <param name="MAXFES">maximum number of function evaluations</param>
	/// <param name="F">mutation factor</param>
	/// <param name="CR">crossover probability</param>
	/// <param name="function">function to explore</param>
	SDE(const int& NP, const int& MAXFES, const double& F, const double& CR, Function& function);

	/// <summary>
	/// Runs SDE using specified parameters
	/// </summary>
	void run();
};

