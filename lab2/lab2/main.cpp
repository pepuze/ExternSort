#include "ExternalSort.h"

int main() {
	ExternalSort<int> extSort;
	extSort.multiphaseSort("test.txt", 5);
	return 0;
}