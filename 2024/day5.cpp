export module day5;

import std;

using namespace std;

using Ordering = vector<pair<int, int>>;
using Update = vector<int>;


static auto file_input() {
	ifstream input("day5.txt");
	bool parse_order = true;
	Ordering ordering;
	vector<Update> updates;

	string line;
	while (getline(input, line)) {
		if (line.empty()) {
			parse_order = false;
			continue;
		}

		if (parse_order) {
			auto& o = ordering.emplace_back();
			for (const auto& num : views::split(line, '|')) {
				from_chars(num.data(), to_address(num.end()), o.first ? o.second : o.first);
			}
		}
		else {
			updates.emplace_back();
			for (const auto& num : views::split(line, ',')) {
				updates.back().emplace_back();
				from_chars(num.data(), to_address(num.end()), updates.back().back());
			}
		}
	}
	ranges::sort(ordering);
	return pair{move(ordering), move(updates)};
}


export void day5_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto [ordering, updates] = file_input();

	const auto is_before = [&ordering](int a, int b) {
		return ranges::binary_search(ordering, pair {a, b});
	};

	int64_t total = 0;
	for (const auto& update : updates) {
		if (ranges::is_sorted(update, is_before))
			total += update[update.size() / 2];
	}
	
	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 5a: {} ({})", total, duration);
}


export void day5_2() {
	const auto start = chrono::high_resolution_clock::now();

	auto [ordering, updates] = file_input();

	const auto is_before = [&ordering](int a, int b) {
		return ranges::binary_search(ordering, pair{a, b});
	};

	int64_t total = 0;
	for (auto& update : updates) {
		if (!ranges::is_sorted(update, is_before)) {
			ranges::sort(update, is_before);
			total += update[update.size() / 2];
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 5b: {} ({})", total, duration);
}
