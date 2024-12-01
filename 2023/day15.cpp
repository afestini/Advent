export module day15;

import std;

using namespace std;


static string ReadInput() {
	ifstream input("day15.txt");

	string line;
	getline(input, line);
	return line;
}


static uint8_t Hash(const auto& str) {
	return ranges::fold_left(str, 0, [](uint8_t hash, char ch) { return (hash + ch) * 17; });
}


export void day15_1() {
	const auto start = chrono::high_resolution_clock::now();

	auto input = ReadInput();

	const auto sum = ranges::fold_left(input | views::split(','), 0LL,
	                                   [](int64_t sum, const auto& str) { return sum + Hash(str); });

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 15a: {} ({})", sum, duration);
}


export void day15_2() {
	const auto start = chrono::high_resolution_clock::now();

	auto input = ReadInput();

	array<vector<pair<string_view, int>>, 256> boxes;

	for (const auto& str : input | views::split(',')) {
		const auto label = string_view(str.begin(), str.end() - (str.back() == '-' ? 1 : 2));
		const auto focal = str.back() - '0'; // Garbage if op is -
		auto& box = boxes[Hash(label)];

		if (str.back() == '-')
			erase_if(box, [label](const auto& p) { return p.first == label; });
		else if (auto it = ranges::find(box, label, &pair<string_view, int>::first); it != box.end())
			it->second = focal;
		else
			box.emplace_back(label, focal);
	}

	int64_t sum = 0;
	for (auto [box_nr, box] : boxes | views::enumerate) {
		for (auto [slot, focal] : box | views::values | views::enumerate) {
			sum += (box_nr + 1) * (slot + 1) * focal;
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 15b: {} ({})", sum, duration);
}
