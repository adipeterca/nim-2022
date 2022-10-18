#include "Function.h"

Function::Function(const char* name, const int &id, const int &D, const double &min, const double &max) : name(name), id(id), dimensions(D), min(min), max(max) {
	this->name = name;
}

const double Function::operator ()(const double* x) {
	double* returnValue = new double;
	cec19_test_func((double* )x, returnValue, dimensions, 1, id);
	return *returnValue;
}

const int Function::getDimensions() {
	return dimensions;
}

const double Function::getMin() {
	return min;
}

const double Function::getMax() {
	return max;
}

string& Function::getName() {
	return name;
}