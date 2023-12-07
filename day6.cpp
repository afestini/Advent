export module day6;

import std;

using namespace std;


struct Race { int time; int dist; };

vector<Race> ParseRaces() {
	ifstream input("day6.txt");
	if (!input) return {};

	string time_str;
	getline(input, time_str);
	spanstream times(time_str);
	times.ignore(99, ':');

	string dist_str;
	getline(input, dist_str);
	spanstream dists(dist_str);
	dists.ignore(99, ':');

	vector<Race> races;
	while (times && dists) {
		auto& race = races.emplace_back();
		times >> race.time;
		dists >> race.dist;
	}
	races.pop_back();
	return races;
}


// Boils down to a simplified quadratic equation with a = 1, b = time, c = distance
pair<int, int> SolveQuadratic(uint64_t time, uint64_t dist) {
	double sqr = sqrt((time * time) - 4 * dist);
	double x0 = (time - sqr) / 2;
	double x1 = (time + sqr) / 2;
	if (sqr > time) {
		println("Oh, oh");
		x0 = x1;
	}
	
	// To beat the time, hold longer than the minimum and shorter than the maxium (at full milliseconds)
	return {static_cast<int>(floor(x0 + 1.0f)), static_cast<int>(ceil(x1 - 1.0f))};
}
 

export void day6_1() {
	const auto races = ParseRaces();

	int product = 1;
	for (const auto& race : races) {
		const auto [from, to] = SolveQuadratic(race.time, race.dist);
		product *= to - from + 1;
	}

	println("Day 6a: {}", product);
}


export void day6_2() {
	const auto races = ParseRaces();

	// Very lazy for the sake of reuse and shortness, lots of silly back and forth conversion
	string time;
	string dist;
	for (const auto& race : races) {
		time += to_string(race.time);
		dist += to_string(race.dist);
	}

	const auto [from, to] = SolveQuadratic(stoull(time), stoull(dist));
	println("Day 6b: {}", to - from + 1);
}
