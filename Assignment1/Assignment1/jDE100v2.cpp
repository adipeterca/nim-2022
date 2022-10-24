#include "jDE100v2.h"

ParameterConstrains::ParameterConstrains(const double lowerBound, const double upperBound, const double initValue, const double chance) :
	lowerBound(lowerBound), upperBound(upperBound), initValue(initValue), chance(chance) {}



jDE100v2::jDE100v2(
	Function& function,
	ParameterConstrains& mutation,
	ParameterConstrains& crossover,
	const size_t bigPopulationSize,
	const size_t smallPopulationSize,
	const unsigned long long int maxFE) :

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
}

void jDE100v2::initPopulation(vector<Individual>& population, const size_t size) {
	population.clear();
	for (unsigned int i = 0; i < size; i++) {
		population.push_back(Individual(function));
	}
}

void jDE100v2::updatePopulation(vector<Individual>& population, vector<double>& mutationFactors, vector<double>& crossoverRates, bool performOnBigPopulation) {

	vector<Individual> newPopulation;
	newPopulation.resize(population.size());
	for (auto& indv : newPopulation) {
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

Individual* jDE100v2::findBestOverall(bool searchAgain) {
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

Individual* jDE100v2::findBest(vector<Individual>& population) {
	if (population.size() == 0) {
		throw exception("Tried to search in a zero-population for the best individual.");
	}
	size_t bestIndex = 0;
	for (size_t i = 1; i < population.size(); ++i) {
		if (function(population[bestIndex].get()) > function(population[i].get())) {
			bestIndex = i;
		}
	}
	return &population[bestIndex];
}

double jDE100v2::bestPercentage(vector<Individual>& population, const Individual* best) {
	double percentage = 0.0;
	for (auto& indv : population) {
		if (*best == indv) {
			percentage++;
		}
	}
	return percentage / population.size();
}

void jDE100v2::algorithm() {
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

	while (currentFE < maxFE) {
		currentGen++;
		if (currentGen % 1000 == 0) {
			// cout << "Currently at generation " << currentGen << "\n";
			if (currentGen % 10000 == 0) {
				cout << "Best so far: " << function(findBestOverall(true)->get()) << "\n";
				cout << "Currently at generation " << setw(15) << currentGen << " with currentFE at " << setw(15) << currentFE << "\n";
			}
			fout << "Best so far: " << function(findBestOverall(true)->get()) << " in generation " << currentGen << "\n";
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
			// cout << "Age limitation hit\n";
			fout << "Age limitation hit\n";
			initPopulation(bigPopulation, bigPopulationSize);
			age = 0;
		}
		if (bestPercentage(bigPopulation, bigBest) >= similarityPercentage) {
			// cout << "Similarity percentage hit\n";
			fout << "Similarity percentage hit\n";
			initPopulation(bigPopulation, bigPopulationSize);
		}

		// Check for a reinitialization of smallPopulation
		if (bestPercentage(smallPopulation, smallBest) >= similarityPercentage) {
			// cout << "Similarity percentage for small hit\n";
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

int jDE100v2::run() {
	
	fout.open("jDE100v2_" + function.getName() + ".txt");

	// No need to print to file, because its name already says what function it is.
	cout << "Created jDE100v2 instance with the following parameters:\n";
	cout << "Function to evaluate: " + function.getName() + "\n"; 
	
	// Run the algorithm
	auto t1 = chrono::high_resolution_clock::now();
	algorithm();
	Individual best = *findBestOverall(true);
	auto t2 = chrono::high_resolution_clock::now();
	size_t seconds = chrono::duration_cast<chrono::seconds>(t2 - t1).count();
	cout << "Overall time: " << seconds / 60 << " minutes and " << seconds % 60 << " seconds.\n";

	double value = function(best.get());
	int correctDigits = countCorrectDigits(value);
	cout << "Best value found: " << value << ", with a total number of correct digits of " << correctDigits << " / 10\n";

	return correctDigits;
}