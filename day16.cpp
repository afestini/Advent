export module day16;

import std;

using namespace std;


static vector<string> ReadInput() {
	ifstream input("day16.txt");

	vector<string> data(1);
	while (getline(input, data.back())) {
		data.emplace_back();
	}
	data.pop_back();
	return data;
}


struct Vec {
	int8_t x; int8_t y;

	auto operator<=>(const Vec& a) const = default;
	Vec& operator+=(const Vec& v) { x += v.x; y += v.y; return *this; }
	Vec operator+(const Vec& v) const { return {x + v.x, y + v.y}; }
	Vec operator-(const Vec& v) const { return {x - v.x, y - v.y}; }
	Vec operator-() const { return {-x, -y}; }
};

struct Ray {
	Vec pos{0, 0};
	Vec dir{1, 0};
};

static int64_t LightItUp(const vector<string>& input, Ray ray) {
	const auto height = static_cast<int>(input.size());
	const auto width = height;

	vector<uint8_t> tile_visited(width * height);

	int64_t count = 0;

	queue<Ray> rays;
	rays.emplace(move(ray));

	while (!rays.empty()) {
		Ray r = rays.front();
		rays.pop();

		for (;;) {
			// Ray went off the map, stop
			if (r.pos.x < 0 || r.pos.x >= width || r.pos.y < 0 || r.pos.y >= height) break;

			// Ray entered a cycle or is going in reverse, stop
			const auto idx = r.pos.y * width + r.pos.x;
			const auto mirror = input[r.pos.y][r.pos.x];

			if (tile_visited[idx] && (mirror == '|' || mirror == '-'))
				break;

			count += !tile_visited[idx];
			tile_visited[idx] = true;

			if ((mirror == '\\' && r.dir.x) || (mirror == '/' && r.dir.y)) {
				r.dir = {-r.dir.y, r.dir.x}; // Rotate right
			}
			else if ((mirror == '/' && r.dir.x) || (mirror == '\\' && r.dir.y)) {
				r.dir = {r.dir.y, -r.dir.x}; // Rotate left
			}
			else if ((mirror == '|' && r.dir.x) || (mirror == '-' && r.dir.y)) {
				swap(r.dir.x, r.dir.y); // Split
				rays.emplace(r.pos - r.dir, -r.dir);
			}

			r.pos += r.dir;
		}
	}

	return count;
}


export void day16_1() {
	const auto start = chrono::high_resolution_clock::now();

	auto input = ReadInput();
	const auto sum = LightItUp(input, {Vec{0, 0}, Vec{1, 0}});

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 16a: {} ({})", sum, duration);
}


export void day16_2() {
	const auto start = chrono::high_resolution_clock::now();

	auto input = ReadInput();

	const auto height = static_cast<int8_t>(input.size());
	const auto width = height;

	vector<Ray> check;
	check.reserve(2 * height + 2 * width);

	for (int8_t i = 0; i < width; ++i) {
		check.emplace_back(Vec{i, 0}, Vec{0, 1});
		check.emplace_back(Vec{i, height - 1}, Vec{0, -1});
	}
	for (int8_t i = 0; i < height; ++i) {
		check.emplace_back(Vec{0, i}, Vec{1, 0});
		check.emplace_back(Vec{width - 1, i}, Vec{-1, 0});
	}

	mutex max_mutex;
	int64_t max_lit = 0;
	for_each(execution::par_unseq, check.begin(), check.end(), [&input, &max_lit, &max_mutex](const auto& ray){
		const auto sum = LightItUp(input, ray);
		lock_guard l(max_mutex);
		max_lit = max(max_lit, sum);
	});

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
	println("Day 16b: {} ({})", max_lit, duration);
}
