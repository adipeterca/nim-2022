#include "SDE.h"
#include "HC.h"

void SDE::initializePopulation() {
	population.clear();
	for (unsigned i = 0; i < POP_SIZE; ++i) {
		// Not very clean but this guarantees that every run of SDE (for every function) has different values
		population.push_back(Individual(function, random));
	}
}

const tuple<unsigned, unsigned, unsigned>  SDE::generateIndexes(const unsigned& index) {
	unsigned first, second, third;
	do {
		first = random.getRandomUnsigned(0, POP_SIZE - 1);
	} while (first == index);
	do {
		second = random.getRandomUnsigned(0, POP_SIZE - 1);
	} while (second == index || second == first);
	do {
		third = random.getRandomUnsigned(0, POP_SIZE - 1);
	} while (third == index || third == first || third == second);
	return make_tuple(first, second, third);
}

void SDE::sample(const unsigned&index, unsigned& first, unsigned& second, unsigned& third) {
	tuple<unsigned, unsigned, unsigned> x = generateIndexes(index);
	tie(first, second, third) = x;
}

const Individual SDE::mutation(const unsigned& first, const unsigned& second, const unsigned& third) {
	return population[first] - (population[second] - population[third]) * MUTATION_FACTOR;
}

vector<Individual> SDE::crossOver(const Individual& v) {
	unsigned D = function.getDimensions();
	vector<Individual> u;
	vector<double> result(D);
	for (unsigned i = 0; i < POP_SIZE; ++i) {
		unsigned jRand = random.getRandomUnsigned(0, D);
		for (unsigned j = 0; j < D; ++j) {
			if (j == jRand || random.getRandomDouble(0.0, 1.0) <= CROSS_OVER) {
				result[j] = v[j];
			}
			else {
				result[j] = population[i][j];
			}
		}
		u.push_back(Individual(result));
	}
	return u;
}

void SDE::evaluate(vector<Individual>& u) {
	for (unsigned i = 0; i < POP_SIZE; ++i) {
		if (function(u[i].get()) < function(population[i].get())) {
			population[i] = u[i];
			// cout << "Found better" << '\n';
		}
	}
}

SDE::SDE(const unsigned&NP, const unsigned&MAXFES, const double &F, const double &CR, Function& function, bool useHC) : POP_SIZE(NP), MAXFES(MAXFES), MUTATION_FACTOR(F), CROSS_OVER(CR), function(function), useHC(useHC) {}

double SDE::run(ofstream& outputFile) {
	unsigned first, second, third;
	initializePopulation();
	for (unsigned CFES = 0; CFES < MAXFES; CFES += POP_SIZE) {
		
		if (CFES % 10000 == 0) {
			cout << "Current iteration " << setw(10) << CFES << "/" << MAXFES << '\n';
		}
		for (unsigned i = 0; i < POP_SIZE; ++i) {

			sample(i, first, second, third);

			Individual v = mutation(first, second, third);

			vector<Individual> u = crossOver(v);
			evaluate(u);
		}
	}

	// Compute best from this run
	unsigned bestIndex = 0;
	double currentValue;
	double lowestValue = function(population[bestIndex].get()); // Best individual is the one with the lowest function value
	double highestValue = lowestValue; // Worst individual is the one with the highest function value
	double avgValue = lowestValue;

	for (unsigned i = 1; i < POP_SIZE; ++i) {
		currentValue = function(population[i].get());
		avgValue += currentValue;
		if (lowestValue > currentValue) {
			bestIndex = i;
			lowestValue = currentValue;
		}
		if (highestValue < currentValue) {
			highestValue = currentValue;
		}
	}
	avgValue = avgValue / POP_SIZE;

	cout << "SDE Results for function " << function.getName() << " :\n";
	cout << "Dimensions: " << function.getDimensions() << "\n";
	cout << "Interval: [" << function.getMin() << ", " << function.getMax() << "]\n";
	cout << "Lowest point coordinates (x) = " << population[bestIndex] << "\n";
	cout << "Lowest value for f(x) = " << lowestValue << "\n";
	cout << "Highest value for f(x) = " << highestValue << "\n";
	cout << "Average value for f(x) = " << avgValue << "\n";

	outputFile << "SDE Results for function " << function.getName() << " :\n";
	outputFile << "Dimensions: " << function.getDimensions() << "\n";
	outputFile << "Interval: [" << function.getMin() << ", " << function.getMax() << "]\n";
	outputFile << "Lowest point coordinates (x) = " << population[bestIndex] << "\n";
	outputFile << "Lowest value for f(x) = " << lowestValue << "\n";
	outputFile << "Highest value for f(x) = " << highestValue << "\n";
	outputFile << "Average value for f(x) = " << avgValue << "\n";
	if (useHC) {
		HC hillClimber = HC(population[bestIndex], function, 0.8, 10);
		double improvedResult = hillClimber.improve();
		cout << "Lowest point value after HC = " << improvedResult << "\n";
		outputFile << "Lowest point value after HC = " << improvedResult << "\n";
		if (improvedResult < lowestValue) {
			return improvedResult;
		}
	}
	return lowestValue;
}

Function& SDE::getFunction() const {
	return function;
}