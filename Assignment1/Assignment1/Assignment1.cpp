#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>
#include "cec19_func.h"
#include "SDE.h"
#include "jDE100.h"
#include "Utils.h"
#pragma warning(disable:4996)

using namespace std;

extern double* OShift, * M, * y, * z, * x_bound;
extern int ini_flag, n_flag, func_flag, * SS;


/// <summary>
/// Function used to count the correct digits. The answer is always 1.000000000.
/// </summary>
/// <param name="value">value whose digits are counted</param>
/// <returns>number of correct digits</returns>
int countCorrectDigits(double value) {
	
	ostringstream os;
	os << fixed << setprecision(9) << value;
	string strValue = os.str();

	// If the first is not 1, all values are wrong.
	if (strValue[0] != '1') return 0;

	int result = 1;
	for (int i = 2; i < strValue.size(); ++i) {
		if (strValue[i] == '0')
			result++;
		else
			break;
	}
	return result;

	/* Tested on:
	* 2.000000000 -> returns 0
	* 1.924235666 -> returns 1
	* 1.003243567 -> returns 3
	* 1.000000000 -> returns 10
	*/
}

struct ParameterConstrains {
	const double lowerBound;
	const double upperBound;
	const double initValue;
	const double chance;
	ParameterConstrains(const double lowerBound, const double upperBound, const double initValue, const double chance) :
		lowerBound(lowerBound), upperBound(upperBound), initValue(initValue), chance(chance) {}
};

class jDE2 {
	
private:
	vector<Individual> bigPopulation;
	vector<Individual> smallPopulation;

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
	
	// Maximum number of function evaluations (this is a stopping criterion)
	const unsigned long long int maxFE;

	// The multiplier between bigPopulatione and smallPopulation vector sizes
	const size_t M;

	// Used for reinitialization (values taken from the research paper)
	const double similarityPercentage = 0.25;
	const size_t ageLimitation = 1e9;

	// For overall logging
	ofstream fout;

public:
	

	jDE2(
		Function& function,
		ParameterConstrains& mutation,
		ParameterConstrains& crossover,
		const size_t bigPopulationSize = 1000,
		const size_t smallPopulationSize = 25,
		const unsigned long long int maxFE = 1e12
	) :
		function(function),
		mutationConstrains(mutation),
		crossoverConstrains(crossover),
		bigPopulationSize(bigPopulationSize),
		smallPopulationSize(smallPopulationSize),
		maxFE(maxFE),
		M(bigPopulationSize / smallPopulationSize)
	{
		if (bigPopulationSize % smallPopulationSize != 0) {
			cout << "Warning! bigPopSize is not a multiple of smallPopSize!\n";
		}

		// Overall settings for better understanding of the algorithm
		fout.open("jDE_run.txt");
		string output;
		output += "Created jDE100_2 instance with the following parameters:\n";
		output += "Function to evaluate: " + function.getName() + "\n";
		// TO DO!
	}

	void initPopulation(vector<Individual>& population, const size_t size) {
		population.clear();
		for (unsigned int i = 0; i < size; i++) {
			population.push_back(Individual(function));
		}
	}

	// Given a population, it performs the usual jDE mutation, crossover and selection,
	// in the end updating the given population to the newly created one.
	// NOTE: performOnBigPopulation SHALL BE USED LATER (to determine r2 and r3 JUST FOR THE BIG POP)!
	void updatePopulation(vector<Individual>& population, vector<double>& mutationFactors, vector<double>& crossoverRates, bool performOnBigPopulation = true) {
		
		vector<Individual> newPopulation;
		newPopulation.resize(population.size());
		for (auto &indv : newPopulation) {
			indv.resize(function);
		}

		for (size_t i = 0; i < population.size(); ++i) {

			double newMutationFactor;
			double newCrossoverRate;

			// Mutation factor update
			double rand_1 = getRandomDouble(0, 1);
			double rand_2 = getRandomDouble(0, 1);
			if (rand_2 < mutationConstrains.chance) {
				newMutationFactor = mutationConstrains.lowerBound + rand_1 * mutationConstrains.upperBound;
			}
			else {
				newMutationFactor = mutationFactors[i];
			}

			// Apply mutation
			Individual v;
			if (performOnBigPopulation) {
				size_t r1, r2, r3;
				do {
					r1 = getRandomUnsigned(0, bigPopulationSize - 1);
					r2 = getRandomUnsigned(0, bigPopulationSize);
					r3 = getRandomUnsigned(0, bigPopulationSize);
				} while (r1 == r2 || r1 == r3 || r1 == i || r2 == r3 || r2 == i || r3 == i);
			
				if (r2 == bigPopulationSize || r3 == bigPopulationSize) {
					// This time the elements will be different nontheless, so there is no need to do-while
					r2 = getRandomUnsigned(0, smallPopulationSize - 1);
					r3 = getRandomUnsigned(0, smallPopulationSize - 1);
					v = bigPopulation[r1] + (smallPopulation[r2] - smallPopulation[r3]) * newMutationFactor;
				}
				else {
					v = bigPopulation[r1] + (bigPopulation[r2] - bigPopulation[r3]) * newMutationFactor;
				}

			}
			else {
				size_t r1, r2, r3;
				do {
					r1 = getRandomUnsigned(0, population.size() - 1);
					r2 = getRandomUnsigned(0, population.size() - 1);
					r3 = getRandomUnsigned(0, population.size() - 1);
				} while (r1 == r2 || r1 == r3 || r1 == i || r2 == r3 || r2 == i || r3 == i);

				v = population[r1] + (population[r2] - population[r3]) * newMutationFactor;
			}
			

			// Crossover factor update
			double rand_3 = getRandomDouble(0, 1);
			double rand_4 = getRandomDouble(0, 1);
			if (rand_4 < crossoverConstrains.chance) {
				newCrossoverRate = rand_3;
			}
			else {
				newCrossoverRate = crossoverRates[i];
			}

			// Apply crossover
			size_t jRand = getRandomUnsigned(1, function.getDimensions() - 1); // Why [1, D-1] and not [0, D-1]?
			for (size_t j = 0; j < function.getDimensions(); ++j) {
				if (getRandomDouble(0, 1) <= newCrossoverRate || j == jRand) {
					newPopulation[i][j] = v[j];
				}
				else {
					newPopulation[i][j] = population[i][j];
				}
			}

			// newPopulation[i] stands for the u individual
			if (function(newPopulation[i].get()) < function(population[i].get())) {
				// newPopulation[i] stays the same
				mutationFactors[i] = newMutationFactor;
				crossoverRates[i] = newCrossoverRate;
			}
			else {
				newPopulation[i] = population[i];
				// mutationFactors[i] stays the same
				// crossoverRates[i] stays the same
			}

		}
		population = newPopulation;
		
	}

	// Find overall best Individual object
	Individual* findBestOverall(bool searchAgain = false) {
		if (bigBest == nullptr) {
			bigBest = findBest(bigPopulation);
			cout << "bigBest was not initialized, but I handled that for you.\n";
		}
		if (smallBest == nullptr) {
			smallBest = findBest(smallPopulation);
			cout << "smallBest was not initialized, but I handled that for you.\n";
		}
		if (searchAgain) {
			bigBest = findBest(bigPopulation);
			smallBest = findBest(smallPopulation);
		}
		return (function(bigBest->get()) < function(smallBest->get()) ? bigBest : smallBest);
	}

	// Finds the best index in the given population
	Individual* findBest(vector<Individual>& population) {
		size_t bestIndex = 0;
		for (size_t i = 1; i < population.size(); ++i) {
			if (function(population[bestIndex].get()) > function(population[i].get())) {
				bestIndex = i;
			}
		}
		return &population[bestIndex];
	}

	// Returns the percentage of individuals equal to the given best
	double bestPercentage(vector<Individual>& population, const Individual* best) {
		double percentage = 0.0;
		for (auto& indv : population) {
			if (*best == indv) {
				percentage++;
			}
		}
		return percentage / population.size();
	}

	void algorithm() {
		initPopulation(bigPopulation, bigPopulationSize);
		bigBest = findBest(bigPopulation);

		initPopulation(smallPopulation, smallPopulationSize);
		smallBest = findBest(smallPopulation);

		bigMutationFactors = vector<double>(bigPopulationSize, mutationConstrains.initValue);
		bigCrossoverRates = vector<double>(bigPopulationSize, crossoverConstrains.initValue);
		smallMutationFactors = vector<double>(smallPopulationSize, mutationConstrains.initValue);
		smallCrossoverRates = vector<double>(smallPopulationSize, crossoverConstrains.initValue);

		Individual* bestOverall = findBestOverall();
		
		size_t currentGen = 0;
		size_t currentFE = 0;
		size_t age = 0;

		cout << "Currently evaluating function " << function.getName() << "\n";
		fout << "Currently evaluating function " << function.getName() << "\n";
		
		while (currentFE < maxFE) {
			currentGen++;
			if (currentGen % 1000 == 0) {
				// cout << "Currently at generation " << currentGen << "\n";
				cout << "Best so far: " << function(findBestOverall(true)->get()) << "\n";
				fout << "Best so far: " << function(findBestOverall(true)->get()) << "\n";
				fout.flush();
				if (function(smallBest->get()) - 1.000000000 <= EPSILON || function(bigBest->get()) - 1.000000000 <= EPSILON) {
					break;
				}
			}
			// Conditions for a reinitialization of bigPopulation:
			// - either similarityPercentage% of the best individuals share very close values (meaning that the best
			//		individual is equal to similarityPercentage% of Individuals from the population)
			// - a certain threshold of function evaluations was touched, in which the best candidate did not improve
			//		(when a new best individual is found, age becomes 0)
			if (age >= ageLimitation) {
				cout << "Age limitation hit\n";
				fout << "Age limitation hit\n";
				initPopulation(bigPopulation, bigPopulationSize);
				age = 0;
			}
			if (bestPercentage(bigPopulation, bigBest) >= similarityPercentage) {
				cout << "Similarity percentage hit\n";
				fout << "Similarity percentage hit\n";
				initPopulation(bigPopulation, bigPopulationSize);
			}

			// Check for a reinitialization of smallPopulation
			if (bestPercentage(smallPopulation, smallBest) >= similarityPercentage) {
				cout << "Similarity percentage for small hit\n";
				fout << "Similarity percentage for small hit\n";
				Individual prevBest = *smallBest;
				initPopulation(smallPopulation, smallPopulationSize);
				smallPopulation[0] = prevBest;
			}

			updatePopulation(bigPopulation, bigMutationFactors, bigCrossoverRates);
			currentFE += bigPopulationSize;

			// Is the best Individual present in the bigPopulation?

			if (bigBest == findBestOverall()) {
				// The overhead for calculating the weakest element in smallPopulation
				// is not worth it, so just override the first element.
				smallPopulation[0] = *bestOverall;
				smallBest = &smallPopulation[0];
			}

			for (size_t k = 0; k < M; ++k) {
				updatePopulation(smallPopulation, smallMutationFactors, smallCrossoverRates, false);
				currentFE += smallPopulationSize; // Could be optimized, but I find it clearer this way.
			}
			
		}
		
		
	}

	// Runs the algorithm, while also handling the results.
	void run() {
		algorithm();
		Individual best = *findBestOverall(true);
		cout << function(best.get()) << "\n";
	}
};


int main()
{
	// Settings for cout
	cout << fixed << showpoint << setprecision(10);

	/*ofstream fout("output.txt");
	fout << fixed << showpoint << setprecision(10);*/

	vector<Function> functions; // difficulty ranking
	functions.push_back(Function("Chebyshev", 1, 9, -8192, 8192)); // 2
	functions.push_back(Function("Hilbert", 2, 16, -16384, 16384)); // 7
	functions.push_back(Function("Lennard Jones", 3, 18, -4, 4)); // 6
	functions.push_back(Function("Rastrigin", 4, 10, -100, 100)); // 5
	functions.push_back(Function("Griewank", 5, 10, -100, 100)); // 4
	functions.push_back(Function("Weierstrass", 6, 10, -100, 100)); // 3
	functions.push_back(Function("Schwefel", 7, 10, -100, 100));// 8
	functions.push_back(Function("Escaffer 6", 8, 10, -100, 100)); // 9
	functions.push_back(Function("Happy Cat", 9, 10, -100, 100)); // 10
	functions.push_back(Function("Ackely", 10, 10, -100, 100)); // 1
	
	ParameterConstrains mutation(0.1, 0.9, 0.5, 0.1);
	ParameterConstrains crossover(0.1, 1.1, 0.5, 0.1);


	auto t0 = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10; i++) {
		
		jDE2 alg(functions[i], mutation, crossover);
		alg.run();
		auto t1 = chrono::high_resolution_clock::now();
		int seconds = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() / 1000;
		cout << "finished current function in " << seconds / 60 << ":" << seconds % 60 << "\n";
	}


	// Idei de improvement:
	// unordered_map pentru functii in loc de vectors
	// problema apare cand sunt prea multe puncte (de obicei dupa 1e10)
	
	return 0;
}



	//vector<SDE> algorithms;
	//algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[0], true)); // 2
	//algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[1], true)); // 7
	//algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[2], true)); // 6
	//algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[3], true)); // 5
	//algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[4], true)); // 4
	//algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[5], true)); // 3
	//algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[6], true)); // 8
	//algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[7], true)); // 9
	//algorithms.push_back(SDE(1000, 1000000, 0.8, 0.2, functions[8], true)); // 10
	//algorithms.push_back(SDE(1000, 1000000, 0.4, 0.2, functions[9], true)); // 1

	//unsigned overallAccuracy = 0;
	//unsigned currentAccuracy;
	//auto t0 = chrono::high_resolution_clock::now();
	//for (auto alg : algorithms) {
	//	cout << "Currently evaluating function " << alg.getFunction().getName() << "\n";
	//	fout << "Currently evaluating function " << alg.getFunction().getName() << "\n";
	//	
	//	if (alg.getFunction().getName() == "Chebyshev") {
	//		cout << "Skipped Chebyshev.\n";
	//		fout << "Skipped Chebyshev.\n";
	//		continue;
	//	}

	//	auto t1 = chrono::high_resolution_clock::now();
	//	double runBest = alg.run(fout);
	//	auto t2 = chrono::high_resolution_clock::now();
	//	auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
	//	currentAccuracy = countCorrectDigits(runBest);
	//	
	//	long long seconds = ms_int.count() / 1000;
	//	cout << "Execution time: " << seconds / 60 << " minutes and " << seconds % 60 << " seconds.\n";
	//	fout << "Execution time: " << seconds / 60 << " minutes and " << seconds % 60 << " seconds.\n";
	//	cout << "Correct digits count: " << currentAccuracy << "\n\n\n";
	//	fout << "Correct digits count: " << currentAccuracy << "\n\n\n";

	//	fout.flush();
	//	overallAccuracy += currentAccuracy;
	//}
	//auto t3 = chrono::high_resolution_clock::now();
	//long long seconds = chrono::duration_cast<chrono::seconds>(t3 - t0).count();


	//cout << "---------------------------------------------------------------------------\n";
	//cout << "Overall accuracy: " << setw(10) << overallAccuracy << "/ 100\n";
	//cout << "Overall time: " << setw(10) << seconds / 60 << " minutes and " << seconds % 60 << " seconds\n";

	//fout << "---------------------------------------------------------------------------\n";
	//fout << "Overall accuracy: " << setw(10) << overallAccuracy << "/ 100\n";
	//fout << "Overall time: " << setw(10) << seconds / 60 << " minutes and " << seconds % 60 << " seconds\n";