#include "Individual.h"

Individual::Individual(vector<double>& const x) {
	this->x = x;
}

Individual::Individual(const Individual& otherIndividual) {
	this->x = otherIndividual.x;
	this->value = otherIndividual.value;
	this->function = otherIndividual.function;
}

Individual::Individual(Function &function) {
	x.clear();
	for (unsigned i = 0; i < function.getDimensions(); ++i) {
		x.push_back(getRandomDouble(function.getMin(), function.getMax()));
	}
	value = function(x.data());
	this->function = &function;
}

void Individual::resize(Function& function) {
	x.resize(function.getDimensions());
	this->function = &function;
}

const double* Individual::get() const {
	return x.data();
}

vector<double> Individual::getVector() {
	return x;
}

double& Individual::operator[](const unsigned&index) {
	return x[index];
}

bool Individual::operator==(const Individual& ind) const {
	// NOTE: needs to be tested
	// LE: tested, works!
	return ind.x == x;
}

const Individual Individual::operator-(const Individual &other) const {

	if (other.x.size() != x.size()) {
		throw exception("Individuals don't have the same size for element-wise difference.");
	}

	vector<double> diff;
	for (unsigned i = 0; i < x.size(); ++i) {
		diff.push_back(x[i] - other.x[i]);
	}
	return Individual(diff);
}

const Individual Individual::operator+(const Individual &other) const {

	if (other.x.size() != x.size()) {
		throw exception("Individuals don't have the same size for element-wise sum.");
	}

	vector<double> sum;
	for (unsigned i = 0; i < x.size(); ++i) {
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
	for (unsigned i = 0; i < individual.x.size(); ++i) {
		if (i + 1 != individual.x.size())
			os << individual.x[i] << ", ";
		else
			os << individual.x[i];
	}
	os << "]";
	return os;
}