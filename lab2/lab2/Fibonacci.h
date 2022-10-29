#pragma once
#include <vector>

//Класс подсчета значений последовательности Фибоначчи n порядка
struct Fibonacci {
	Fibonacci(const unsigned int);
	Fibonacci();
	
	unsigned long long operator()(const unsigned int);
	unsigned int getOrder() const { return order; }

private:
	const unsigned int order;
	std::vector<unsigned long long> Fibs;
};