#pragma once
#include "ExternalSort.h"

class ExternalSortDemo {
	ExternalSort<int> externSort;
	void genVals(const std::string&, const unsigned int, const unsigned int);

public:
	void printMenu();
	bool doAction(const char);
	void compareMergeWithInternal(const std::string&, std::vector<int>&);
	void compareMultiphaseWithInternal(const std::string&, const unsigned int, std::vector<int>&);
};