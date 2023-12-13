export module day2;

import std;

using namespace std;


struct CubeSet { int r; int g; int b; };

struct Game {
	int id = -1;
	vector<CubeSet> sets;
};


static Game parse_game(string_view line) {
	auto game_sets = views::split(line, ':') | ranges::to<vector>();

	Game game;
	ispanstream stream{string_view(game_sets[0])};
	stream.ignore(99, ' ') >> game.id;

	for (const auto& set_range : views::split(game_sets[1], ';')) {
		auto& set = game.sets.emplace_back();

		for (const auto& color : views::split(string_view(set_range), ',')) {
			ispanstream stream{string_view(color)};
			int count;
			string color;
			stream >> count >> color;

			if (color == "red") set.r = count;
			else if (color == "green") set.g = count;
			else if (color == "blue") set.b = count;
		}
	}

	return game;
}


bool set_is_possible(const CubeSet& to_check) {
	static constexpr CubeSet available{.r = 12, .g = 13, .b = 14};
	return to_check.r <= available.r &&
		to_check.g <= available.g &&
		to_check.b <= available.b;
}


vector<Game> parse_games() {
	ifstream input("day2.txt");
	if (!input) return {};

	vector<Game> games;

	string line;
	while (getline(input, line)) {
		games.emplace_back(parse_game(line));
	}

	return games;
}


export void day2_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto games = parse_games();

	int sum = 0;
	for (const auto& game : games) {
		if (ranges::all_of(game.sets, set_is_possible))
			sum += game.id;
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 2a: {} ({})", sum, duration);
}




export void day2_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto games = parse_games();

	int sum = 0;
	for (const auto& game : games) {
		CubeSet maxes{};
		for (const auto& set : game.sets) {
			maxes.r = max(maxes.r, set.r);
			maxes.g = max(maxes.g, set.g);
			maxes.b = max(maxes.b, set.b);
		}
		sum += maxes.r * maxes.g * maxes.b;
	}
	
	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 2b: {} ({})", sum, duration);
}
