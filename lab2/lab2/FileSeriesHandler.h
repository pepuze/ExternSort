#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

template<class T>
bool valReadable(std::ifstream& input) {
	T temp;
	const auto pos = input.tellg();
	if (input >> temp) {
		input.seekg(pos);
		return 1;
	}
	return 0;
}

template<class T>
bool valReadable(std::ifstream& input, T& t) {
	T temp;
	const auto pos = input.tellg();
	if (input >> t) {
		input.seekg(pos);
		return 1;
	}
	return 0;
}



template<class T>
class FileSeriesHandler {
	std::string filePath;
	std::fstream fileStream;
	unsigned int series;
	bool seriesEnd, fileEnd;
	T nextVal, prevVal;

	bool setNext();

public:
	FileSeriesHandler(const std::string&, const unsigned int, std::ifstream&);
	~FileSeriesHandler();

	bool operator<(const FileSeriesHandler<T>&);
	
	template<class U>
	friend FileSeriesHandler<U>& operator>>(FileSeriesHandler<U>&, FileSeriesHandler<U>&);

	unsigned int getSeries() const { return series; }
	bool seriesFinished() const { return seriesEnd; }
	T getNext() const { return nextVal; }
	T peekNext();
	std::string getPath() const { return filePath; }
	void swap(FileSeriesHandler<T>&);
	void newSeries() { seriesEnd = 0; }
	void incSeries() { ++series; }
	void flushStream() { fileStream.flush(); fileStream.seekg(0); nextVal = peekNext(); }
	void clearStream() { fileStream.clear(); fileStream.seekp(0); fileStream.seekg(0); std::ofstream clearFile(filePath); }
};


template<class T>
FileSeriesHandler<T>::FileSeriesHandler(const std::string& _filePath, const unsigned int _series, std::ifstream& input) : filePath(_filePath), series(_series) {
	fileStream.open(filePath, std::ios::out | std::ios::in | std::ios::trunc);
	T temp, tempPrev;
	if (!series) {
		fileEnd = 1;
		seriesEnd = 1;
		return;
	}
	if (input >> tempPrev) {
		nextVal = tempPrev;
		fileEnd = 0;
		for (unsigned int i = 0; 1;) {
			fileStream << tempPrev << " ";
			if (!valReadable<T>(input, temp)) break;
			if (temp < tempPrev) {
				if (++i >= series) break;
			}
			input >> tempPrev;
			tempPrev = temp;
		}
	}
	else fileEnd = 1;
	fileStream.flush();
	fileStream.seekg(0);
	seriesEnd = 0;
}

template<class T>
FileSeriesHandler<T>::~FileSeriesHandler() {
	//std::remove(filePath);
}

template<class T>
bool FileSeriesHandler<T>::operator<(const FileSeriesHandler<T>& fsHandler) {
	if (seriesEnd || fsHandler.seriesEnd) return !seriesEnd;
	return ((nextVal < fsHandler.nextVal) || fileEnd || fsHandler.fileEnd);
}

template<class T>
FileSeriesHandler<T>& operator>>(FileSeriesHandler<T>& fsInput, FileSeriesHandler<T>& fsOutput) {
	if (fsInput.seriesEnd) return fsInput;
	if (!fsInput.fileEnd) {
		fsOutput.fileStream << fsInput.nextVal << " ";
		fsOutput.fileEnd = 0;
	}
	fsInput.setNext();
	return fsInput;
}

template<class T>
bool FileSeriesHandler<T>::setNext() {
	if (fileEnd) {
		--series;
		seriesEnd = 1;
	}
	else {
		fileStream >> prevVal;
		auto pos = fileStream.tellg();
		if (fileStream >> nextVal) {
			fileStream.seekg(pos);
			if (nextVal < prevVal) {
				seriesEnd = 1;
				--series;
			}
		}
		else {
			fileEnd = 1;
			seriesEnd = 1;
			--series;
		}
	}
	return fileEnd;
}

template<class T>
void FileSeriesHandler<T>::swap(FileSeriesHandler<T>& other) {
	std::swap(filePath, other.filePath);
	std::swap(series, other.series);
	std::swap(fileStream, other.fileStream);
	std::swap(fileEnd, other.fileEnd);
	std::swap(seriesEnd, other.seriesEnd);
	T temp = nextVal; nextVal = other.nextVal; other.nextVal = temp;
	temp = prevVal; prevVal = other.prevVal; other.prevVal = temp;
}

template<class T>
T FileSeriesHandler<T>::peekNext() {
	T temp;
	const auto pos = fileStream.tellg();
	fileStream >> temp;
	fileStream.seekg(pos);
	return temp;
}