#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Fibonacci.h"


template<class T>
class ExternalSort {
	unsigned int splitIn2Files(std::fstream&, std::fstream&, std::fstream&, const unsigned int); //Разбиение значений из осн. файла в вспомогательные группами по n
	void mergeInFile(std::fstream&, std::fstream&, std::fstream&, const unsigned int); //Слияние упорядоченных последовательностей из вспомог. файлов в осн.
	void printValues(const std::string&);
	void clearFile(const std::string& path) { std::ofstream file(path, std::ios::out | std::ofstream::trunc); }; //Отчистка файла
	bool dumpValues(std::fstream&, std::fstream&, unsigned int); //Сливание n значений из одного файла в другой
	bool valuesExist(std::fstream&); //Проверка на наличие в потоке значений, к-х можно считать

public:
	void mergeSort(const std::string&); //Сортировка прямым слиянием
	void multiphaseSort(const std::string&, const unsigned int); //Многофазная сортировка 
	unsigned int countSeries(const std::string&); //Подсчет серий в файле path

};


template<class T>
void ExternalSort<T>::mergeSort(const std::string& path) {
	const std::string sub1 = "subFile1.txt", sub2 = "subFile2.txt"; //Имена вспомогательных файлов
	std::fstream mainFile(path, std::ios::in | std::ios::out), subFile1(sub1, std::ios::in | std::ios::out | std::fstream::trunc), subFile2(sub2, std::ios::in | std::ios::out | std::fstream::trunc);
	for (unsigned int groupSize = 1; groupSize < splitIn2Files(mainFile, subFile1, subFile2, groupSize); groupSize *= 2) //Пока общее кол-во элементов < размера упоряд. послед. 
	{
		printValues(path);
		printValues(sub1); printValues(sub2);
		clearFile(path);
		mergeInFile(mainFile, subFile1, subFile2, groupSize); //Слияние упорядоченных групп размера groupSize в осн. файл
		clearFile(sub1); clearFile(sub2);
	}
	printValues(path);
}

template<class T>
void ExternalSort<T>::multiphaseSort(const std::string& path, const unsigned int nPhases) {
	std::vector<std::fstream> subFiles(nPhases);
	for (unsigned int i = 0; i < nPhases; ++i) subFiles[i].open("m" + std::to_string(i) + ".txt", std::ios::out | std::ios::in | std::ofstream::trunc);
	const unsigned int nSeries = countSeries(path);


}

template<class T>
unsigned int ExternalSort<T>::splitIn2Files(std::fstream& input, std::fstream& output1, std::fstream& output2, const unsigned int groupSize) {
	input.clear(); output1.clear(); output2.clear();
	input.seekg(std::ios::beg); output1.seekp(std::ios::beg); output2.seekp(std::ios::beg);
	T temp;
	unsigned int size = 0;
	bool swapped = 0;
	while (input >> temp) {
		output1 << temp << " ";
		++size;
		if (size % groupSize == 0) {
			std::swap(output1, output2);
			swapped == !swapped;
		}
	}
	if (swapped) std::swap(output1, output2);
	output1.flush(); output2.flush();
	return size;
}

template<class T>
void ExternalSort<T>::mergeInFile(std::fstream& output, std::fstream& input1, std::fstream& input2, const unsigned int groupSize) {
	output.clear(); input1.clear(); input2.clear();
	output.seekp(std::ios::beg); input1.seekg(std::ios::beg); input2.seekg(std::ios::beg);
	T temp1, temp2;
	unsigned int groupSize1 = groupSize, groupSize2 = groupSize;
	bool allValuesProcessed = 0;
	
	while (!allValuesProcessed) {
		if (groupSize1 == groupSize && groupSize2 == groupSize) { input1 >> temp1; input2 >> temp2; }

		if (temp1 < temp2) {
			output << temp1 << " ";
			if (!valuesExist(input1)) allValuesProcessed = 1;
			if (--groupSize1 == 0 || allValuesProcessed) {
				output << temp2 << " ";
				if (!dumpValues(input2, output, groupSize2 - 1)) allValuesProcessed = 1;
				groupSize1 = groupSize2 = groupSize;
				continue;
			}
			else if(!allValuesProcessed) input1 >> temp1;
		}
		else {
			output << temp2 << " ";
			if (!valuesExist(input2)) allValuesProcessed = 1;
			if (--groupSize2 == 0 || allValuesProcessed) {
				output << temp1 << " ";
				if (!dumpValues(input1, output, groupSize1 - 1)) allValuesProcessed = 1;
				groupSize1 = groupSize2 = groupSize;
				continue;
			}
			else if (!allValuesProcessed) input2 >> temp2;
		}
	}

	while (input1 >> temp1) output << temp1 << " ";
	while (input2 >> temp2) output << temp2 << " ";
	output.flush();
}

template<class T>
bool ExternalSort<T>::valuesExist(std::fstream& input) {
	const std::ios::streampos pos = input.tellg();
	T temp;
	if (input >> temp) {
		input.seekg(pos);
		return 1;
	}
	return 0;
}

template<class T>
bool ExternalSort<T>::dumpValues(std::fstream& from, std::fstream& to, unsigned int amount) {
	T temp;
	while (amount--) {
		if (from >> temp) to << temp << " ";
		else return 0;
	}
	if (!valuesExist(from)) return 0;
	return 1;
}

template<class T>
void ExternalSort<T>::printValues(const std::string& path) {
	std::cout << path << ": ";
	std::ifstream input(path);
	T temp;
	while (input >> temp) { std::cout << temp << " "; }
	std::cout << "\n";
	input.close();
};

template<class T>
unsigned int ExternalSort<T>::countSeries(const std::string& path) {
	std::ifstream file(path);
	unsigned int count = 1;
	T temp, tempPrev;
	if (!(file >> tempPrev)) return 0;
	while (file >> temp) {
		if (temp < tempPrev) ++count;
		tempPrev = temp;
	}
	return count;
}