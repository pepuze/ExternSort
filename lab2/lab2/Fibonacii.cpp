#include "Fibonacci.h"

Fibonacci::Fibonacci(const unsigned int _order) : order(_order) {
	Fibs.resize((unsigned long long)order + 1);
	for (unsigned int i = 0; i < order; ++i) Fibs[i] = 0;
	Fibs[order] = 1;
}

Fibonacci::Fibonacci() : Fibonacci::Fibonacci(1) {}

unsigned long long Fibonacci::operator()(const unsigned int n) {
	if (n < order) return Fibs[n];
	if (n >= Fibs.size()) {
		Fibs.push_back(0);
		for (unsigned int i = n - 1; (n - i) <= (order + 1); --i) Fibs[n] += this->operator()(i);
	}
	return Fibs[n];
}