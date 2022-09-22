#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>
#include <iostream>

bool compareFiles(const std::string &p1, const std::string &p2) {
	std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
	std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

	if (f1.fail() || f2.fail()) {
		std::cout << "File problem" << std::endl;
		return false; // file problem
	}

	if (f1.tellg() != f2.tellg()) {
		std::cout << "size mismatch" << std::endl;
		return false; // size mismatch
	}

	// seek back to beginning and use std::equal to compare contents
	f1.seekg(0, std::ifstream::beg);
	f2.seekg(0, std::ifstream::beg);
	return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
					std::istreambuf_iterator<char>(),
					std::istreambuf_iterator<char>(f2.rdbuf()));
}

int main(int argc, char const* argv[]) {
	// check that the two files are exactly the same.
	std::string file1 = argv[1];
	std::string file2 = argv[2];
	printf("Are the two files the same? >>> %d <<< 1 = yes, 0 = no\n", compareFiles(file1, file2));
}