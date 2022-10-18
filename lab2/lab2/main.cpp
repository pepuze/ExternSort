#include "ExternalSort.h"

int main() {
	ExternalSort<int> extSort;
	extSort.mergeSort("test.txt");
	return 0;
}