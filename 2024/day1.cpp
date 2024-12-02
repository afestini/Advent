export module day1;

import std;

using namespace std;


static generator<pair<int, int>> file_input() {
	ifstream input("day1.txt");
	while (input) {
		int left_num = 0;
		int right_num = 0;
		if (input >> left_num >> right_num)
			co_yield {left_num, right_num};
	}
}


export void day1_1() {
	const auto start = chrono::high_resolution_clock::now();

	vector<int> left;
	vector<int> right;

	for (auto [left_num, right_num] : file_input()) {
		left.emplace_back(left_num);
		right.emplace_back(right_num);
	}

	ranges::sort(left);
	ranges::sort(right);

	uint64_t sum = 0;
	for (const auto [left_num, right_num] : views::zip(left, right)) {
		sum += abs(right_num - left_num);
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 1a: {} ({})", sum, duration);
}


export void day1_2() {
	const auto start = chrono::high_resolution_clock::now();

	vector<int> left;
	unordered_map<int, uint64_t> counts;

	for (auto [left_num, right_num] : file_input()) {
		left.emplace_back(left_num);
		++counts[right_num];
	}

	uint64_t similarity = 0;
	for (const auto left_no : left) {
		similarity += left_no * counts[left_no];
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 1b: {} ({})", similarity, duration);
}
