export module utils;

import std;

using namespace std;


#ifdef _WIN32
static constexpr size_t newline_len = 2;
#else
static constexpr size_t newline_len = 1;
#endif


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
		ifstream input(path, ios::binary);

		grid.resize(filesystem::file_size(path));
		input.read(grid.data(), grid.size());

		const bool has_final_newline = grid.back() == '\n' || grid.back() == '\r';
		const auto correction = has_final_newline ? 0 : newline_len;

		width = distance(grid.begin(), ranges::find_first_of(grid, array{'\n', '\r'}));
		stride = width + newline_len;
		height = (grid.size() + correction) / stride;
	}

	char& operator[](Vec2i pos) {
		return grid[pos.y * stride + pos.x];
	}

	const char& operator[](Vec2i pos) const {
		return grid[pos.y * stride + pos.x];
	}

	// Looks weird, but one less check
	bool is_in_bounds(Vec2i pos) const {
		return static_cast<size_t>(pos.x) < width
			&& static_cast<size_t>(pos.y) < height;
	}

	size_t Width() const { return width; }
	size_t Height() const { return height; }

	//vector<string> grid;
	vector<char> grid;
	size_t stride = 0;
	size_t width = 0;
	size_t height = 0;
};
