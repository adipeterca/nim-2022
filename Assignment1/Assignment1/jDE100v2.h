#ifndef JDE100_V2_H
#define JDE100_V2_H

#include <iostream>
#include <fstream>
#include "Function.h"
#include "Individual.h"

/// <summary>
/// Structure used to hold the necessary information for each parameter of the jDE algorithm (mutation and crossover).
/// </summary>
struct ParameterConstrains {
	const double lowerBound;
	const double upperBound;
	const double initValue;
	const double chance;
	/// <summary>
	/// Constructs an object which cannot be modified later.
	/// </summary>
	/// <param name="lowerBound">the lower bound of the parameter's value (inclusive)</param>
	/// <param name="upperBound">the upper bound of the parameter's value (inclusive)</param>
	/// <param name="initValue">the initial value of the parameter</param>
	/// <param name="chance">the individual chance to be modified (usually 0.1)</param>
	ParameterConstrains(const double lowerBound, const double upperBound, const double initValue, const double chance);
};

/// <summary>
/// Class implemeting the jDE100 algorithm.
/// </summary>
class jDE100v2 {

private:
	vector<Individual> bigPopulation;
	vector<Individual> smallPopulation;

	// Pointers are used in favor of Individual objects because we don't need a copy of the best individual,
	// we just need to know who is that one.
	// REFACTOR_IDEA: can be refactored later to be only an index value and not a pointer.
	Individual* bigBest = nullptr;
	Individual* smallBest = nullptr;

	size_t bigPopulationSize;
	size_t smallPopulationSize;

	vector<double> bigMutationFactors;
	vector<double> smallMutationFactors;
	ParameterConstrains mutationConstrains;

	vector<double> bigCrossoverRates;
	vector<double> smallCrossoverRates;
	ParameterConstrains crossoverConstrains;

	// Function to evaluate
	Function function;

	// Maximum number of function evaluations (this is a stopping criterion for the overall algorithm)
	const unsigned long long int maxFE;

	// The multiplier between bigPopulatione and smallPopulation vector sizes
	const size_t M;

	// Used for reinitialization (values taken from the research paper)
	const double similarityPercentage = 0.25;
	const size_t ageLimitation = 1e9;

	// For overall logging & debugging information (creates a file based on the function's name)
	ofstream fout;

	/// <summary>
	/// Given a population vector, it clears it and add --size-- Individual objects, based on the current --function--.
	/// </summary>
	/// <param name="population">reference to the population which needs initialization</param>
	/// <param name="size">number of Individual objects to add</param>
	void initPopulation(vector<Individual>& population, const size_t size);
	
	/// <summary>
	/// Given a population, it performs the usual jDE mutation, crossover and selection,
	/// in the end updating the given population to the newly created one.
	/// </summary>
	/// <param name="population">reference for the population vector</param>
	/// <param name="mutationFactors">the mutation factors for the current population</param>
	/// <param name="crossoverRates">the crossover rates for the current po;ulation</param>
	/// <param name="performOnBigPopulation">should the small population have an influence on the big population? (defaults to true)</param>
	void updatePopulation(vector<Individual>& population, vector<double>& mutationFactors, vector<double>& crossoverRates, bool performOnBigPopulation = true);

	/// <summary>
	/// Find overall best Individual object.
	/// </summary>
	/// <param name="searchAgain">compare the current available bests from both population (faster) or search again both populations (slower)?</param>
	/// <returns>a pointer to the overall best Individual object</returns>
	Individual* findBestOverall(bool searchAgain = false);
	
	/// <summary>
	/// Find the best Individual in the given population
	/// </summary>
	/// <param name="population">the given population</param>
	/// <returns>a pointer to the best Individual object from the specified population</returns>
	Individual* findBest(vector<Individual>& population);

	/// <summary>
	/// Returns the percentage of individuals equal to the given one.
	/// </summary>
	/// <param name="population">the population in which to search for</param>
	/// <param name="best">Individual object to compare agains</param>
	/// <returns>percentage (between 0.0-100.0) of Individual objects that are equal to the specified one.</returns>
	double bestPercentage(vector<Individual>& population, const Individual* best);

	/// <summary>
	/// Placeholder function to organize the overall algorithm (should be called just once!).
	/// </summary>
	void algorithm();

public:

	/// <summary>
	/// Constructs the overall algorithm, setting parameters but NOT starting a run.
	/// Default values are taken from the research paper.
	/// </summary>
	/// <param name="function">the function tied to this jDE instance</param>
	/// <param name="mutation">object holding mutation information (Fl, Fu, Finit, Fchance)</param>
	/// <param name="crossover">object holding crossover information (CRl, CRu, CRinit, Cchance)</param>
	/// <param name="bigPopulationSize">size of the big population (denoted bNP)</param>
	/// <param name="smallPopulationSize">size of the small population (denoted sNP)</param>
	/// <param name="maxFE">maximum number of function evaluations</param>
	jDE100v2(Function& function,
			ParameterConstrains& mutation,
			ParameterConstrains& crossover,
			const size_t bigPopulationSize = 1000,
			const size_t smallPopulationSize = 25,
			const unsigned long long int maxFE = 1e12);


	/// <summary>
	/// Placeholder function that runs the behind-the-scenes algorithm, while also handling
	/// stuff like information logging and (in a future version) error handling (lolz).
	/// </summary>
	/// <returns>number of correct digits for the final solution</returns>
	int run();
};

#endif