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
	void run(bool verbose = true) {
		
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

		if (verbose) {
			cout << "Final best : " << BLUE_START << function(populationBest) << COLOR_END << "\n\n\n";
		}
	}
};

#endif