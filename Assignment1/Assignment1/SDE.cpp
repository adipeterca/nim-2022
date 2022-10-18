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
			cout << "Found better" << '\n';
		}
	}
}

SDE::SDE(const int &NP, const int &MAXFES, const double &F, const double &CR, Function& function) : POP_SIZE(NP), MAXFES(MAXFES), MUTATION_FACTOR(F), CROSS_OVER(CR), function(function) {}

void SDE::run() {
	int first, second, third;
	initializePopulation();
	for (int CFES = 0; CFES < MAXFES; CFES += POP_SIZE) {
		for (int i = 0; i < POP_SIZE; ++i) {

			sample(i, first, second, third);

			Individual v = mutation(first, second, third);

			vector<Individual> u = crossOver(v);
			evaluate(u);
		}
		cout << "==================================" << '\n';
		cout << "Current iteration " << CFES << '\n';
		cout << "==================================" << '\n';
	}

	// Compute best from this run
	int bestIndex = 0;
	for (int i = 1; i < POP_SIZE; ++i) {
		if (function(population[bestIndex].get()) < function(population[i].get())) {
			bestIndex = i;
		}
	}
	cout << "SDE Results for function " << function.getName() << " :\n";
	cout << "Dimensions: " << function.getDimensions() << "\n";
	cout << "Interval: [" << function.getMin() << ", " << function.getMax() << "]\n";
	cout << "x = " << population[bestIndex] << "\n";
	cout << "Value for f(x) = " << function(population[bestIndex].get()) << "\n";
}