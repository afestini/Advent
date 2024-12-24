export module day25;

import std;

using namespace std;
using namespace std::chrono;


using Schematic = array<uint8_t, 5>;


static auto file_input() {
	ifstream input("day25.txt");

	vector<Schematic> keys;
	vector<Schematic> locks;

	string line;
	while (getline(input, line)) {
		auto& schematic = (line[0] == '#') ? locks.emplace_back() : keys.emplace_back();

		while (getline(input, line)) {
			if (line.empty()) break;

			for (auto [idx, c] : line | views::enumerate) {
				if (c == '#') schematic[idx]++;
			}
		}
	}
	return pair{move(locks), move(keys)};
}


export void day25() {
	const auto start_time = high_resolution_clock::now();

	const auto [locks, keys] = file_input();

	const auto key_lock_fit = [](const auto& lock_key) {
		const auto& [lock, key] = lock_key;  // Yes, 6. Not going to subtract one from each key position.
		return ranges::none_of(views::zip_transform(plus<>(), lock, key), bind_back(greater<>(), 6));
	};

	const auto count = ranges::count_if(views::cartesian_product(locks, keys), key_lock_fit);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 25a: {} ({})", static_cast<int>(count), duration);
}
