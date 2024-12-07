export module day8;

import std;

using namespace std;


static void file_input() {
	ifstream input("day8.txt");
	string line;
	while (getline(input, line)) {
	}
}


export void day8_1() {
	const auto start = chrono::high_resolution_clock::now();



	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 8a: {} ({})", 0, duration);
}


export void day8_2() {
	const auto start = chrono::high_resolution_clock::now();



	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 8b: {} ({})", 0, duration);
}
