#include "SDE.h"

void SDE::initializePopulation() {
	population.clear();
	for (int i = 0; i < POP_SIZE; ++i) {
		// Not very clean but this guarantees that every run of SDE (for every function) has different values
		population.push_back(Individual(function, rng));
	}
}

const tuple<int, int, int>  SDE::generateIndexes(const int& index) {
	int first, second, third;
	do {
		first = rng.getRandomInteger(0, POP_SIZE - 1);
	} while (first == index);
	do {
		second = rng.getRandomInteger(0, POP_SIZE - 1);
	} while (second == index || second == first);
	do {
		third = rng.getRandomInteger(0, POP_SIZE - 1);
	} while (third == index || third == first || third == second);
	return make_tuple(first, second, third);
}

void SDE::sample(const int &index, int& first, int& second, int& third) {
	tuple<double, double, double> x = generateIndexes(index);
	tie(first, second, third) = x;
}

const Individual SDE::mutation(const int& first, const int& second, const int& third) {
	return population[first] - (population[second] - population[third]) * MUTATION_FACTOR;
}

vector<Individual> SDE::crossOver(const Individual& v) {
	int D = function.getDimensions();
	vector<Individual> u;
	vector<double> result(D);
	for (int i = 0; i < POP_SIZE; ++i) {
		int jRand = rng.getRandomInteger(0, D);
		for (int j = 0; j < D; ++j) {
			if (j == jRand || rng.getRandomDouble(0.0, 1.0) <= CROSS_OVER) {
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

	for (int i = 0; i < POP_SIZE; ++i) {
		
		if (function(u[i].get()) < function(population[i].get())) {
			population[i] = u[i];
			// cout << "Found better" << '\n';
		}
	}
}

SDE::SDE(const int &NP, const int &MAXFES, const double &F, const double &CR, Function& function) : POP_SIZE(NP), MAXFES(MAXFES), MUTATION_FACTOR(F), CROSS_OVER(CR), function(function) {}

void SDE::run() {
	int first, second, third;
	initializePopulation();
	for (int CFES = 0; CFES < MAXFES; CFES += POP_SIZE) {
		
		if (CFES % 1000 == 0) {
			cout << "Current iteration " << setw(10) << CFES << "/" << MAXFES << '\n';
		}
		for (int i = 0; i < POP_SIZE; ++i) {

			sample(i, first, second, third);

			Individual v = mutation(first, second, third);

			vector<Individual> u = crossOver(v);
			evaluate(u);
		}
	}

	// Compute best from this run
	int bestIndex = 0;
	double currentValue;
	double lowestValue = function(population[bestIndex].get()); // Best individual is the one with the lowest function value
	double highestValue = lowestValue; // Worst individual is the one with the highest function value
	double avgValue = lowestValue;

	for (int i = 1; i < POP_SIZE; ++i) {
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
}