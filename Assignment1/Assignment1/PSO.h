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

//class AbstractPSO {
//protected:
//	struct Particle {
//		vector<double> currentPosition;
//		vector<double> bestPosition;
//		vector<double> velocity;
//	};
//
//	const int populationSize;
//
//	// Parameter which influences how much the particle's best position influences the particle's next velocity
//	// Typical values are between [1, 3]
//	double cognitiveCoef = 2.0;
//
//	// Parameter which influences how much the swarn's best position influences the particle's next velocity
//	// Typical values are between [1, 3]
//	double socialCoef = 2.0;
//
//	// Parameter which influences how much of the particle's current velocity influences the next velocity
//	const double inertiaWeight = 0.1;
//
//	// The benchmark function. The algorithm will try and get to the global minimum.
//	Function function;
//
//	// The stopping criterion
//	size_t funcEvalMax;
//public:
//	/// <summary>
//	/// Runs the algorithm and updates a local variable called "populationBest".
//	/// </summary>
//	/// <param name="verbose">if true, will print various information to the screen. Defaults to true.</param>
//	virtual void run(bool verbose = true) = 0;
//};


class PSO {
private:
	struct Particle {
		vector<double> currentPosition;
		vector<double> bestPosition;
		vector<double> velocity;
	};

	struct Swarn {
		vector<Particle> particles;
		vector<double> bestPosition;
	};

	vector<Swarn> swarns;

	vector<double> bestGlobal;

	// Parameter which influences how much the particle's best position influences the particle's next velocity
	// Typical values are between [1, 3]
	double cognitiveCoef = 1.49445;

	// Parameter which influences how much the swarn's best position influences the particle's next velocity
	// Typical values are between [1, 3]
	double socialCoef = 1.49445;

	// Parameter which influences how much the global best position influences the particle's next velocity
	double globalCoef = 0.3645;

	// Parameter which influences how much of the particle's current velocity influences the next velocity
	double inertiaWeight = 0.729;

	// The benchmark function. The algorithm will try and get to the global minimum.
	Function function;

	// The stopping criterion
	const size_t funcEvalMax;

	const size_t numberOfSwarns;

	const size_t swarnSize;

	bool alreadyRun = false;

public:
	PSO(Function& function, size_t numberOfSwarns = 1, size_t swarnSize = 100, size_t funcEvalMax = 1e7) : 
		function(function), 
		numberOfSwarns(numberOfSwarns),
		swarnSize(swarnSize),
		funcEvalMax(funcEvalMax) {}

	void setCognitiveCoefficient(double value) {
		cognitiveCoef = value;
	}

	void setSocialCoefficient(double value) {
		socialCoef = value;
	}

	void setGlobalCoefficient(double value) {
		globalCoef = value;
	}

	void setInertiaWeight(double value) {
		inertiaWeight = value;
	}

	vector<double>& run(bool verbose = true) {
		// Clear any past values
		// DO NOT count on this measure! Run each PSO instance only ONCE!
		// This will only clear the -swarns- variable, but each individual swarn
		// may still hold values (which will clog up the RAM space) - needs testing
		swarns.clear();
		bestGlobal.clear();
		

		if (alreadyRun) {
			cout << RED_START <<"WARNING! Running a PSO instance more than once!\n" << COLOR_END;
		}
		alreadyRun = true;

		if (verbose) {
			cout << "--------------- Testing for function ";
			cout << BLUE_START << function.getName() << COLOR_END;
			cout << " ---------------\n";
		}

		// Ignore the global coeficient if only 1 swarn (standard PSO) is implemented
		if (numberOfSwarns == 1) {
			globalCoef = 0.0;
		}

		// Create the Swarn objects
		for (size_t i = 0; i < numberOfSwarns; i++) {
			swarns.push_back(Swarn());
			
			// Create the Particle objects
			for (size_t j = 0; j < swarnSize; j++) {
				swarns[i].particles.push_back(Particle());
			}
		}

		// Assign default values
		for (auto& swarn : swarns) {
			for (auto& particle : swarn.particles) {

				// Velocity constrains
				double margin = abs(function.getMax() - function.getMin());
				
				// Randomly initialize the starting position & velocity
				for (size_t i = 0; i < function.getDimensions(); i++) {
					particle.currentPosition.push_back(getRandomDouble(function.getMin(), function.getMax()));
					particle.velocity.push_back(getRandomDouble(-margin, margin));
				}
				particle.bestPosition = particle.currentPosition;

				// Update swarn's best position
				// NOTE: it is better if THE VALUE IS LOWER
				double currentValue = function(particle.currentPosition);
				if (swarn.bestPosition.size() == 0 || currentValue < function(swarn.bestPosition)) {
					swarn.bestPosition = particle.currentPosition;

					// Update global best position
					if (bestGlobal.size() == 0 || currentValue < function(bestGlobal)) {
						bestGlobal = particle.currentPosition;
					}
				}
			}
		}

		if (verbose) {
			cout << "Initial global best value : " << YELLOW_START << function(bestGlobal) << COLOR_END << "\n";
		}

		// Variable used for verbose output
		int currentProc = 0;
		for (size_t fe = 0; fe < funcEvalMax; fe += numberOfSwarns * swarnSize) {

			if (verbose) {
				if (fe >= funcEvalMax * 90 / 100 && currentProc < 90) {
					cout << YELLOW_START << "90% complete\n" << COLOR_END;
					currentProc = 90;
				}
				else if (fe >= funcEvalMax * 80 / 100 && currentProc < 80) {
					cout << YELLOW_START << "80% complete\n" << COLOR_END;
					currentProc = 80;
				}
				else if (fe >= funcEvalMax * 70 / 100 && currentProc < 70) {
					cout << YELLOW_START << "70% complete\n" << COLOR_END;
					currentProc = 70;
				}
				else if (fe >= funcEvalMax * 50 / 100 && currentProc < 50) {
					cout << YELLOW_START << "50% complete\n" << COLOR_END;
					currentProc = 50;
				}
				else if (fe >= funcEvalMax * 40 / 100 && currentProc < 40) {
					cout << YELLOW_START << "40% complete\n" << COLOR_END;
					currentProc = 40;
				}
				else if (fe >= funcEvalMax * 30 / 100 && currentProc < 30) {
					cout << YELLOW_START << "30% complete\n" << COLOR_END;
					currentProc = 30;
				}
				else if (fe >= funcEvalMax * 15 / 100 && currentProc < 15) {
					cout << YELLOW_START << "15% complete\n" << COLOR_END;
					currentProc = 15;
				}
				else if (fe >= funcEvalMax * 5 / 100 && currentProc < 5) {
					cout << YELLOW_START << "5% complete\n" << COLOR_END;
					currentProc = 5;
				}
				
			}

			for (auto& swarn : swarns) {
				for (auto& p : swarn.particles) {
					// Velocity update
					for (size_t i = 0; i < function.getDimensions(); i++) {
						double randomCognitive = getRandomDouble(0.0, 1.0);
						double randomSocial = getRandomDouble(0.0, 1.0);
						double randomGlobal = getRandomDouble(0.0, 1.0);

						p.velocity[i] = inertiaWeight * p.velocity[i] +
							cognitiveCoef * randomCognitive * (p.bestPosition[i] - p.currentPosition[i]) +
							socialCoef * randomSocial * (swarn.bestPosition[i] - p.currentPosition[i]) +
							globalCoef * randomGlobal * (bestGlobal[i] - p.currentPosition[i]);
					}

					for (size_t i = 0; i < function.getDimensions(); i++) {
						p.currentPosition[i] += p.velocity[i];
					}

					// Particle best position update
					double currentValue = function(p.currentPosition);
					if (currentValue < function(p.bestPosition)) {
						p.bestPosition = p.currentPosition;

						// Swarn best position update
						if (currentValue < function(swarn.bestPosition)) {
							swarn.bestPosition = p.currentPosition;

							// Global best position update
							if (currentValue < function(bestGlobal)) {
								bestGlobal = p.currentPosition;
								if (countCorrectDigits(currentValue) == 10) {
									if (verbose) {
										cout << "Found ";
										cout << RED_START << " perfect value " << COLOR_END;
										cout << "(10 / 10 correct digits) at FE " << setw(10) << fe << " / " << funcEvalMax << "\n";
									}
									return bestGlobal;
								}
							}
						}
					}
				}
			}
		}

		if (verbose) {
			double result = function(bestGlobal);
			cout << "Final best : " << result << " with ";
			cout << BLUE_START << countCorrectDigits(result) << COLOR_END << " / 10 correct digits.";
			cout << "\n\n\n";
		}

		return bestGlobal;
	}
};

#endif