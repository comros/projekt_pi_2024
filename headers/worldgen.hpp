#ifndef WORLDGEN_H
#define WORLDGEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "PerlinNoise.hpp"
#include "../headers/tile.hpp"
#include "definitions.hpp"
#include <random>

class WorldGen {
public:
    float falloffStrength = 4.0f;
    int octaves = 10;
    float frequency = 0.01f;

    float thresholdDeepWater = 0.2f;
    float thresholdShallowWater = 0.4f;
    float thresholdSand = 0.48f;
    float thresholdGrass = 0.67f;

    int mTerrainSeed;

    WorldGen(unsigned int width, unsigned int height, unsigned int terrainSeed);

    // Main map generation method
    void generateMap();
    int calculateBitmask(int x, int y, Tile::TileType type) const;

    // Tile rendering method
    void render(sf::RenderWindow& window);

private:
    // Dimensions of the world map
    unsigned int mWidth, mHeight;

    // Noise generators for terrain and moisture
    PerlinNoise mTerrainNoise;

    // Vectors to store terrain and moisture values
    std::vector<std::vector<float>> mTerrainValues;
    std::vector<std::vector<float>> mMoistureValues;
    std::vector<std::vector<float>> mFalloffValues;

    // Tiles to render
    std::vector<std::vector<Tile>> mTiles;
    sf::Texture mTextureAtlas;

    // Noise and falloff generation functions
    float generateFractalNoise(int x, int y, const PerlinNoise& noise) const;
    void precomputeFalloff(int mapWidth, int mapHeight);
    void precomputeNoise(int mapWidth, int mapHeight, int tileSize);
};

#endif // WORLDGEN_H
