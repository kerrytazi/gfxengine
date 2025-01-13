#include "gfxengine/noise_generator.hpp"

// Modified version of
// https://github.com/daniilsjb/perlin-noise/tree/cabc932d4083c6fbb07565eeba86718fa7f72981

namespace db {
	// Permutation table.
	static constexpr unsigned char default_p[256] = {
		0x97, 0xA0, 0x89, 0x5B, 0x5A, 0x0F, 0x83, 0x0D, 0xC9, 0x5F, 0x60, 0x35, 0xC2, 0xE9, 0x07, 0xE1,
		0x8C, 0x24, 0x67, 0x1E, 0x45, 0x8E, 0x08, 0x63, 0x25, 0xF0, 0x15, 0x0A, 0x17, 0xBE, 0x06, 0x94,
		0xF7, 0x78, 0xEA, 0x4B, 0x00, 0x1A, 0xC5, 0x3E, 0x5E, 0xFC, 0xDB, 0xCB, 0x75, 0x23, 0x0B, 0x20,
		0x39, 0xB1, 0x21, 0x58, 0xED, 0x95, 0x38, 0x57, 0xAE, 0x14, 0x7D, 0x88, 0xAB, 0xA8, 0x44, 0xAF,
		0x4A, 0xA5, 0x47, 0x86, 0x8B, 0x30, 0x1B, 0xA6, 0x4D, 0x92, 0x9E, 0xE7, 0x53, 0x6F, 0xE5, 0x7A,
		0x3C, 0xD3, 0x85, 0xE6, 0xDC, 0x69, 0x5C, 0x29, 0x37, 0x2E, 0xF5, 0x28, 0xF4, 0x66, 0x8F, 0x36,
		0x41, 0x19, 0x3F, 0xA1, 0x01, 0xD8, 0x50, 0x49, 0xD1, 0x4C, 0x84, 0xBB, 0xD0, 0x59, 0x12, 0xA9,
		0xC8, 0xC4, 0x87, 0x82, 0x74, 0xBC, 0x9F, 0x56, 0xA4, 0x64, 0x6D, 0xC6, 0xAD, 0xBA, 0x03, 0x40,
		0x34, 0xD9, 0xE2, 0xFA, 0x7C, 0x7B, 0x05, 0xCA, 0x26, 0x93, 0x76, 0x7E, 0xFF, 0x52, 0x55, 0xD4,
		0xCF, 0xCE, 0x3B, 0xE3, 0x2F, 0x10, 0x3A, 0x11, 0xB6, 0xBD, 0x1C, 0x2A, 0xDF, 0xB7, 0xAA, 0xD5,
		0x77, 0xF8, 0x98, 0x02, 0x2C, 0x9A, 0xA3, 0x46, 0xDD, 0x99, 0x65, 0x9B, 0xA7, 0x2B, 0xAC, 0x09,
		0x81, 0x16, 0x27, 0xFD, 0x13, 0x62, 0x6C, 0x6E, 0x4F, 0x71, 0xE0, 0xE8, 0xB2, 0xB9, 0x70, 0x68,
		0xDA, 0xF6, 0x61, 0xE4, 0xFB, 0x22, 0xF2, 0xC1, 0xEE, 0xD2, 0x90, 0x0C, 0xBF, 0xB3, 0xA2, 0xF1,
		0x51, 0x33, 0x91, 0xEB, 0xF9, 0x0E, 0xEF, 0x6B, 0x31, 0xC0, 0xD6, 0x1F, 0xB5, 0xC7, 0x6A, 0x9D,
		0xB8, 0x54, 0xCC, 0xB0, 0x73, 0x79, 0x32, 0x2D, 0x7F, 0x04, 0x96, 0xFE, 0x8A, 0xEC, 0xCD, 0x5D,
		0xDE, 0x72, 0x43, 0x1D, 0x18, 0x48, 0xF3, 0x8D, 0x80, 0xC3, 0x4E, 0x42, 0xD7, 0x3D, 0x9C, 0xB4,
	};

	template<typename T>
	static constexpr auto lerp(T a, T b, T t) -> T {
		return a + t * (b - a);
	}

	template<typename T>
	static constexpr auto floor(T x) -> int {
		auto const xi = int(x);
		return (x < T(xi)) ? xi - 1 : xi;
	}

	template<typename T>
	static constexpr auto fade(T t) -> T {
		return t * t * t * (t * (t * T(6.0) - T(15.0)) + T(10.0));
	}

	template<typename T>
	static constexpr auto dot_grad(unsigned char hash, T xf) -> T {
		// In 1D case, the gradient may be either 1 or -1.
		// The distance vector is the input offset (relative to the smallest bound).
		return (hash & 0x1) ? xf : -xf;
	}

	template<typename T>
	static constexpr auto dot_grad(unsigned char hash, T xf, T yf) -> T {
		// In 2D case, the gradient may be any of 8 direction vectors pointing to the
		// edges of a unit-square. The distance vector is the input offset (relative to
		// the smallest bound).
		switch (hash & 0x7) {
			case 0x0: return  xf + yf;
			case 0x1: return  xf;
			case 0x2: return  xf - yf;
			case 0x3: return -yf;
			case 0x4: return -xf - yf;
			case 0x5: return -xf;
			case 0x6: return -xf + yf;
			case 0x7: return  yf;
			default: return {};
		}
	}

	template<typename T>
	static constexpr auto dot_grad2(unsigned char hash, T xf, T yf) -> T {
		// In 2D case, the gradient may be any of 8 direction vectors pointing to the
		// edges of a unit-square. The distance vector is the input offset (relative to
		// the smallest bound).
		switch (hash & 0xF) {
			case 0x0: return  xf          + yf;
			case 0x1: return  xf          + yf * T(0.5);
			case 0x2: return  xf;
			case 0x3: return  xf          - yf * T(0.5);
			case 0x4: return  xf          - yf;
			case 0x5: return  xf * T(0.5) - yf;
			case 0x6: return -yf;
			case 0x7: return -xf * T(0.5) - yf;
			case 0x8: return -xf          - yf;
			case 0x9: return -xf          - yf * T(0.5);
			case 0xA: return -xf;
			case 0xB: return -xf          + yf * T(0.5);
			case 0xC: return -xf          + yf;
			case 0xD: return -xf * T(0.5) + yf;
			case 0xE: return  yf;
			case 0xF: return  xf * T(0.5) + yf;
			default: return {};
		}
	}

	template<typename T>
	static constexpr auto dot_grad3(unsigned char hash, T xf, T yf) -> T {
		// In 2D case, the gradient may be any of 8 direction vectors pointing to the
		// edges of a unit-square. The distance vector is the input offset (relative to
		// the smallest bound).
		switch (hash & 0xF) {
			case 0x0: return  xf * T(0.7) + yf * T(0.7);
			case 0x1: return  xf          + yf * T(0.5);
			case 0x2: return  xf;
			case 0x3: return  xf          - yf * T(0.5);
			case 0x4: return  xf * T(0.7) - yf * T(0.7);
			case 0x5: return  xf * T(0.5) - yf;
			case 0x6: return -yf;
			case 0x7: return -xf * T(0.5) - yf;
			case 0x8: return -xf * T(0.7) - yf * T(0.7);
			case 0x9: return -xf          - yf * T(0.5);
			case 0xA: return -xf;
			case 0xB: return -xf          + yf * T(0.5);
			case 0xC: return -xf * T(0.7) + yf * T(0.7);
			case 0xD: return -xf * T(0.5) + yf;
			case 0xE: return  yf;
			case 0xF: return  xf * T(0.5) + yf;
			default: return {};
		}
	}

	template<typename T>
	static constexpr auto dot_grad(unsigned char hash, T xf, T yf, T zf) -> T {
		// In 3D case, the gradient may be any of 12 direction vectors pointing to the edges
		// of a unit-cube (rounded to 16 with duplications). The distance vector is the input
		// offset (relative to the smallest bound).
		switch (hash & 0xF) {
			case 0x0: return  xf + yf;
			case 0x1: return -xf + yf;
			case 0x2: return  xf - yf;
			case 0x3: return -xf - yf;
			case 0x4: return  xf + zf;
			case 0x5: return -xf + zf;
			case 0x6: return  xf - zf;
			case 0x7: return -xf - zf;
			case 0x8: return  yf + zf;
			case 0x9: return -yf + zf;
			case 0xA: return  yf - zf;
			case 0xB: return -yf - zf;
			case 0xC: return  yf + xf;
			case 0xD: return -yf + zf;
			case 0xE: return  yf - xf;
			case 0xF: return -yf - zf;
			default: return {};
		}
	}

	template<typename T>
	constexpr auto perlin(unsigned char const p[512], T x) -> T {
		// Left coordinate of the unit-line that contains the input.
		int const xi = floor(x);

		// Input location in the unit-line.
		T const xf0 = x - T(xi);
		T const xf1 = xf0 - T(1.0);

		// Wrap to range 0-255.
		int const xi0 = (xi + 0) & 0xFF;
		int const xi1 = (xi + 1) & 0xFF;

		// Apply the fade function to the location.
		T const u = fade(xf0);

		// Generate hash values for each point of the unit-line.
		int const h0 = p[xi0];
		int const h1 = p[xi1];

		// Linearly interpolate between dot products of each gradient with its distance to the input location.
		return lerp(dot_grad(h0, xf0), dot_grad(h1, xf1), u);
	}

	template<typename T>
	constexpr auto perlin(unsigned char const p[512], T x, T y) -> T {
		// Top-left coordinates of the unit-square.
		int const xi = floor(x);
		int const yi = floor(y);

		// Input location in the unit-square.
		T const xf0 = x - T(xi);
		T const yf0 = y - T(yi);
		T const xf1 = xf0 - T(1.0);
		T const yf1 = yf0 - T(1.0);

		// Wrap to range 0-255.
		int const xi0 = (xi + 0) & 0xFF;
		int const xi1 = (xi + 1) & 0xFF;
		int const yi0 = (yi + 0) & 0xFF;
		int const yi1 = (yi + 1) & 0xFF;

		// Apply the fade function to the location.
		T const u = fade(xf0);
		T const v = fade(yf0);

		// Generate hash values for each point of the unit-square.
		int const h00 = p[p[xi0] + yi0];
		int const h01 = p[p[xi0] + yi1];
		int const h10 = p[p[xi1] + yi0];
		int const h11 = p[p[xi1] + yi1];

		// Linearly interpolate between dot products of each gradient with its distance to the input location.
		T const x1 = lerp(dot_grad(h00, xf0, yf0), dot_grad(h10, xf1, yf0), u);
		T const x2 = lerp(dot_grad(h01, xf0, yf1), dot_grad(h11, xf1, yf1), u);
		return lerp(x1, x2, v);
	}

	template<typename T>
	constexpr auto perlin(unsigned char const p[512], T x, T y, T z) -> T {
		// Top-left coordinates of the unit-cube.
		int const xi = floor(x);
		int const yi = floor(y);
		int const zi = floor(z);

		// Input location in the unit-cube.
		T const xf0 = x - T(xi);
		T const yf0 = y - T(yi);
		T const zf0 = z - T(zi);
		T const xf1 = xf0 - T(1.0);
		T const yf1 = yf0 - T(1.0);
		T const zf1 = zf0 - T(1.0);

		// Wrap to range 0-255.
		int const xi0 = (xi + 0) & 0xFF;
		int const xi1 = (xi + 1) & 0xFF;
		int const yi0 = (yi + 0) & 0xFF;
		int const yi1 = (yi + 1) & 0xFF;
		int const zi0 = (zi + 0) & 0xFF;
		int const zi1 = (zi + 1) & 0xFF;

		// Apply the fade function to the location.
		T const u = fade(xf0);
		T const v = fade(yf0);
		T const w = fade(zf0);

		// Generate hash values for each point of the unit-cube.
		int const h000 = p[p[p[xi0] + yi0] + zi0];
		int const h001 = p[p[p[xi0] + yi0] + zi1];
		int const h010 = p[p[p[xi0] + yi1] + zi0];
		int const h011 = p[p[p[xi0] + yi1] + zi1];
		int const h100 = p[p[p[xi1] + yi0] + zi0];
		int const h101 = p[p[p[xi1] + yi0] + zi1];
		int const h110 = p[p[p[xi1] + yi1] + zi0];
		int const h111 = p[p[p[xi1] + yi1] + zi1];

		// Linearly interpolate between dot products of each gradient with its distance to the input location.
		T const x11 = lerp(dot_grad(h000, xf0, yf0, zf0), dot_grad(h100, xf1, yf0, zf0), u);
		T const x12 = lerp(dot_grad(h010, xf0, yf1, zf0), dot_grad(h110, xf1, yf1, zf0), u);
		T const x21 = lerp(dot_grad(h001, xf0, yf0, zf1), dot_grad(h101, xf1, yf0, zf1), u);
		T const x22 = lerp(dot_grad(h011, xf0, yf1, zf1), dot_grad(h111, xf1, yf1, zf1), u);

		T const y1 = lerp(x11, x12, v);
		T const y2 = lerp(x21, x22, v);

		return lerp(y1, y2, w);
	}
}

template <typename T>
static constexpr void swap(T &p1, T &p2)
{
	T tmp = p1;
	p1 = p2;
	p2 = tmp;
}

NoiseGenerator::NoiseGenerator(long seed /* = 0 */)
{
	for (int j = 0; j < 256; ++j)
		p[j] = db::default_p[j];

	unsigned char *s = (unsigned char *)&seed;

	for (int i = 0; i < sizeof(seed); ++i)
	{
		for (int j = 0; j < 1024; ++j)
		{
			swap(p[(s[i] + j) & 0xFF], p[p[(i + j) & 0xFF]]);
		}
	}

	for (int j = 0; j < 256; ++j)
		p[j + 256] = p[j];
}

[[nodiscard]]
double NoiseGenerator::noise(double x) const
{
	return db::perlin(p, x);
}

[[nodiscard]]
double NoiseGenerator::noise(double x, double y) const
{
	return db::perlin(p, x, y);
}

[[nodiscard]]
double NoiseGenerator::noise(double x, double y, double z) const
{
	return db::perlin(p, x, y, z);
}
