export module day9;

import std;

using namespace std;


static vector<vector<int64_t>> ParseValues() {
	ifstream input("day9.txt");
	if (!input) return {};

	const auto subrange_to_num = [](const auto& r) {return strtoll(string_view(r).data(), nullptr, 10);};

	vector<vector<int64_t>> v;

	string line;
	while (getline(input, line)) {
		v.emplace_back(line | views::split(' ') | views::transform(subrange_to_num) | ranges::to<vector>());
	}
	return v;
}


static auto CreateHistory(span<const int64_t> values) {
	constexpr auto diff = [](const auto& p) {return p.back() - p.front(); };

	vector<vector<int64_t>> diffs{ranges::to<vector>(values)};

	while (!ranges::all_of(diffs.back(), bind_front(equal_to(), 0LL))) {
		diffs.emplace_back(diffs.back() | views::slide(2) | views::transform(diff) | ranges::to<vector>());
	}
	return diffs;
}


template<typename Func>
int64_t Predict(const Func& predict_func) {
	const auto data = ParseValues();

	int64_t sum = 0;
	for (const auto& values : data) {
		const auto history = CreateHistory(values);
		sum += ranges::fold_right(history, 0, predict_func);
	}
	return sum;
}


export void day9_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto sum = Predict([](const auto& vals, auto prediction) {return vals.back() + prediction;});

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 9a: {} ({})", sum, duration);
}


export void day9_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto sum = Predict([](const auto& vals, auto prediction) {return vals.front() - prediction;});

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 9b: {} ({})", sum, duration);
}
