export module day1;

import std;

using namespace std;
using namespace std::chrono;


static generator<pair<int, int>> file_input() {
	ifstream input("day1.txt");
	string line;
	while (getline(input, line)) {
		pair<int, int> p;
		for (const auto& [idx, num] : views::split(line, "   "sv) | views::enumerate) {
			int& n = (idx == 0) ? p.first : p.second;
			from_chars(to_address(num.begin()), to_address(num.end()), n);
		}
		co_yield p;
	}
}


export void day1_1() {
	const auto start_time = high_resolution_clock::now();

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

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 1a: {} ({})", sum, duration);
}


export void day1_2() {
	const auto start_time = high_resolution_clock::now();

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

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 1b: {} ({})", similarity, duration);
}
