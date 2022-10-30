#include "ExternalSortDemo.h"
#include <random>
#include <chrono>
#include <algorithm>

void ExternalSortDemo::printMenu() {
	std::cout << "1 - сравнение внешней сортировки с внутренней\n";
	std::cout << "2 - режим статистического накопления данных\n";
	std::cout << "3 - завершение работы программы\n";
}

bool ExternalSortDemo::doAction(const char action) {
	char sortType;
	unsigned int nPhases, nVals;
	int lBound, rBound;
	if (action < '1' || action > '3') {
		std::cout << "Выбрана несуществующая опция\n";
		return 0;
	}
	else if (action == '3') return 1;

	std::cout << "Выберите тип сортировки:\n";
	std::cout << "1 - сортировка прямым слиянием\n";
	std::cout << "2 - многофазная сортировка\n";
	std::cin >> sortType;
	if (sortType == '2') {
		std::cout << "Введите количество фаз сортировки:\n";
		std::cin >> nPhases;
	}
	if (action == '1') {
		std::cout << "Введите количество сгенерируемых элементов:\n";
		std::cin >> nVals;
		std::cout << "Введите границы генерации [l, r]:\n";
		std::cin >> lBound >> rBound;
		
		std::vector<int> arr(nVals);
		const std::string fileUnsorted = "unsortedVals.txt", fileSorted = "sortedVals.txt";
		std::ofstream unsorted(fileUnsorted), sorted(fileSorted);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(lBound, rBound);
		int temp;

		for (unsigned int i = 0; i < nVals; ++i) {
			temp = dist(gen);
			arr[i] = temp;
			unsorted << temp << " ";
			sorted << temp << " ";
		}
		unsorted.close(); sorted.close();

		if (sortType == '1') compareMergeWithInternal(fileSorted, arr);
		else compareMultiphaseWithInternal(fileSorted, nPhases, arr);
		return 0;
	}
	return 0;
}

void ExternalSortDemo::compareMergeWithInternal(const std::string& filePath, std::vector<int>& arr) {
	auto start = std::chrono::high_resolution_clock::now();
	std::sort(arr.begin(), arr.end());
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds stdSortDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
	std::cout << "Время сортировки во внутренней памяти (std::sort): " << stdSortDuration.count() << " мс\n";

	start = std::chrono::high_resolution_clock::now();
	externSort.mergeSort(filePath);
	end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds mergeSortDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Время сортировки во внешней памяти (Прямое слияние): " << mergeSortDuration.count() << " мс\n";
}

void ExternalSortDemo::compareMultiphaseWithInternal(const std::string& filePath, const unsigned int nPhases, std::vector<int>& arr) {
	auto start = std::chrono::high_resolution_clock::now();
	std::sort(arr.begin(), arr.end());
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds stdSortDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Время сортировки во внутренней памяти (std::sort): " << stdSortDuration.count() << " мс\n";

	start = std::chrono::high_resolution_clock::now();
	externSort.multiphaseSort(filePath, nPhases);
	end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds multiphaseSortDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Время сортировки во внешней памяти (Многофазная сортировка): " << multiphaseSortDuration.count() << " мс\n";
}
