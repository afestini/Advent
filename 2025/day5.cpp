export module day5;

import std;
import utils;

using namespace std;
using namespace std::chrono;


struct Data {
	vector<pair<uint64_t, uint64_t>> ranges;
	vector<uint64_t> ids;
};


static Data read_input() {
	ifstream input("day5.txt");

	bool ranges = true;
	Data data;
	string line;
	while (getline(input, line)) {
		if (line.empty()) {
			ranges = false;
			continue;
		}

		if (ranges) {
			const auto from_to = split<2>(line, "-");
			data.ranges.emplace_back(str_as<uint64_t>(from_to[0]), str_as<uint64_t>(from_to[1]));
		}
		else {
			data.ids.emplace_back(stoull(line));
		}
	}
	return data;
}


static void collapse_ranges(Data& data) {
	ranges::sort(data.ranges, {}, &pair<uint64_t, uint64_t>::first);

	for (size_t i = 0; i < data.ranges.size() - 1;) {
		if (data.ranges[i].second >= data.ranges[i + 1].first) {
			data.ranges[i].second = max(data.ranges[i].second, data.ranges[i + 1].second);
			data.ranges.erase(data.ranges.begin() + i + 1);
		}
		else ++i;
	}
}


export void day5_1() {
	const auto start_time = high_resolution_clock::now();

	auto data = read_input();
	collapse_ranges(data);

	const auto fresh = ranges::count_if(data.ids, [&data](auto id) {
		return ranges::any_of(data.ranges, [id](auto r) { return id >= r.first && id <= r.second; });
	});

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 5a: {} ({})", fresh, duration);
}


export void day5_2() {
	const auto start_time = high_resolution_clock::now();

	auto data = read_input();
	collapse_ranges(data);

	const auto counts = data.ranges | views::transform([](auto r){return r.second - r.first + 1;});
	const auto sum = reduce(counts.begin(), counts.end());

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 5b: {} ({})", sum, duration);
}
