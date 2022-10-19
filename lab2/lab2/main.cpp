#include "ExternalSort.h"
#include <string>

int main() {
	ExternalSort<int> extSort;
	extSort.mergeSort("test.txt");
	return 0;
}