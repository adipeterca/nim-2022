#include "Individual.h"

Individual::Individual(vector<double>& x) {
	this->x = x;
}

Individual::Individual(Function &function, Random &rng) {
	for (int i = 0; i < function.getDimensions(); ++i) {
		x.push_back(rng.getRandomDouble(function.getMin(), function.getMax()));
	}
}

double* Individual::get() {
	return x.data(); // doesnt work for bool
}

const double Individual::operator[](const int &index) const {
	return x[index];
}

const Individual Individual::operator-(const Individual &other) const {

	if (other.x.size() != x.size()) {
		throw exception("Individuals don't have the same size for element-wise difference.");
	}

	vector<double> diff;
	for (int i = 0; i < x.size(); ++i) {
		diff.push_back(x[i] - other.x[i]);
	}
	return Individual(diff);
}

const Individual Individual::operator+(const Individual &other) const {

	if (other.x.size() != x.size()) {
		throw exception("Individuals don't have the same size for element-wise sum.");
	}

	vector<double> sum;
	for (int i = 0; i < x.size(); ++i) {
		sum.push_back(x[i] + other.x[i]);
	}
	return Individual(sum);
}

const Individual Individual::operator*(const double &factor) const {
	vector<double> result;
	for (int i = 0; i < x.size(); ++i) {
		result.push_back(x[i] * factor);
	}
	return Individual(result);
}

ostream& operator<<(ostream& os, Individual const& individual) {
	os << "[";
	for (int i = 0; i < individual.x.size(); ++i) {
		if (i + 1 != individual.x.size())
			os << individual.x[i] << ", ";
		else
			os << individual.x[i];
	}
	os << "]";
	return os;
}