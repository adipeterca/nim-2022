#include "Function.h"

Function::Function(const char* name, int id, int D, double min, double max) {
	this->name = name;
	this->id = id;
	this->dimensions = D;
	this->min = min;
	this->max = max;
}

double Function::operator ()(double* x) {
	double* returnValue = new double;
	cec19_test_func(x, returnValue, dimensions, 1, id);
	return *returnValue;
}

int Function::getDimensions() {
	return dimensions;
}

double Function::getMin() {
	return min;
}

double Function::getMax() {
	return max;
}

std::string& Function::getName() {
	return name;
}