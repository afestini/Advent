export module day24;

import std;

using namespace std;


namespace {
	template<typename T>
	struct Vec3 {
		T x, y, z;

		auto operator<=>(const Vec3&) const = default;
		Vec3 operator-() const { return {-x, -y, -z}; }
		Vec3 operator+(Vec3 v) const { return {x + v.x, y + v.y, z + v.z}; }
		Vec3 operator-(Vec3 v) const { return {x - v.x, y - v.y, z - v.z}; }
		Vec3 operator/(Vec3 v) const { return {x / v.x, y / v.y, z / v.z}; }
		T Dot(Vec3 v) const { return x * v.x + y * v.y + z * v.z; }
		T LenSq() const { return Dot(*this); }
		T Len() const { return sqrt(LenSq()); }
		Vec3 Norm() const { const auto len = Len(); return {x / len, y / len, z / len}; }
		Vec3 Cross(Vec3 v) const { return {y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y}; }
	};

	template<typename T>
	Vec3<T> operator*(T s, Vec3<T> v) { return {s * v.x, s * v.y, s * v.z}; }

	struct Hail { Vec3<double> pos, vel; };
}

static vector<Hail> ReadInput() {
	ifstream input("day24.txt");

	vector<Hail> hailstones;

	string line;
	while (getline(input, line)) {
		auto& hail = hailstones.emplace_back();
		array coords = {&hail.pos.x, &hail.pos.y, &hail.pos.z, &hail.vel.x, &hail.vel.y, &hail.vel.z};

		for (int idx = 0; const auto corner : views::split(line, '@')) {
			for (const auto coord : views::split(corner, ',')) {
				ispanstream(coord) >> *coords[idx++];
			}
		}
	}
	return hailstones;
}


export void day24_1() {
	const auto start_time = chrono::high_resolution_clock::now();

	const auto hailstones = ReadInput();
	const double a_min = 200000000000000;
	const double a_max = 400000000000000;

	int count = 0;
	for (auto [i, a] : hailstones | views::enumerate) {
		for (const auto& b : ranges::subrange(hailstones.begin() + i + 1, hailstones.end())) {
			const auto delta_p = b.pos - a.pos;

			const auto cross = a.vel.x * b.vel.y - a.vel.y * b.vel.x;
			if (cross == 0) continue; // Parallel

			const auto t = (delta_p.x * b.vel.y - delta_p.y * b.vel.x) / cross;
			if (t < 0) continue; // Past of a

			const auto s = (delta_p.x * a.vel.y - delta_p.y * a.vel.x) / cross;
			if (s < 0) continue; // Past of b

			const auto is = a.pos + t * a.vel;
			if (is.x >= a_min && is.y >= a_min && is.x <= a_max && is.y <= a_max)
				++count;
		}
	}

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start_time);
	println("Day 24a: {} ({})", count, duration);
}


export void day24_2() {
	const auto start_time = chrono::high_resolution_clock::now();

	const auto hailstones = ReadInput();

	Vec3<double> point_on_plane;
	Vec3<double> normal;

	for (auto [i, a] : hailstones | views::enumerate) {
		for (const auto& b : ranges::subrange(hailstones.begin() + i + 1, hailstones.end())) {
			if (a.vel.Cross(b.vel).Dot(b.pos - a.pos) == 0) {
				println("Coplanar");

				if (a.vel.Cross(b.vel).LenSq() == 0) println("Parallel");

				point_on_plane = a.pos;
				normal = a.vel.Cross(b.pos - a.pos).Norm();
				println("Normal: {} / {} / {}", normal.x, normal.y, normal.z);
			}
		}
	}

	struct Intersection {
		Vec3<double> pos;
		double t;
	};

	vector<Intersection> intersections;

	for (const auto& stone : hailstones) {
		const auto dist = (point_on_plane - stone.pos).Dot(normal);
		const auto t = dist / stone.vel.Dot(normal);

		if (isnan(t)) continue; // Skip the co-planar lines

		intersections.emplace_back(stone.pos + t * stone.vel, t);
	}

	ranges::sort(intersections, {}, &Intersection::t);

	const auto velocity = 1.0 / (intersections[1].t - intersections[0].t) * (intersections[1].pos - intersections[0].pos);
	const auto position = intersections[0].pos - intersections[0].t * velocity;

	const auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start_time);
	println("Day 24b: {} ({})", position.x + position.y + position.z, duration);
}
