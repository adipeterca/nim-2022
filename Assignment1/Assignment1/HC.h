#pragma once
#include <vector>
#include "Individual.h"
#include "Function.h"

using namespace std;

class HC {
private:
	Individual individual;
	Function& function;
	double epsilon;
	unsigned MAX_ITER;

	/// <summary>
	/// Function that applies the best improvement variant of hill climbing.
	/// </summary>
	/// <param name="individual">individual to improve</param>
	/// <returns>improved individual or original if nothing better was found</returns>
	Individual bestImprovement(Individual &individual);
public:

	/// <summary>
	/// HC constructor.
	/// </summary>
	/// <param name="individual">individual to improve</param>
	/// <param name="function">function used for evaluation</param>
	/// <param name="epsilon">constant used to find neighbours</param>
	/// <param name="MAX_ITER">maximum number of iterations to run improvement strategy</param>
	HC(Individual& individual, Function& function, const double& epsilon, const unsigned& MAX_ITER);

	/// <summary>
	/// Function that improves given individual using a strategy.
	/// </summary>
	/// <returns>f(individual) if no improvement is found f(best) otherwise</returns>
	double improve();
};