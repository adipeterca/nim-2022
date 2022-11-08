#ifndef PSO_H
#define PSO_H

#include <vector>
#include "Utils.h"

using namespace std;

// To be refactored later
// in an abstract class from which each function will derive.
// Function class -> Rastrigin class
class Function {
public:
	size_t getDimensions() {}
	double getMin() {}
	double getMax() {}
	double operator()(vector<double>& x) {}
	string getName();
};


class PSO {
private:
	struct Particle {
		vector<double> currentPosition;
		vector<double> bestPosition;
		vector<double> velocity;
	};


	const int populationSize = 100;
	
	vector<Particle> particles;

	// The swarns best position
	vector<double> populationBest;

	// PSO parameters with default values
	double cognitiveCoef = 1.5;
	double socialCoef = 1.5;
	double inertiaWeight = 0.5;

	Function function;

	// The stopping criterion
	const int funcEvalMax = 1e7;
public:
	struct A {};

	void run() {
		
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
			if (function(p.currentPosition) < function(populationBest)) {
				populationBest = p.currentPosition;
			}
		}

		for (size_t funcEval = 0; funcEval < funcEvalMax; funcEval++) {
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
				if (function(p.currentPosition) > function(p.bestPosition)) {
					p.bestPosition = p.currentPosition;

					// Swarn best position update
					if (function(p.currentPosition) > function(populationBest)) {
						populationBest = p.currentPosition;
					}
				}
			}
		}
	}
};

#endif