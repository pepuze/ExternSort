#pragma once
#include <iostream>
#include <fstream>


template<class T>
class ExternalSort {
	unsigned int splitIn2Files(std::fstream&, std::fstream&, std::fstream&, const unsigned int);
	void mergeInFile(std::fstream&, std::fstream&, std::fstream&, const unsigned int);

	void printFile(const std::string& path) {
		std::cout << path << ": ";
		std::ifstream input(path);
		T temp;
		while (input >> temp) { std::cout << temp << " "; }
		std::cout << "\n";
		input.close();
	};

	void clearFile(const std::string& path) { std::ofstream file(path); };
	bool valuesExist(std::fstream&);

public:
	void mergeSort(const std::string& inputPath);
};


template<class T>
void ExternalSort<T>::mergeSort(const std::string& path) {
	const std::string sub1 = "subFile1.txt", sub2 = "subFile2.txt";
	std::fstream mainFile(path, std::ios::in | std::ios::out), subFile1(sub1, std::ios::in | std::ios::out | std::fstream::trunc), subFile2(sub2, std::ios::in | std::ios::out | std::fstream::trunc);
	char c;
	printFile(path);

	for (unsigned int groupSize = 1; groupSize < splitIn2Files(mainFile, subFile1, subFile2, groupSize); groupSize *= 2) {
		printFile(sub1); printFile(sub2);
		std::cin >> c;
		std::cout << "Merging...\n";
		mergeInFile(mainFile, subFile1, subFile2, groupSize);
		printFile(path);
	}
}

template<class T>
unsigned int ExternalSort<T>::splitIn2Files(std::fstream& input, std::fstream& output1, std::fstream& output2, const unsigned int groupSize) {
	input.clear(); output1.clear(); output2.clear();
	input.seekg(std::ios::beg); output1.seekp(std::ios::beg); output2.seekp(std::ios::beg);
	T temp;
	unsigned int size = 0;
	while (input >> temp) {
		output1 << temp << " ";
		++size;
		if (size % groupSize == 0) {
			std::swap(output1, output2);
		}
	}
	output1.flush(); output2.flush();
	return size;
}

template<class T>
void ExternalSort<T>::mergeInFile(std::fstream& output, std::fstream& input1, std::fstream& input2, const unsigned int groupSize) {
	output.clear(); input1.clear(); input2.clear();
	output.seekp(std::ios::beg); input1.seekg(std::ios::beg); input2.seekg(std::ios::beg);
	T temp1, temp2;
	unsigned int groupSize1 = groupSize, groupSize2 = groupSize;

	while (valuesExist(input1) && valuesExist(input2)) {
		if (input1 >> temp1 && (input2 >> temp2 || temp1 <= temp2)) {
			output << temp1 << " ";
			if (--groupSize1 == 0) {
				output << temp2 << " ";
				while (groupSize2-- && valuesExist(input2)) {
					input2 >> temp2;
					output << temp2 << " ";
				}
				groupSize1 = groupSize2 = groupSize;
			}
		}
		else {
			output << temp2 << " ";
			if (--groupSize2 == 0) {
				output << temp1 << " ";
				while (groupSize1-- && valuesExist(input1)) {
					input1 >> temp1;
					output << temp1 << " ";
				}
				groupSize1 = groupSize2 = groupSize;
			}
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