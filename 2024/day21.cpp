export module day21;

import std;
import utils;

using namespace std;
using namespace std::chrono;


static auto file_input() {
	ifstream input("day21.txt");

	vector<string> codes;
	while (getline(input, codes.emplace_back()));
	codes.pop_back();

	return codes;
}


static unordered_map<char, Vec2i> keypad_pos = {
	{'7', {0,3}}, {'8', {1,3}}, {'9', {2,3}},
	{'4', {0,2}}, {'5', {1,2}}, {'6', {2,2}},
	{'1', {0,1}}, {'2', {1,1}}, {'3', {2,1}},
	              {'0', {1,0}}, {'A', {2,0}}
};

static unordered_map<char, Vec2i> dpad_pos = {
	               {'^', {1, 0}}, {'A', {2, 0}},
	{'<', {0,-1}}, {'v', {1,-1}}, {'>', {2,-1}}
};

static unordered_map<char, Vec2i> dpad_step = {
	{'^', {0,1}}, {'<', {-1,0}}, {'v',{0,-1}}, {'>',{1,0}}
};


static bool moves_valid(Vec2i from, string_view moves) {
	const auto hits_gap = [pos = from](auto step) mutable {
		return (pos += dpad_step[step]) == Vec2i {};
	};
	return ranges::none_of(moves, hits_gap);
}


static vector<string> dpad_permutations(Vec2i from, Vec2i to) {
	const auto move = to - from;
	string moves = string()
	     .append(max(0, -move.x), '<')
	     .append(max(0,  move.x), '>')
	     .append(max(0,  move.y), '^')
	     .append(max(0, -move.y), 'v');

	vector<string> permutations;
	do {
		if (moves_valid(from, moves))
			permutations.emplace_back(moves).push_back('A');
	} while (ranges::next_permutation(moves).found);
	return permutations;
}


static map<tuple<Vec2i, Vec2i, int>, size_t> cost_cache;

static size_t get_cost(Vec2i from, Vec2i to, int depth);


static size_t get_length(string_view sequence, auto& pad_pos, int depth) {
	size_t len = 0;
	for (Vec2i pos = pad_pos['A']; char next : sequence) {
		const auto next_pos = pad_pos[next];
		len += get_cost(pos, next_pos, depth);
		pos = next_pos;
	}
	return len;
}


static size_t get_cost(Vec2i from, Vec2i to, int depth) {
	auto [it, added] = cost_cache.try_emplace({from, to, depth});
	if (!added) return it->second;

	const auto options = dpad_permutations(from, to);

	if (depth == 0)
		return it->second = ranges::min(options, {}, &string::size).size();

	const auto lengths = views::transform(options, [depth](string_view s) {return get_length(s, dpad_pos, depth - 1);});
	return it->second = ranges::min(lengths);
}


static size_t code_complexities(const vector<string>& codes, int chain_length) {
	return accumulate(codes.begin(), codes.end(), 0ULL, [chain_length](size_t sum, string_view code){
		uint64_t num;
		from_chars(code.data(), code.data() + 3, num);

		return sum + num * get_length(code, keypad_pos, chain_length);
	});
}


export void day21_1() {
	const auto start_time = high_resolution_clock::now();

	const auto total_complexity = code_complexities(file_input(), 2);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 21a: {} ({})", total_complexity, duration);
}


export void day21_2() {
	const auto start_time = high_resolution_clock::now();

	const auto total_complexity = code_complexities(file_input(), 25);

	const auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start_time);
	println("Day 21b: {} ({})", total_complexity, duration);
}
