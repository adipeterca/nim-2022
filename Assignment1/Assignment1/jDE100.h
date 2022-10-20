#pragma once
#include <vector>
#include <tuple>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include "Individual.h"
#include "Function.h"
#include "Random.h"

class jDE100 {
private:
	unsigned bNP, sNP, ageLimit, maxFE, myEqs;
	double Fl, Fu, Finit, CRl, CRu, CRinit, s1, s2, epsilon;

	Random random;

	vector<Individual> Pb, Ps;
	vector<double> *Fs, *Fb, *CRs, *CRb;
	Function& function;

	void initializePopulation(vector<Individual> population, unsigned size);

	const tuple<unsigned, unsigned, unsigned> generateIndexes(const unsigned& index, unsigned popSize, unsigned offset);

	void sample(const unsigned& index, unsigned& first, unsigned& second, unsigned& third, unsigned popSize, unsigned offset);

	// jde mutation
	// randj = rand(0,1)
	// F = if rand2 <  s1 Fl + rand1 * Fu else Fi
	// mutation process is the same from sde with the possibility for r2 or r3 to be one individual from Ps(same for both)
	const Individual mutation(const unsigned& first, const unsigned& second, const unsigned& third, double F, vector<Individual>& population);

	// jde cross-over
	// CR = if rand4 < s2 rand3 else previos CRi
	// cr process is the same from sde
	vector<Individual> crossOver(const Individual& v, double CR, vector<Individual>& population);

	// selection process is the same from sde
	// if element is better update vector values for CR and F with F and CR otherwise keep old values
	void evaluate(vector<Individual>& u, vector<Individual>& population, double F, double CR, vector<double>& Fi, vector<double>& CRi);

	void jDEOperations(vector<Individual>& population, vector<double>& Fi, vector<double>& CRi, unsigned offset);

	Individual getBestIndividual(vector<Individual>& population);

	unsigned getWorstIndividualPosition(vector<Individual>& population);

	double maxSimilarityPercentage(vector<Individual>& population);

public:
	jDE100(Function& function, const unsigned bNP, const unsigned sNP, const unsigned ageLimit, const unsigned maxFE, const unsigned myEqs, const double Fl, const double Fu, const double Finit, const double CRl, const double CRu, const double CRinit, const double s1, const double s2, const double epsilon);

	Individual run();
};