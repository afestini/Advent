export module utils;

import std;

using namespace std;


#ifdef _WIN32
static constexpr size_t newline_len = 2;
#else
static constexpr size_t newline_len = 1;
#endif


template<typename T>
concept arithmetic = is_arithmetic_v<T>;


export template<arithmetic T>
struct Vec2 {
	friend Vec2 operator+(Vec2 a, Vec2 b) { return Vec2 {.x = a.x + b.x, .y = a.y + b.y}; }
	Vec2& operator+=(Vec2 v) { x += v.x; y += v.y; return *this; }

	friend Vec2 operator-(Vec2 a, Vec2 b) { return Vec2 {.x = a.x - b.x, .y = a.y - b.y}; }
	Vec2& operator-=(Vec2 v) { x -= v.x; y -= v.y; return *this; }

	friend Vec2 operator*(Vec2 a, Vec2 b) { return Vec2 {.x = a.x * b.x, .y = a.y * b.y}; }
	Vec2& operator*=(Vec2 v) { x *= v.x; y *= v.y; return *this; }

	template<arithmetic Scalar>
	friend Vec2 operator*(Scalar s, Vec2 v) { return Vec2 {.x = s * v.x, .y = s * v.y}; }

	template<arithmetic Scalar>
	friend Vec2 operator*(Vec2 v, Scalar s) { return Vec2 {.x = s * v.x, .y = s * v.y}; }

	template<arithmetic Scalar>
	friend Vec2 operator/(Vec2 v, Scalar s) { return Vec2 {.x = v.x / s, .y = v.y / s}; }

	template<arithmetic Scalar>
	friend Vec2 operator/=(Vec2 v, Scalar s) { v.x /= s; v.y /= s; return * this; }

	template<arithmetic Scalar>
	friend Vec2 operator%(Vec2 v, Scalar s) { return Vec2 {.x = v.x % s, .y = v.y % s}; }

	template<arithmetic Scalar>
	friend Vec2 operator%=(Vec2 v, Scalar s) { v.x %= s; v.y %= s; return *this; }

	friend Vec2 operator%(Vec2 a, Vec2 b) { return Vec2 {.x = a.x % b.x, .y = a.y % b.y}; }
	Vec2 operator%=(Vec2 b) { x %= b.x; y %= b.y; return *this; }

	Vec2 operator-() const { return {-x, -y}; }

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
	Map2D(size_t width, size_t height) : grid(width* height), stride(width), width(width), height(height) {}
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

	void print() const {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				std::print("{}", (*this)[Vec2i(x, y)]);
			}
			println();
		}
	}

	size_t Width() const { return width; }
	size_t Height() const { return height; }

	vector<char> grid;
	size_t stride = 0;
	size_t width = 0;
	size_t height = 0;
};
