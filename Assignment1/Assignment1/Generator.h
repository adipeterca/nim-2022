#pragma once
#include <random>
#include <chrono>

using namespace std;

class Generator
{
private:
	static mt19937* generator;

	Generator();

public:

	/// <summary>
	/// Static access method
	/// </summary>
	/// <returns>mt19937 initialized instance</returns>
	static mt19937* getInstance() {
		if (generator == nullptr) {
			int chrono_seed = chrono::system_clock::now().time_since_epoch().count();
			mt19937 mt(chrono_seed);
			int seed = mt();
			mt19937 gen(seed);
			generator = &gen;
		}
		return generator;
	}
};