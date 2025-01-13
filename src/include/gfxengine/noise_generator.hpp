#pragma once

// Perlin noise generator
class NoiseGenerator
{
private:

	unsigned char p[512];

public:

	NoiseGenerator(long seed = 0);

	[[nodiscard]]
	double noise(double x) const;

	[[nodiscard]]
	double noise(double x, double y) const;

	[[nodiscard]]
	double noise(double x, double y, double z) const;
};
