export module utils;

import std;

using namespace std;


export template<typename T>
struct Vec2 {
	friend Vec2 operator+(Vec2 a, Vec2 b) { return Vec2 {.x = a.x + b.x, .y = a.y + b.y}; }
	Vec2& operator+=(Vec2 v) { x += v.x; y += v.y; return *this; }
	friend Vec2 operator-(Vec2 a, Vec2 b) { return Vec2 {.x = a.x - b.x, .y = a.y - b.y}; }
	Vec2& operator-=(Vec2 v) { x -= v.x; y -= v.y; return *this; }

	auto operator<=>(const Vec2&) const = default;

	T x {};
	T y {};
};


export using Vec2i = Vec2<int>;


export template<typename T> requires(sizeof(T) <= 4)
struct Vec2Hash {
	size_t operator()(Vec2<T> a) const { return static_cast<size_t>(a.x) << 32 | a.y; };
};


export struct Map2D {
	using Pos = Vec2i;

	Map2D() = default;
	explicit Map2D(const string& path) { Load(path); }

	void Load(const string& path) {
		ifstream input(path);
		grid.emplace_back();
		while (getline(input, grid.back())) {
			grid.emplace_back();
		}
		grid.pop_back();
	}

	char& operator[](Vec2i pos) {
		return grid[pos.y][pos.x];
	}

	const char& operator[](Vec2i pos) const {
		return grid[pos.y][pos.x];
	}

	// Looks weird, but one less check
	bool is_in_bounds(Vec2i pos) const {
		return static_cast<size_t>(pos.x) < Width()
			&& static_cast<size_t>(pos.y) < Height();
	}

	size_t Width() const { return grid[0].size(); }
	size_t Height() const { return grid.size(); }

	vector<string> grid;
};
