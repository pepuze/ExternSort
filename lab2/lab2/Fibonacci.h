#pragma once
#include <vector>

struct Fibonacci {
	Fibonacci(const unsigned int);
	Fibonacci();
	
	unsigned long long operator()(const unsigned int);

private:
	const unsigned int order;
	std::vector<unsigned long long> Fibs;
};