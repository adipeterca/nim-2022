#ifndef PSO_H
#define PSO_H

#include <vector>
#include "Utils.h"
#include "Function.h"

using namespace std;

// To be refactored later
// in an abstract class from which each function will derive.
// Function class -> Rastrigin class
//class Function {
//public:
//	size_t getDimensions() {}
//	double getMin() {}
//	double getMax() {}
//	double operator()(vector<double>& x) {}
//	string getName();
//};


class PSO {
private:
	struct Particle {
		vector<double> currentPosition;
		vector<double> bestPosition;
		vector<double> velocity;
	};


	const int populationSize;
	
	vector<Particle> particles;

	// The swarns best position
	vector<double> populationBest;

	// PSO parameters with default values
	double cognitiveCoef = 2.0;
	double socialCoef = 2.0;
	const double inertiaWeight = 0.1;

	// The benchmark function. The algorithm will try and get to the global minimum.
	Function function;

	// The stopping criterion
	size_t funcEvalMax;
public:
	PSO(Function& function, size_t populationSize = 100, size_t funcEvalMax = 1e7) : function(function), populationSize(populationSize), funcEvalMax(funcEvalMax) {
		for (size_t i = 0; i < populationSize; i++) {
			particles.push_back(Particle());
		}
	}

	/// <summary>
	/// Runs the algorithm and updates a local variable called "populationBest".
	/// </summary>
	/// <param name="verbose">if true, will print various information to the screen. Defaults to true.</param>
	/// <param name="useHillclimber">if true, will use a HCFI on bitstrings to improve the result. Defaults to true.</param>
	void run(bool verbose = true, bool useHillclimber = true) {
		
		if (verbose) {
			cout << "--------------- Testing for function " << BLUE_START << function.getName() << COLOR_END << " ---------------\n";
			cout << "Initializing population...\n";
		}

		double margin = abs(function.getMax() - function.getMin());
		for (auto& p : particles) {
			// Randomly initialize the starting position & velocity
			for (size_t i = 0; i < function.getDimensions(); i++) {
				p.currentPosition.push_back(getRandomDouble(function.getMin(), function.getMax()));	
				p.velocity.push_back(getRandomDouble(-margin, margin));
			}
			p.bestPosition = p.currentPosition;
			
			// Update swarn's best position
			// NOTE: it is better if THE VALUE IS LOWER
			if (populationBest.size() == 0 || function(p.currentPosition) < function(populationBest)) {
				populationBest = p.currentPosition;
			}
		}

		if (verbose) {
			cout << "Initial population best value : " << YELLOW_START << function(populationBest) << COLOR_END << "\n";
		}

		for (size_t funcEval = 0; funcEval < funcEvalMax; funcEval += populationSize) {

			// Was the overall best position updated?
			bool updatedBest = false;
			for (auto& p : particles) {
				// Velocity update
				for (size_t i = 0; i < function.getDimensions(); i++) {
					double randomCognitive = getRandomDouble(0.0, 1.0);
					double randomSocial = getRandomDouble(0.0, 1.0);

					p.velocity[i] = inertiaWeight * p.velocity[i] + 
									cognitiveCoef * randomCognitive * (p.bestPosition[i] - p.currentPosition[i]) + 
									socialCoef * randomSocial * (populationBest[i] - p.currentPosition[i]);
				}
				
				for (size_t i = 0; i < function.getDimensions(); i++) {
					p.currentPosition[i] += p.velocity[i];
				}

				// Particle best position update
				if (function(p.currentPosition) < function(p.bestPosition)) {
					p.bestPosition = p.currentPosition;

					// Swarn best position update
					if (function(p.currentPosition) < function(populationBest)) {
						populationBest = p.currentPosition;
						updatedBest = true;
					}
				}
			}

			/*if (verbose && funcEval % 100000 == 0) {
				cout << "Current evaluations : " << funcEval << "\n";
				cout << "Best so far : " << YELLOW_START << function(populationBest) << COLOR_END << "\n";
			}*/
		}

		if (useHillclimber) {
			// The following variables are taken from https://profs.info.uaic.ro/~eugennc/teaching/ga/
			long long int a = long long int(function.getMin());
			long long int b = long long int(function.getMax());
			long long int d = 10;
			long long int N = (b - a) * long long int(pow(10, d));
			// The requiered number of bits for a single real number
			long long int n = long long int(ceil(log2(N)));

			// The current best solution is hold in populationBest
			// So, convert it into a bitstring
			vector<bool> bitstringBest;
			vector<bool> currentBitstring;
			currentBitstring.reserve(n);
			
			for (auto value : populationBest) {
				long long int valueToConvert = (value - a) * (pow(2, n) - 1) / (b - a);
				currentBitstring.clear();
				int i = n - 1;
				while (valueToConvert) {
						
				}

				bitstringBest.insert(bitstringBest.end(), currentBitstring.begin(), currentBitstring.end());
			}
			

			bool local = false;
			vector<double> neighbor;
			while (!local) {
				// Get the first improved neighbor

				// If none were found, that means we are in a local minimum
			}
		}

		if (verbose) {
			double result = function(populationBest);
			cout << "Final best : " << result << " with ";
			cout << BLUE_START << countCorrectDigits(result) << COLOR_END << " / 10 correct digits.";
			cout << "\n\n\n";
		}
	}
};

#endif