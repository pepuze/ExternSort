#include "ExternalSortDemo.h"
#include <Windows.h>

int main() {
	system("chcp 1251");
	char action;
	ExternalSortDemo demo;
	while (1) {
		demo.printMenu();
		std::cin >> action;
		if (demo.doAction(action)) break;
		std::cout << "\n";
	}
	return 0;
}