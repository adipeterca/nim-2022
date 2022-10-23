#ifndef JDE100_H
#define JDE100_H
#include <vector>
#include <tuple>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <fstream>
#include "Individual.h"
#include "Function.h"

class jDE100 {
private:
	unsigned bNP, sNP, ageLimit, maxFE, myEqs;
	double Fl, Fu, Finit, CRl, CRu, CRinit, s1, s2, epsilon;

	vector<Individual> Pb, Ps;
	vector<double>* Fs, * Fb, * CRs, * CRb;
	Function& function;

	/// <summary>
	/// Function used to (re)initialize given population of given size randomly.
	/// </summary>
	/// <param name="population"> vector of Individuals to randomly initialize</param>
	/// <param name="size">population size</param>
	void initializePopulation(vector<Individual>& population, const unsigned& size);

	/// <summary>
	/// Generates three random distinct positions from population vector with first != second != third != index. 
	/// If offset is 1 increases range by 1 and if value is equal to popSize it represents a radnom Xs from Ps.(when sampling from Pb one Xs from ps should also be included)
	/// </summary>
	/// <param name="index">index</param>
	/// <param name="popSize">population size</param>
	/// <param name="offset">flag for when a member from Ps is to be included in the sampling of Pb</param>
	/// <returns>tuple containing positions</returns>
	const tuple<unsigned, unsigned, unsigned> generateIndexes(const unsigned& index, const unsigned& popSize, const unsigned& offset);

	/// <summary>
	/// Passes as reference three random distinct positions from population vector with first != second != third != index population size and offset flag
	/// </summary>
	/// <param name="index">index</param>
	/// <param name="first">first position</param>
	/// <param name="second">second position</param>
	/// <param name="third">third position</param>
	/// <param name="popSize">population size</param>
	/// <param name="offset">flag for when a member from Ps is to be included in the sampling of Pb</param>
	void sample(const unsigned& index, unsigned& first, unsigned& second, unsigned& third, const unsigned& popSize, const unsigned& offset);

	/// <summary>
	/// jDE mutation 
	/// notation used forward randj = rand[0,1]
	/// F parameter update rule: F = if rand2 <  s1 Fl + rand1 * Fu else Fi
	/// Mutation process is essentially the same from sde with the possibility for r2 or r3 to be one individual from Ps
	/// </summary>
	/// <param name="first">first individual</param>
	/// <param name="second">second individual</param>
	/// <param name="third">third individual</param>
	/// <param name="F">parameter that controls mutation impact</param>
	/// <param name="population">population of individuals to apply mutation on</param>
	/// <returns>new individual</returns>
	const Individual mutation(const unsigned& first, const unsigned& second, const unsigned& third, const double& F, const vector<Individual>& population);

	/// <summary>
	/// jDE cross-over
	/// CR paramter update rule: CR = if rand4 < s2 rand3 else CRi
	/// CR process is esentially the same from sde.
	/// </summary>
	/// <param name="v">donor vector</param>
	/// <param name="CR">parameter that controls how often cross over happens</param>
	/// <param name="population">population of individuals to apply cross-over on</param>
	/// <returns>vector of individuals</returns>
	vector<Individual> crossOver(Individual& const v, const double& CR, vector<Individual>& const population);

	/// <summary>
	/// jDE selection
	/// if an element from u is better than corespondent from population update vector values for CRi and Fi with F and CR otherwise keep old values
	/// selection is esentially the same from sde.
	/// </summary>
	/// <param name="u">vector of individuals</param>
	/// <param name="population">population of individuals to apply selection on</param>
	/// <param name="F">parameter that controls mutation impact</param>
	/// <param name="CR">parameter that controls how often cross over happens</param>
	/// <param name="Fi">vector containing F parameter value for each individual from population</param>
	/// <param name="CRi">vector containing CR parameter value for each individual from population</param>
	void evaluate(vector<Individual>& u, vector<Individual>& population, const double& F, const double& CR, vector<double>& Fi, vector<double>& CRi);

	/// <summary>
	/// Applies mutation, cross-over, selection and computes F and CR values.
	/// </summary>
	/// <param name="population">population of individuals to apply selection on</param>
	/// <param name="Fi">vector containing F parameter value for each individual from population</param>
	/// <param name="CRi">vector containing CR parameter value for each individual from population</param>
	/// <param name="offset">flag for when a member from Ps is to be included in the sampling of Pb</param>
	void jDEOperations(vector<Individual>& population, vector<double>& Fi, vector<double>& CRi, unsigned offset);

	/// <summary>
	/// Return best individual from given population. (smallest function activation value).
	/// </summary>
	/// <param name="population">population of individuals to search</param>
	/// <returns>best individual</returns>
	Individual getBestIndividual(vector<Individual>& population);

	/// <summary>
	/// Return index of worst individual from given population. (largest function activation value).
	/// </summary>
	/// <param name="population">population of individuals to search</param>
	/// <returns>worst individual index</returns>
	unsigned getWorstIndividualPosition(vector<Individual>& population);

	/// <summary>
	/// Compute largest percentage of identical individuals.(2 individuals are considered identical ix abs(x - y) < epsilon)
	/// </summary>
	/// <param name="population">population of individuals</param>
	/// <returns>largest percentage of identical individuals</returns>
	double maxSimilarityPercentage(vector<Individual>& population);

public:
	/// <summary>
	/// jDE100 Constructor
	/// bNP = m * sNP
	/// </summary>
	/// <param name="function">function used for evaluation</param>
	/// <param name="bNP">large population size</param>
	/// <param name="sNP">small population size</param>
	/// <param name="ageLimit">age limit for best individual, reinitialize population if age == ageLimit</param>
	/// <param name="maxFE">maximum number of function evaluations, stop algorithm after reaching threshold</param>
	/// <param name="myEqs">maximum percentage of identical individuals, reinitialize if treshold is passed</param>
	/// <param name="Fl">lower bound for F parameter</param>
	/// <param name="Fu">upper bound for F parameter</param>
	/// <param name="Finit">initial value for F parameter</param>
	/// <param name="CRl">lower bound for CR parameter</param>
	/// <param name="CRu">upper bound for CR parameter</param>
	/// <param name="CRinit">initial value for CR parameter</param>
	/// <param name="s1">paramter controlling F value updates</param>
	/// <param name="s2">paramter controlling CR value updates</param>
	/// <param name="epsilon">treshold for precision</param>
	jDE100(Function& function, 
			const unsigned bNP = 1000,
			const unsigned sNP = 25,
			const unsigned ageLimit = 1e9,
			const unsigned maxFE = 1e3,
			const unsigned myEqs = 25,
			const double Fl = 0.1,
			const double Fu = 0.9,
			const double Finit = 0.5,
			const double CRl = 0.0,
			const double CRu = 1.1,
			const double CRinit = 0.9,
			const double s1 = 0.1,
			const double s2 = 0.1,
			const double epsilon = 1e-14);

	/// <summary>
	/// Runs jDE100 using specified parameters
	/// </summary>
	Individual run();
};
#endif