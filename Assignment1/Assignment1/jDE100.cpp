#pragma once
#include "jDE100.h"

void jDE100::initializePopulation(vector<Individual>& population, const unsigned& size) {
	population.clear();
	for (unsigned i = 0; i < size; ++i) {
		population.push_back(Individual(function));
	}
}

const tuple<unsigned, unsigned, unsigned> jDE100::generateIndexes(const unsigned& index, const unsigned& popSize, const unsigned& offset) {
	unsigned first, second, third;
	do {
		first = getRandomUnsigned(0, popSize - 1);
	} while (first == index);
	do {
		second = getRandomUnsigned(0, popSize - 1 + offset); // aici crapa pentru ca iese din interval (de aici porneste eroarea)
	} while (second == index || second == first);
	do {
		third = getRandomUnsigned(0, popSize - 1 + offset);
	} while (third == index || third == first || third == second);
	return make_tuple(first, second, third);
}

void jDE100::sample(const unsigned& index, unsigned& first, unsigned& second, unsigned& third, const unsigned& popSize, const unsigned& offset) {
	tuple<unsigned, unsigned, unsigned> x = generateIndexes(index, popSize, offset);
	tie(first, second, third) = x;
}

const Individual jDE100::mutation(const unsigned& first, const unsigned& second, const unsigned& third, const double& F, const vector<Individual>& population) {
	// LE 23.10 ora 13:15
	// Am inteles ce vrei sa faci aici, dar ar trebui sa fie at the very least >=, nu ==. Daca seconds iti ia de exemplu valoarea 102, el nu intra aici.
	// Also, getRandomUnsigned ar trebui sa fie intre [0, sNP-1]
	if (second == population.size())
		return population[first] - (Ps[getRandomUnsigned(0, sNP)] - population[third]) * F;
	if (third == population.size())
		return population[first] - (population[second] - Ps[getRandomUnsigned(0, sNP)]) * F;
	return population[first] - (population[second] - population[third]) * F;
}

vector<Individual> jDE100::crossOver(Individual& const v, const double& CR, vector<Individual>& const population) {
	// 1000 individizi
	// vector V
	// CR iteratiei curente (i)
	/*unsigned D = function.getDimensions();
	vector<Individual> u;
	vector<double> result(D);
	for (unsigned i = 0; i < population.size(); ++i) {
		unsigned jRand = getRandomUnsigned(0, D);
		for (unsigned j = 0; j < D; ++j) {
			if (j == jRand || getRandomDouble(0.0, 1.0) <= CR) {
				
				result[j] = v[j];
			}
			else {
				result[j] = population[i][j];
			}
		}
		u.push_back(Individual(result));
	}
	return u;*/
	return population;
}

void jDE100::evaluate(vector<Individual>& u, vector<Individual>& population, const double& F, const double& CR, vector<double>& Fi, vector<double>& CRi) {
	for (unsigned i = 0; i < population.size(); ++i) {
		if (function(u[i].get()) < function(population[i].get())) {
			population[i] = u[i];
			Fi[i] = F;
			CRi[i] = CR;
		}
	}
}

void jDE100::jDEOperations(vector<Individual>& population, vector<double>& Fi, vector<double>& CRi, unsigned offset) {
	unsigned first, second, third;
	double F, CR;
	for (unsigned j = 0; j < population.size(); ++j) {
		// jde mutation
		if (getRandomDouble(0, 1) < s1) {
			F = Fl + getRandomDouble(0, 1) * Fu;
		}
		else {
			F = Fi[j];
		}
		offset = 0;
		sample(j, first, second, third, population.size(), offset);
		// cout << "Sampled " << first << " " << second << " " << third << "\n";
		Individual v = mutation(first, second, third, F, population);
		// jde crossover
		if (getRandomDouble(0, 1) < s2) {
			CR = getRandomDouble(0, 1);
		}
		else {
			CR = CRi[j];
		}
		vector<Individual> u = crossOver(v, CR, population);
		// jde selection
		evaluate(u, population, F, CR, Fi, CRi);
	}
}

Individual jDE100::getBestIndividual(vector<Individual>& population) {
	Individual best = population[0];
	double bestValue = function(best.get());
	for (unsigned i = 1; i < population.size(); ++i) {
		double iValue = function(population[i].get());
		if (iValue < bestValue) {
			best = population[i];
			bestValue = iValue;
		}
	}
	return best;
}

unsigned jDE100::getWorstIndividualPosition(vector<Individual>& population) {
	unsigned worstIndividualIndex = 0;
	double worstValue = function(population[worstIndividualIndex].get());
	for (unsigned i = 1; i < population.size(); ++i) {
		double iValue = function(population[i].get());
		if (iValue > worstValue) {
			worstIndividualIndex = i;
			worstValue = iValue;
		}
	}
	return worstIndividualIndex;
}

double jDE100::maxSimilarityPercentage(vector<Individual>& population) {
	unsigned n = population.size(), maxCount = 0;
	unordered_map<int, int> elements;
	vector<double> functionValues(n, 0);
	stringstream tmp;
	for (unsigned i = 0; i < n; ++i) {
		functionValues[i] = function(population[i].get());
	}
	for (unsigned i = 0; i < n; ++i) {
		double val = functionValues[i];
		tmp << setprecision(14) << fixed << val;
		double newVal = stod(tmp.str());
		elements[newVal]++;
	}
	for (auto& i : elements) {
		if (maxCount < i.second) {
			maxCount = i.second;
		}
	}
	return maxCount / n;
}


jDE100::jDE100(Function& function, 
				const unsigned bNP, 
				const unsigned sNP, 
				const unsigned ageLimit, 
				const unsigned maxFE, 
				const unsigned myEqs, 
				const double Fl, 
				const double Fu, 
				const double Finit, 
				const double CRl, 
				const double CRu, 
				const double CRinit, 
				const double s1, 
				const double s2, 
				const double epsilon) : function(function) {

	this->bNP = bNP;
	this->sNP = sNP;
	this->ageLimit = ageLimit;
	this->maxFE = maxFE;
	this->myEqs = myEqs;
	this->Fl = Fl;
	this->Fu = Fu;
	this->Finit = Finit;
	this->CRl = CRl;
	this->CRu = CRu;
	this->CRinit = CRinit;
	this->s1 = s1;
	this->s2 = s2;
	this->epsilon = epsilon;

	// initialize Fi = 0.5, CRi = 0.9 for every i in {1...bNP + sNP}
	this->Fs = new vector<double>(sNP, Finit);
	this->Fb = new vector<double>(bNP, Finit);
	this->CRs = new vector<double>(sNP, CRinit);
	this->CRb = new vector<double>(bNP, CRinit);
}

Individual jDE100::run() {
	// initialize Pb (big pop) bNP = m * sNP
	// initialize Ps (small pop)
	ofstream fout("jde_output.txt");

	auto t0 = chrono::high_resolution_clock::now();

	initializePopulation(Pb, bNP);
	initializePopulation(Ps, sNP);
	Individual bBest = getBestIndividual(Pb), sBest = getBestIndividual(Ps);
	Individual best = function(bBest.get()) < function(sBest.get()) ? bBest : sBest;

	unsigned m = bNP / sNP, age = 0, cFE = 0;
	unsigned currentIter = 0;
	// while solution not found or max iter not reached
	while (function(best.get()) > 1.000000000) { // cFE < maxFE, should getBestIndividual fe be considered?
		// check for Pb reinitialization -> do if myEqs% individuals have similar function value(diff is less than epsilon)
		//									or if best individual is not improved for ageLmt evaluations
		// random reinitialization
		
		
		cout << "Current main iteration: " << currentIter << endl;
		fout << "Current main iteration: " << currentIter << endl;
		currentIter++;
		fout.flush();

		if (age == ageLimit || maxSimilarityPercentage(Pb) >= myEqs) {
			initializePopulation(Pb, bNP);
			age = 0;
		}

		// check for Ps reinitialization -> do if myEqs% individuals have similar function value as the best from Ps
		// keep best, random reinitialization for others
		if (maxSimilarityPercentage(Pb) >= myEqs) {
			initializePopulation(Ps, sNP);
			Ps[0] = best;
		}
		// for each j in Pb
		jDEOperations(Pb, *Fb, *CRb, 1);
		cFE += bNP;
		// if best in Pb -> copy best to Ps
		bBest = getBestIndividual(Pb), sBest = getBestIndividual(Ps);
		if (function(bBest.get()) < function(sBest.get())) {
			unsigned index = getWorstIndividualPosition(Ps);
			Ps[index] = bBest;
		}
		// for k in 1...m
		for (unsigned k = 0; k < m; ++k) {
			// for each j in Ps
			jDEOperations(Ps, *Fs, *CRs, 0);
			cFE += sNP;
		}
		sBest = getBestIndividual(Ps);
		Individual currentBest = function(bBest.get()) < function(sBest.get()) ? bBest : sBest;
		age += 1;
		if (function(currentBest.get()) < function(best.get())) {
			best = currentBest;
			age = 0;
		}
	}

	auto t1 = chrono::high_resolution_clock::now();
	auto seconds = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() / 1000;

	cout << "Found best value of " << function(best.get()) << " for function " << function.getName() << " in " << seconds / 60 << ":" << seconds % 60 << "\n";
	fout << "Found best value of " << function(best.get()) << " for function " << function.getName() << " in " << seconds / 60 << ":" << seconds % 60 << "\n";
	fout.flush();
	return best;
}