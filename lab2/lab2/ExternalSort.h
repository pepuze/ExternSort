#pragma once
#include "FileSeriesHandler.h"
#include "Fibonacci.h"
#include <list>


//Шаблон класса внешних сортировок: Прямое слияние | Многофазная сортировка
template<class T>
class ExternalSort {
	unsigned int splitIn2Files(const std::string&, const std::string&, const std::string&, const unsigned int); //Разбиение значений из файла группами опред. размера
	void merge2InFile(const std::string&, const std::string&, const std::string&, const unsigned int); //Слияние в двух файлов в файл группами опред. размера
	bool dumpVals(std::ifstream&, std::ofstream&, unsigned int); //Считывание и вывод n значений из потока ввода в поток вывода, возвращает 1 если достигнут конец потока ввода
	bool multiphaseSplit(const std::string&, std::list<FileSeriesHandler<T>>&, const unsigned int); //Разбиение на файлы для многофазной сортировки, возвращает 1 если исход. файл отсортирован
	bool multiphaseMerge(std::list<FileSeriesHandler<T>>&); //Слияние для многофаз. сортировки, возвращает 1 если есть файлы, в к-х закончились серии
	void clearFiles(std::list<FileSeriesHandler<T>>&); //Избавление от всех файлов без серий кроме одного, перестановка этого файла в конец списка
	void copy(const std::string&, const std::string&); //Копирование значений из файла в файл
	unsigned int countSeries(const std::string&); //Подсчет к-ва серий в файле

public:
	void mergeSort(const std::string&); //Сортировка прямым слиянием
	void multiphaseSort(const std::string&, const unsigned int); //Многофазная сортировка 
};


template<class T>
void ExternalSort<T>::mergeSort(const std::string& mainFile) {
	const std::string subFile1 = "subFile1.txt", subFile2 = "subFile2.txt";
	for (unsigned int groupSize = 1; groupSize < splitIn2Files(mainFile, subFile1, subFile2, groupSize); groupSize *= 2)
		merge2InFile(mainFile, subFile1, subFile2, groupSize);
}

template<class T>
void ExternalSort<T>::multiphaseSort(const std::string& mainFile, const unsigned int nPhases) {
	std::list<FileSeriesHandler<T>> subFiles;
	if (multiphaseSplit(mainFile, subFiles, nPhases)) return;;

	while (subFiles.size() > 2) {
		while (!multiphaseMerge(subFiles)) {}
		clearFiles(subFiles);
	}
	copy(subFiles.front().getPath(), mainFile);
}

template<class T>
unsigned int ExternalSort<T>::splitIn2Files(const std::string& inputFile, const std::string& outputFile1, const std::string& outputFile2, const unsigned int groupSize) {
	std::ifstream input(inputFile);
	std::ofstream output1(outputFile1), output2(outputFile2);
	T temp;
	unsigned int nElements = 0;

	while (input >> temp) {
		output1 << temp << " ";
		if (++nElements % groupSize == 0) std::swap(output1, output2);
	}
	return nElements;
}

template<class T>
void ExternalSort<T>::merge2InFile(const std::string& outputFile, const std::string& inputFile1, const std::string& inputFile2, const unsigned int groupSize) {
	std::ofstream output(outputFile); 
	std::ifstream input1(inputFile1), input2(inputFile2);
	unsigned int groupCount1 = groupSize, groupCount2 = groupSize;
	T temp1, temp2;
	bool allValsProcessed = 0;

	while (!allValsProcessed) {
		if (groupCount1 == groupCount2 && groupCount1 == groupSize) { input1 >> temp1; input2 >> temp2; }
		if (temp1 < temp2) {
			output << temp1 << " ";
			if (!valReadable<T>(input1)) allValsProcessed = 1;
			if (--groupCount1 == 0 || allValsProcessed) {
				output << temp2 << " ";
				if (dumpVals(input2, output, groupCount2 - 1)) allValsProcessed = 1;
				groupCount1 = groupCount2 = groupSize;
				continue;
			}
			input1 >> temp1;
		}
		else {
			output << temp2 << " ";
			if (!valReadable<T>(input2)) allValsProcessed = 1;
			if (--groupCount2 == 0 || allValsProcessed) {
				output << temp1 << " ";
				if (dumpVals(input1, output, groupCount1 - 1)) allValsProcessed = 1;
				groupCount1 = groupCount2 = groupSize;
				continue;
			}
			input2 >> temp2;
		}
	}

	while (input1 >> temp1) output << temp1 << " ";
}

template<class T>
bool ExternalSort<T>::dumpVals(std::ifstream& from, std::ofstream& to, unsigned int amount) {
	T temp;
	while (amount--) {
		if (!(from >> temp)) return 1;
		to << temp << " ";
	}
	if (!valReadable<T>(from)) return 1;
	return 0;
}

template<class T>
unsigned int ExternalSort<T>::countSeries(const std::string& filePath) {
	unsigned int count = 1;
	std::ifstream input(filePath);
	T temp, tempPrev;
	if (!(input >> tempPrev)) return 0;
	while (input >> temp) {
		if (temp < tempPrev) ++count;
		tempPrev = temp;
	}
	return count;
}

template<class T>
bool  ExternalSort<T>::multiphaseSplit(const std::string& mainFile, std::list<FileSeriesHandler<T>>& subFiles, const unsigned int nPhases) {
	Fibonacci fib(nPhases - 2);
	unsigned int nSeries = countSeries(mainFile), fibIndex = 0;
	if (nSeries == 1) return 1;
	while (nSeries > fib(fibIndex + 1)) ++fibIndex; 
	std::ifstream mainFileInput(mainFile);
	for (unsigned int i = 0; i < nPhases - 1; ++i) subFiles.emplace_back("subFile" + std::to_string(i + 1) + ".txt", fib(fibIndex--), mainFileInput);
	subFiles.emplace_back("subFile" + std::to_string(nPhases) + ".txt", 0, mainFileInput);
	return 0;
}

template<class T>
bool ExternalSort<T>::multiphaseMerge(std::list<FileSeriesHandler<T>>& files) {
	typename std::list<FileSeriesHandler<T>>::iterator itLast = (--files.end());
	bool emptyFiles = 0, allSeriesFinished = 0;

	while (!allSeriesFinished) {
		typename std::list<FileSeriesHandler<T>>::iterator itMin = files.begin();
		allSeriesFinished = 1 && itMin->seriesFinished();
		for (typename std::list<FileSeriesHandler<T>>::iterator it = (++files.begin()); it != itLast; ++it) {
			if ((*it) < (*itMin)) itMin = it;
			allSeriesFinished = allSeriesFinished && it->seriesFinished();
		}
		*itMin >> *itLast;
		if (itMin->getSeries() == 0) emptyFiles = 1;
	}

	(*itLast).incSeries();
	for (typename std::list<FileSeriesHandler<T>>::iterator it = files.begin(); it != (--files.end()); ++it) it->newSeries();
	return emptyFiles;
}

template<class T>
void ExternalSort<T>::clearFiles(std::list<FileSeriesHandler<T>>& files) {
	auto itEmpty = files.end();
	files.back().flushStream();
	for (auto it = files.begin(); it != files.end();) {
		if (it->getSeries() == 0) {
			if (itEmpty == files.end()) itEmpty = it++;
			else {
				auto itErased = it++;
				files.erase(itErased);
			}
		}
		else ++it;
	}
	files.back().swap(*itEmpty);
	files.back().clearStream();
}

template<class T>
void ExternalSort<T>::copy(const std::string& from, const std::string& to) {
	std::ifstream input(from);
	std::ofstream output(to);
	T temp;
	while(input >> temp) output << temp << " ";
}