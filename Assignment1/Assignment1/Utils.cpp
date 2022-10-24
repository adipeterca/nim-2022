#include "Utils.h"

mt19937 generator(chrono::high_resolution_clock::now().time_since_epoch().count());

double getRandomDouble(const double min, const double max) {
	uniform_real_distribution<> distribution(min, max);
	return distribution(generator);
}

size_t getRandomUnsigned(const size_t min, const size_t max) {
	uniform_int_distribution<> distribution(min, max);
	return distribution(generator);
}

int countCorrectDigits(double value) {

	ostringstream os;
	os << fixed << setprecision(9) << value;
	string strValue = os.str();

	// If the first is not 1, all values are wrong.
	if (strValue[0] != '1') return 0;

	int result = 1;
	for (int i = 2; i < strValue.size(); ++i) {
		if (strValue[i] == '0')
			result++;
		else
			break;
	}
	return result;

	/* Tested on:
	* 2.000000000 -> returns 0
	* 1.924235666 -> returns 1
	* 1.003243567 -> returns 3
	* 1.000000000 -> returns 10
	*/
}