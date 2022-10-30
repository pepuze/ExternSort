#include "ExternalSortDemo.h"
#include <random>
#include <chrono>
#include <algorithm>

void ExternalSortDemo::printMenu() {
	std::cout << "1 - ��������� ������� ���������� � ����������\n";
	std::cout << "2 - ����� ��������������� ���������� ������\n";
	std::cout << "3 - ���������� ������ ���������\n";
}

bool ExternalSortDemo::doAction(const char action) {
	char sortType;
	unsigned int nPhases, nVals;
	int lBound, rBound;
	if (action < '1' || action > '3') {
		std::cout << "������� �������������� �����\n";
		return 0;
	}
	else if (action == '3') return 1;

	std::cout << "�������� ��� ����������:\n";
	std::cout << "1 - ���������� ������ ��������\n";
	std::cout << "2 - ����������� ����������\n";
	std::cin >> sortType;
	if (sortType == '2') {
		std::cout << "������� ���������� ��� ����������:\n";
		std::cin >> nPhases;
	}
	if (action == '1') {
		std::cout << "������� ���������� ������������� ���������:\n";
		std::cin >> nVals;
		std::cout << "������� ������� ��������� [l, r]:\n";
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
	std::cout << "����� ���������� �� ���������� ������ (std::sort): " << stdSortDuration.count() << " ��\n";

	start = std::chrono::high_resolution_clock::now();
	externSort.mergeSort(filePath);
	end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds mergeSortDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "����� ���������� �� ������� ������ (������ �������): " << mergeSortDuration.count() << " ��\n";
}

void ExternalSortDemo::compareMultiphaseWithInternal(const std::string& filePath, const unsigned int nPhases, std::vector<int>& arr) {
	auto start = std::chrono::high_resolution_clock::now();
	std::sort(arr.begin(), arr.end());
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds stdSortDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "����� ���������� �� ���������� ������ (std::sort): " << stdSortDuration.count() << " ��\n";

	start = std::chrono::high_resolution_clock::now();
	externSort.multiphaseSort(filePath, nPhases);
	end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds multiphaseSortDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "����� ���������� �� ������� ������ (����������� ����������): " << multiphaseSortDuration.count() << " ��\n";
}
