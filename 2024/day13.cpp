export module day13;

import std;
import utils;

using namespace std;
using namespace std::chrono;


struct MachineConfig {
	Vec2<int64_t> a;
	Vec2<int64_t> b;
	Vec2<int64_t> prize;
};


static vector<MachineConfig> file_input() {
	ifstream input("day13.txt");

	vector<MachineConfig> config;

	string line_str;
	config.emplace_back();

	while (getline(input, line_str)) {
		if (line_str.empty()) {
			config.emplace_back();
			continue;
		}

		Vec2<int64_t>* reading = nullptr;
		if (line_str.starts_with("Button A:")) reading = &config.back().a;
		else if (line_str.starts_with("Button B:")) reading = &config.back().b;
		else if (line_str.starts_with("Prize:")) reading = &config.back().prize;

		string_view line = string_view(line_str).substr(line_str.find(": ") + 2);
		for (auto [idx, token] : views::split(line, ", "sv) | views::enumerate) {
			const string_view t = string_view(token).substr(2);
			from_chars(t.data(), to_address(t.end()), idx ? reading->y : reading->x);
		}
	}

	return config;
}


static int64_t solve(const MachineConfig& cfg) {
	const auto a = (cfg.prize.x * cfg.b.y - cfg.prize.y * cfg.b.x) / (cfg.a.x * cfg.b.y - cfg.a.y * cfg.b.x);
	const auto b = (cfg.prize.y * cfg.a.x - cfg.prize.x * cfg.a.y) / (cfg.a.x * cfg.b.y - cfg.a.y * cfg.b.x);
	return (a * cfg.a + b * cfg.b == cfg.prize) ? (a * 3 + b) : 0;
}


export void day13_1() {
	const auto start_time = high_resolution_clock::now();

	const auto tokens = ranges::fold_left(file_input() | views::transform(solve), 0LL, plus<>());

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 13a: {} ({})", tokens, duration);
}


export void day13_2() {
	const auto start_time = high_resolution_clock::now();

	int64_t tokens = 0;
	for (auto& cfg : file_input()) {
		cfg.prize += Vec2<int64_t>{10000000000000, 10000000000000};
		tokens += solve(cfg);
	}

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 13b: {} ({})", tokens, duration);
}
