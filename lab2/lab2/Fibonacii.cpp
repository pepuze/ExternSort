#include "Fibonacci.h"

Fibonacci::Fibonacci(const unsigned int _order) : order(_order) {
	Fibs.resize((unsigned long long)order + 1, 0);
	Fibs[order] = 1;
}

Fibonacci::Fibonacci() : Fibonacci::Fibonacci(1) {}

unsigned long long Fibonacci::operator()(const unsigned int n) {
	if (n < order) return Fibs[n];
	if (n >= Fibs.size()) {
		unsigned long long fib = 0;
		for (unsigned int i = n - 1; (n - i) <= (order + 1); --i) fib += this->operator()(i);
		Fibs.push_back(fib);
	}
	return Fibs[n];
}

unsigned long long Fibonacci::sumNeighbours(const unsigned int index) {
	if (index < order) return 0;
	unsigned long long sum = 0;
	unsigned int mltp = order + 1;
	for (unsigned int i = index; (index - i) < (order + 1); --i, --mltp)
		sum += mltp * this->operator()(i);
	return sum;
}