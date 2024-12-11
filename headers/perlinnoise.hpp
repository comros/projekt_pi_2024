#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <vector>
#include <cmath>
#include <numeric>

class PerlinNoise {
public:
    explicit PerlinNoise(unsigned int seed = 0);

    float noise(float x, float y, float z = 0.0f) const;

private:
    std::vector<int> p; // Permutation vector
    float fade(float t) const;
    float lerp(float t, float a, float b) const;
    float grad(int hash, float x, float y, float z) const;
};

#endif // PERLIN_NOISE_HPP