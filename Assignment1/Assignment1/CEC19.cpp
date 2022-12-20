#include "CEC19.h"

void AbstractFunction::shiftAndRotate(vector<double>& point) {
	if (point.size() != dimensions) {
		throw invalid_argument("Point size and function's dimensions don't match!");
	}

	// Shift value
	for (size_t i = 0; i < point.size(); ++i) {
		point[i] -= OShift[i];
	}

	// Shrink to the original size
	for (size_t i = 0; i < point.size(); ++i) {
		point[i] *= shrinkRate;
	}

	// Rotate value
	vector<double> rotatedPoint;
	rotatedPoint.resize(point.size());
	for (size_t i = 0; i < point.size(); ++i) {
		rotatedPoint[i] = 0;
		for (size_t j = 0; j < point.size(); ++j) {
			rotatedPoint[i] += point[j] * M[i * point.size() + j];
		}
	}

	point = rotatedPoint;
}

AbstractFunction::AbstractFunction(size_t functionNumber, size_t dimensions, double shrinkRate) {
	if (functionNumber > 10) {
		throw invalid_argument("Function number must be lower that 11.");
	}

	this->dimensions = dimensions;
	this->shrinkRate = shrinkRate;

	// If requiered, initialize M and OShift
	if (functionNumber > 3) {
		M = new double[dimensions * dimensions];
		OShift = new double[dimensions];

		// Read shift data
		ifstream fShift("input_data/shift_data_" + to_string(functionNumber) + ".txt");
		for (size_t i = 0; i < dimensions; ++i) {
			fShift >> OShift[i];
		}

		// Read matrix data
		ifstream fMatrix("input_data/M_" + to_string(functionNumber) + "_D" + to_string(dimensions) + ".txt");
		for (size_t i = 0; i < dimensions * dimensions; ++i) {
			fMatrix >> M[i];
		}
	}
}

AbstractFunction::~AbstractFunction() {
	delete[] M;
	delete[] OShift;
}


Chebyshev::Chebyshev(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double Chebyshev::operator() (vector<double> point) {
	if (point.size() <= 2) {
		throw invalid_argument("Chebyshev function is only defined for Dsize > 2");
	}

	int sample;
	double result = 0.0;
	double a = 1.0;
	double b = 1.2;
	double y = -1.0;
	double px;
	long double dx, dy;

	for (int i = 0; i < point.size() - 2; ++i) {
		dx = 2.4 * b - a;
		a = b;
		b = dx;
	}

	sample = 32 * point.size();
	dy = 2.0 / (long double)sample;

	for (int i = 0; i <= sample; ++i) {
		px = point[0];
		for (int j = 1; j < point.size(); ++j) {
			px = y * px + point[j];
		}

		if (px < -1 || px > 1) {
			result += (1.0 - fabs(px)) * (1.0 - fabs(px));
		}
		y += dy;
	}

	for (int i = -1; i <= 1; i += 2) {
		px = point[0];

		for (int j = 1; j < point.size(); ++j) {
			px = 1.2 * px + point[j];
		}

		if (px < dx) {
			result += px * px;
		}
	}

	// We add one to guarantee that the global minimum is different from 0 (useful for fitness functions).
	return result + 1;
}


Rastrigin::Rastrigin(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double Rastrigin::operator()(vector<double> point) {
	double result = 0.0;
	shiftAndRotate(point);

	for (int i = 0; i < point.size(); ++i) {
		result += (point[i] * point[i] - 10.0 * cos(2.0 * PI * point[i]) + 10.0);
	}

	return result + 1;
}


Weierstrass::Weierstrass(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double Weierstrass::operator()(vector<double> point)  {
	int k_max;
	double sum, sum2, a, b, result;
	a = 0.5;
	b = 3.0;
	k_max = 20;
	result = 0.0;
	shiftAndRotate(point);

	for (int i = 0; i < point.size(); ++i) {
		sum = 0.0;
		sum2 = 0.0;
		for (int j = 0; j <= k_max; ++j)
		{
			sum += pow(a, j) * cos(2.0 * PI * pow(b, j) * (point[i] + 0.5));
			sum2 += pow(a, j) * cos(2.0 * PI * pow(b, j) * 0.5);
		}
		result += sum;
	}
	result -= point.size() * sum2;

	return result + 1;
}


Ackley::Ackley(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double Ackley::operator()(vector<double> point) {
	double result = 0.0, sum1 = 0.0, sum2 = 0.0;
	shiftAndRotate(point);

	for (int i = 0; i < point.size(); ++i) {
		sum1 += point[i] * point[i];
		sum2 += cos(2.0 * PI * point[i]);
	}
	sum1 = -0.2 * sqrt(sum1 / point.size());
	sum2 /= point.size();
	result = E - 20.0 * exp(sum1) - exp(sum2) + 20.0;
	return result + 1;
}


Griewank::Griewank(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double Griewank::operator()(vector<double> point) {
	double result = 0.0, s = 0.0, p = 1.0;
	shiftAndRotate(point);

	for (int i = 0; i < point.size(); ++i) {
		s += point[i] * point[i];
		p *= cos(point[i] / sqrt(1.0 + i));
	}
	result = 1.0 + s / 4000.0 - p;
	return result + 1;
}


HappyCat::HappyCat(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double HappyCat::operator()(vector<double> point) {
	double result = 0.0, alpha = 1.0 / 8.0, r2 = 0.0, sum_z = 0.0;
	shiftAndRotate(point);

	for (int i = 0; i < point.size(); ++i) {
		point[i] = point[i] - 1.0;//shift to orgin
		r2 += point[i] * point[i];
		sum_z += point[i];
	}
	result = pow(fabs(r2 - point.size()), 2 * alpha) + (0.5 * r2 + sum_z) / point.size() + 0.5;
	return result + 1;
}


Escaffer6::Escaffer6(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double Escaffer6::operator()(vector<double> point) {
	double result = 0.0, temp1, temp2;
	shiftAndRotate(point);

	for (int i = 0; i < point.size() - 1; ++i) {
		temp1 = sin(sqrt(point[i] * point[i] + point[i + 1] * point[i + 1]));
		temp1 = temp1 * temp1;
		temp2 = 1.0 + 0.001 * (point[i] * point[i] + point[i + 1] * point[i + 1]);
		result += 0.5 + (temp1 - 0.5) / (temp2 * temp2);
	}
	temp1 = sin(sqrt(point[point.size() - 1] * point[point.size() - 1] + point[0] * point[0]));
	temp1 = temp1 * temp1;
	temp2 = 1.0 + 0.001 * (point[point.size() - 1] * point[point.size() - 1] + point[0] * point[0]);
	result += 0.5 + (temp1 - 0.5) / (temp2 * temp2);
	return result + 1;
}


Schwefel::Schwefel(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double Schwefel::operator()(vector<double> point) {
	double result = 0.0, tmp;
	shiftAndRotate(point);

	for (int i = 0; i < point.size(); ++i) {
		point[i] += 4.209687462275036e+002;
		if (point[i] > 500)
		{
			result -= (500.0 - fmod(point[i], 500)) * sin(pow(500.0 - fmod(point[i], 500), 0.5));
			tmp = (point[i] - 500.0) / 100;
			result += tmp * tmp / point.size();
		}
		else if (point[i] < -500)
		{
			result -= (-500.0 + fmod(fabs(point[i]), 500)) * sin(pow(500.0 - fmod(fabs(point[i]), 500), 0.5));
			tmp = (point[i] + 500.0) / 100;
			result += tmp * tmp / point.size();
		}
		else
			result -= point[i] * sin(pow(fabs(point[i]), 0.5));
	}
	result += 4.189828872724338e+002 * point.size();
	return result + 1;
}


LennardJones::LennardJones(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double LennardJones::operator()(vector<double> point) {
	double result = 0.0;
	/* valid for any dimension, D=3*k, k=2,3,4,...,25.   k is the number of atoms in 3-D space
	constraints: unconstrained
	type: multi-modal with one global minimum; non-separable
	initial upper bound = 4, initial lower bound = -4
	value-to-reach = minima[k-2]+.0001
	f(x*) = minima[k-2]; see array of minima below; additional minima available at the
	Cambridge cluster database: http://www-wales.ch.cam.ac.uk/~jon/structures/LJ/tables.150.html
	*/
	int i, j, k, a, b, D;
	long double xd, yd, zd, ed, ud, sum = 0;


	static double minima[] = { -1.,-3.,-6.,-9.103852,-12.712062,-16.505384,-19.821489,-24.113360,
		-28.422532,-32.765970,-37.967600,-44.326801,-47.845157,-52.322627,-56.815742,-61.317995,
		-66.530949,-72.659782,-77.1777043,-81.684571,-86.809782,-02.844472,-97.348815,-102.372663 };

	k = point.size() / 3;

	if (k < 2)  // default if k<2
	{
		k = 2;
	}

	for (i = 0; i < k - 1; i++)
	{
		for (j = i + 1; j < k; j++)
		{
			a = 3 * i;
			b = 3 * j;
			xd = point[a] - point[b];
			yd = point[a + 1] - point[b + 1];
			zd = point[a + 2] - point[b + 2];
			ed = xd * xd + yd * yd + zd * zd;
			ud = ed * ed * ed;
			if (ud > 1.0e-10) sum += (1.0 / ud - 2.0) / ud;
			else sum += 1.0e20;
		}
	}

	result += sum;
	result += 12.7120622568;
	return result + 1.0;
}


Hilbert::Hilbert(size_t functionNumber, size_t dimensions, double shrinkRate) : AbstractFunction(functionNumber, dimensions, shrinkRate) {

}

double Hilbert::operator()(vector<double> point) {
	double result = 0.0;
	long long b;

	long double hilbert[10][10], y[10][10];			// Increase matrix size if D > 100

	b = (long long)sqrt((double)point.size());

	for (size_t i = 0; i < b; i++)
	{
		for (size_t j = 0; j < b; j++)
		{
			hilbert[i][j] = 1. / (double)(i + j + 1);		// Create a static Hilbert matrix
		}
	}

	for (size_t j = 0; j < b; j++)
	{
		for (size_t k = 0; k < b; k++)
		{
			y[j][k] = 0;
			for (size_t i = 0; i < b; i++)
			{
				y[j][k] += hilbert[j][i] * point[k + b * i];		// Compute matrix product H*x
			}
		}
	}


	for (size_t i = 0; i < b; i++)
	{
		for (size_t j = 0; j < b; j++)
		{
			if (i == j) result += fabs(y[i][j] - 1);				// Sum absolute value of deviations
			else result += fabs(y[i][j]);
		}
	}

	return result + 1.0;
}