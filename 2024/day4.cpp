export module day4;

import std;

using namespace std;


static vector<string> file_input() {
	ifstream input("day4.txt");
	vector<string> letters(1);
	while (getline(input, letters.back())) {
		letters.emplace_back();
	}
	letters.pop_back();
	return letters;
}


static int IsItXmas(const vector<string>& letters, size_t s_y, size_t s_x) {
	static constexpr array<array<int, 2>, 8> directions {{{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}}};
	static constexpr string_view word = "XMAS";

	int count = 0;
	for (const auto& dir : directions) {
		size_t pos = 0;
		size_t y = s_y;
		size_t x = s_x;
		while (y < letters.size() && x < letters[0].size() && pos < word.size()) {
			if (letters[y][x] != word[pos]) break;

			y += dir[0];
			x += dir[1];
			if (++pos == word.size()) {
				count++;
			}
		}
	}
	return count;
}

export void day4_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto letters = file_input();

	int count = 0;
	for (size_t y = 0; y < letters.size(); ++y) {
		for (size_t x = 0; x < letters[0].size(); ++x) {
			count += IsItXmas(letters, y, x);
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 4a: {} ({})", count, duration);
}


static int IsItCrossMas(const vector<string>& letters, size_t y, size_t x) {
	static constexpr array<array<int, 2>, 8> directions {{{-1,-1}, {-1,1}, {1,-1}, {1,1}}};

	if (letters[y][x] != 'A') return 0;

	int count = 0;
	for (const auto& dir : directions) {
		if (letters[y + dir[0]][x + dir[1]] == 'M' &&
			letters[y - dir[0]][x - dir[1]] == 'S') {
			++count;
		}
	}
	return count == 2 ? 1 : 0;
}


export void day4_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto letters = file_input();

	int count = 0;
	for (size_t y = 1; y + 1 < letters.size(); ++y) {
		for (size_t x = 1; x + 1 < letters[0].size(); ++x) {
			count += IsItCrossMas(letters, y, x);
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 4b: {} ({})", count, duration);
}
