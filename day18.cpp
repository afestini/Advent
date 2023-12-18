export module day18;

import std;

using namespace std;


namespace {
	struct Vec {
		int64_t x; int64_t y;

		Vec& operator+=(const Vec& v) { x += v.x; y += v.y; return *this; }
	};
	Vec operator*(int scalar, const Vec& v) { return {scalar * v.x, scalar * v.y}; }


	struct DigLine {
		Vec from;
		Vec to;
		char dir;
	};


	struct DigPlan {
		Vec min{0, 0};
		Vec max{0, 0};
		vector<DigLine> lines;
	};
}


static Vec ToVec(char dir) {
	switch (dir) {
	case 'D': return {0, 1};
	case 'L': return {-1, 0};
	case 'R': return {1, 0};
	case 'U': return {0, -1};
	default: return {0, 0};
	}
}

static Vec IntToVec(int dir) {
	switch (dir) {
	case 0: return {1, 0};
	case 1: return {0, 1};
	case 2: return {-1, 0};
	case 3: return {0, -1};
	default: return {0, 0};
	}
}


static DigPlan ReadInput(bool fix_plan) {
	ifstream input("day18.txt");

	DigPlan plan;
	Vec pos{0, 0};

	string line;
	char dir;
	int dist;
	string color;

	while (getline(input, line)) {
		ispanstream str(line);

		str >> dir >> dist;
		str.ignore(3) >> color;

		Vec tmp = pos;

		if (fix_plan) {
			dir = color[5];
			from_chars(color.data(), color.data() + 5, dist, 16);
			pos += dist * IntToVec(color[5] - '0');
		}
		else {
			pos += dist * ToVec(dir);
		}

		auto& l = plan.lines.emplace_back(tmp, pos, dir);

		// Make all lines left to right and top to bottom
		if (pos.x < tmp.x || pos.y < tmp.y) swap(l.from, l.to);

		plan.min.x = min(plan.min.x, pos.x);
		plan.min.y = min(plan.min.y, pos.y);
		plan.max.x = max(plan.max.x, pos.x);
		plan.max.y = max(plan.max.y, pos.y);
	}

	// Sort all lines left to right
	ranges::sort(plan.lines, {}, [](const auto& l) {
		return min(l.from.x, l.to.x) + (l.from.x != l.to.x ? 1 : 0);
	});

	return plan;
}


static int64_t ScanLine(const DigPlan& plan, int64_t y) {
	int64_t sum = 0;
	bool inside = false;
	char last_y_dir = 0;
	int64_t x = plan.min.x;

	for (const auto& l : plan.lines) {
		if (l.from.x != l.to.x) { // Horizontal line
			if (l.from.y == y) {
				sum += inside * (l.from.x + 1 - x);
				sum += l.to.x - l.from.x - 1;
				x = l.to.x;
			}
		}
		else if (y >= l.from.y && y <= l.to.y) {
			sum += inside * (l.from.x - x);
			++sum;
			x = l.from.x + 1;

			inside ^= (last_y_dir != l.dir);
			last_y_dir = l.dir;
		}
	}
	return sum;
}


static int64_t Dump(const DigPlan& plan) {
	atomic<int64_t> sum{};
	auto lines = views::iota(plan.min.y, plan.max.y - plan.min.y + 1);
	for_each(execution::par_unseq, lines.begin(), lines.end(), [&plan, &sum](auto y) {
		const auto count = ScanLine(plan, y);
		sum += count;
	});
	return sum;
}


export void day18_1() {
	const auto start = chrono::high_resolution_clock::now();

	auto plan = ReadInput(false);
	const auto sum = Dump(plan);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 18a: {} ({})", sum, duration);
}


export void day18_2() {
	const auto start = chrono::high_resolution_clock::now();

	auto plan = ReadInput(true);
	const auto sum = Dump(plan);

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 18b: {} ({})", sum, duration);
}
