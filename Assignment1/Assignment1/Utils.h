#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

#define BLUE_START "\x1B[36m"
#define RED_START "\x1B[31m"
#define YELLOW_START "\x1B[33m"
#define COLOR_END "\033[0m"

using namespace std;

// Default value for a constant very close to zero.
constexpr auto EPSILON = 1e-16;

/// <summary>
/// Generate a random double number using a uniform distribution inside an interval.
/// </summary>
/// <param name="min">interval lowest value (inclusive)</param>
/// <param name="max">interval highest value (inclusive)</param>
/// <returns>random double number</returns>
double getRandomDouble(const double min, const double max);

/// <summary>
/// Generate a random unsigned number using a uniform distribution inside an interval.
/// </summary>
/// <param name="min">interval lowest value (inclusive)</param>
/// <param name="max">interval highest value (inclusive)</param>
/// <returns>random size_t number</returns>
size_t getRandomUnsigned(const size_t min, const size_t max);

/// <summary>
/// Function used to count the correct digits. The answer is always 1.000000000.
/// </summary>
/// <param name="value">value whose digits are counted</param>
/// <returns>number of correct digits</returns>
int countCorrectDigits(double value);

#endif