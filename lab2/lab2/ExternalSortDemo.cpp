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
	unsigned int nPhases = 0, nVals;
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
		std::cout << "Сгенерированные значения выведены в " << fileUnsorted << "\n";

		if (sortType == '1') compareMergeWithInternal(fileSorted, arr);
		else if(sortType == '2') compareMultiphaseWithInternal(fileSorted, nPhases, arr);
		std::cout << "Отсортированные значения выведены в " << fileSorted << "\n";
		return 0;
	}
	if (action == '2') {
		unsigned int step, genType;
		std::cout << "Введите границы отрезка генерации [l, r]:\n";
		std::cin >> lBound >> rBound;
		std::cout << "Введите шаг отрезка генерации:\n";
		std::cin >> step;
		std::cout << "Выберите режим генерации значений:\n";
		std::cout << "1 - случайные значения [1, n]\n";
		std::cout << "2 - частично отсортированные значения\n";
		std::cout << "3 - упорядоченные в обратном порядке значения\n";
		std::cin >> genType;
		
		std::string filePath, temp = "temp.txt";
		if (sortType == '1') filePath += "MergeSort";
		else if (sortType == '2') filePath += "MultiphaseSort";
		if (genType == 1) filePath += "Random";
		else if (genType == 2) filePath += "PartiallySorted";
		else if (genType == 3) filePath += "Reversed";
		filePath += ".txt";
		std::ofstream output(filePath);

		if (nPhases) output << nPhases << "-фазная сортировка\n";
		output << "Пары значений типа 'Количество элементов': 'время'\n";
		std::chrono::steady_clock::time_point start, end;
		for (; lBound <= rBound; lBound += step) {
			genVals(temp, genType, lBound);
			start = std::chrono::high_resolution_clock::now();
			if (sortType == '1') externSort.mergeSort(temp);
			else if (sortType == '2') externSort.multiphaseSort(temp, nPhases);
			end = std::chrono::high_resolution_clock::now();
			output << lBound << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " мс\n";
		}
		std::cout << "Результаты измерений выведены в " << filePath << "\n";
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

void ExternalSortDemo::genVals(const std::string& filePath, const unsigned int genType, const unsigned int amount) {
	std::ofstream output(filePath);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distHalf(amount / 2 + 1, amount), distRand(1, amount);
	switch (genType) {
	case 1:
		for (unsigned int i = 0; i < amount; ++i) output << distRand(gen) << " ";
		break;
	case 2:
		for (unsigned int i = 0; i < amount/2; ++i) output << i+1 << " ";
		for (unsigned int i = amount / 2; i < amount; ++i) output << distHalf(gen) << " ";
		break;
	case 3:
		for (unsigned int i = amount; i > 0; --i) output << i << " ";
		break;
	}
}