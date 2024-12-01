export module day18;

import std;

using namespace std;


namespace {
	struct Vec { int64_t x; int64_t y; };
	Vec operator+(Vec a, Vec b) { return {a.x + b.x, a.y + b.y}; }
	Vec operator*(int scalar, const Vec& v) { return {scalar * v.x, scalar * v.y}; }
}


static Vec ToVec(char dir) {
	switch (dir) { // No overlap, so we use the same for both versions
	case 0: return {1, 0};
	case 1: return {0, 1};
	case 2: return {-1, 0};
	case 3: return {0, -1};
	case 'D': return {0, 1};
	case 'L': return {-1, 0};
	case 'R': return {1, 0};
	case 'U': return {0, -1};
	default: return {0, 0};
	}
}


static vector<Vec> ReadInput(bool fix_plan) {
	ifstream input("day18.txt");

	char dir;
	int dist;
	string color;
	vector<Vec> plan{{0, 0}};

	while ((input >> dir >> dist).ignore(3) >> color) {
		if (fix_plan) {
			dir = color[5] - '0';
			from_chars(color.data(), color.data() + 5, dist, 16);
		}
		plan.emplace_back(plan.back() + (dist * ToVec(dir)));
	}
	return plan;
}


static int64_t ShoeLace(span<const Vec> plan) {
	const auto pairs = plan | views::slide(2);
	const auto area = reduce(pairs.begin(), pairs.end(), 0LL, [](auto area, const auto& v) {
		area += (v[0].x * v[1].y) - (v[1].x * v[0].y); // Area (shoe lace)
		return area + abs((v[1].x - v[0].x) + (v[1].y - v[0].y)); // Perimeter
	});
	return (area / 2) + 1;
}


export void day18_1() {
	const auto start = chrono::high_resolution_clock::now();

	const auto plan = ReadInput(false);
	const auto sum = ShoeLace(plan);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 18a: {} ({})", sum, duration);
}


export void day18_2() {
	const auto start = chrono::high_resolution_clock::now();

	const auto plan = ReadInput(true);
	const auto sum = ShoeLace(plan);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 18b: {} ({})", sum, duration);
}
