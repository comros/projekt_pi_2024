#include "../headers/WorldGen.hpp"

// Constructor
WorldGen::WorldGen(unsigned int width, unsigned int height, unsigned int terrainSeed, unsigned int moistureSeed)
:   mWidth(width),
    mHeight(height),
    mTerrainSeed(terrainSeed),
    mMoistureSeed(moistureSeed),
    mTerrainNoise(terrainSeed),
    mMoistureNoise(moistureSeed)
{

    // Ensure terrain and moisture values are initialized properly
    mTerrainValues.resize(height, std::vector<float>(width, 0.0f));
    mMoistureValues.resize(height, std::vector<float>(width, 0.0f));

    precomputeFalloff(mWidth, mHeight);
    precomputeNoise(mWidth, mHeight, 16);  // Adjust tile size as needed
    generateMap();
}

float WorldGen::generateFractalNoise(int x, int y, const PerlinNoise& noise) const {
    float value = 0.0f;
    float amplitude = 1.0f;
    float frequencyFactor = frequency; // Can be adjusted for map detail

    for (int i = 0; i < octaves; ++i) {  // Using 5 octaves for better details
        value += noise.noise(x * frequencyFactor, y * frequencyFactor) * amplitude;
        frequencyFactor *= 2.0f; // Increase frequency for finer details
        amplitude *= 0.5f;       // Reduce amplitude for diminishing contributions
    }

    return value;
}

// Updated getTileColor with your thresholds
sf::Color WorldGen::getTileColor(float terrainValue, float moistureValue) const {
    if (terrainValue < thresholdDeepWater) {
        return sf::Color(0, 62, 178); // Deep water
    } else if (terrainValue < thresholdShallowWater) {
        return sf::Color(9, 82, 198); // Shallow water
    } else if (terrainValue < thresholdSand) {
        if (moistureValue < -0.2f)
            return sf::Color(134, 118, 69); // Dry sand
        else if (moistureValue < 0.2f)
            return sf::Color(164, 148, 99); // Normal sand
        else
            return sf::Color(194, 178, 129); // Wet sand
    } else if (terrainValue < thresholdGrass) {
        if (moistureValue < -0.2f)
            return sf::Color(40, 77, 0); // Dry grass
        else if (moistureValue < 0.2f)
            return sf::Color(60, 97, 20); // Normal grass
        else
            return sf::Color(90, 127, 50); // Lush grass
    } else {
        if (moistureValue < -0.2f)
            return sf::Color(139, 141, 123); // Rocky mountain
        else if (moistureValue < 0.2f)
            return sf::Color(160, 162, 143); // Mountain
        else
            return sf::Color(235, 235, 235); // Snowy mountain
    }
}


void WorldGen::generateMap() {
    // Clear existing data
    mTerrainValues.clear();
    mMoistureValues.clear();
    mTiles.clear();

    // Reinitialize noise objects based on new seeds
    mTerrainNoise = PerlinNoise(mTerrainSeed);  // Recreate noise object with the new seed
    mMoistureNoise = PerlinNoise(mMoistureSeed); // Recreate noise object with the new seed

    mTerrainValues.resize(mHeight, std::vector<float>(mWidth, 0.0f));
    mMoistureValues.resize(mHeight, std::vector<float>(mWidth, 0.0f));
    mFalloffValues.resize(mHeight, std::vector<float>(mWidth, 0.0f));

    // Recompute falloff and noise with new seeds and parameters
    precomputeFalloff(mWidth, mHeight);
    precomputeNoise(mWidth, mHeight, 16);  // Adjust tile size as needed

    // Create tiles based on updated terrain and moisture values
    for (unsigned int y = 0; y < mHeight; ++y) {
        for (unsigned int x = 0; x < mWidth; ++x) {
            float terrainValue = mTerrainValues[y][x];
            float moistureValue = mMoistureValues[y][x];

            sf::Color tileColor = getTileColor(terrainValue, moistureValue);

            sf::RectangleShape tile(sf::Vector2f(16.0f, 16.0f));
            tile.setPosition(x * 16.0f, y * 16.0f);
            tile.setFillColor(tileColor);
            mTiles.push_back(tile);
        }
    }
}


void WorldGen::precomputeFalloff(int mapWidth, int mapHeight) {
    mFalloffValues.clear();
    mFalloffValues.resize(mapHeight, std::vector<float>(mapWidth));

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            float nx = 2.0f * (x / static_cast<float>(mapWidth)) - 1.0f;
            float ny = 2.0f * (y / static_cast<float>(mapHeight)) - 1.0f;
            float distance = std::sqrt(nx * nx + ny * ny);
            mFalloffValues[y][x] = std::pow(distance, falloffStrength);  // Apply falloff strength
        }
    }
}

void WorldGen::precomputeNoise(int mapWidth, int mapHeight, int tileSize) {
    int chunkSize = 64; // Calculate tiles in chunks
    for (int y = 0; y < mapHeight; y += chunkSize) {
        for (int x = 0; x < mapWidth; x += chunkSize) {
            for (int dy = 0; dy < chunkSize && (y + dy) < mapHeight; ++dy) {
                for (int dx = 0; dx < chunkSize && (x + dx) < mapWidth; ++dx) {
                    int tileX = x + dx;
                    int tileY = y + dy;

                    // Terrain generation using fractal noise
                    float terrainValue = generateFractalNoise(tileX, tileY, mTerrainNoise);
                    terrainValue = (terrainValue + 1.0f) / 2.0f;  // Normalize to 0-1
                    mTerrainValues[tileY][tileX] = terrainValue - mFalloffValues[tileY][tileX]; // Apply falloff to terrain

                    // Moisture generation using fractal noise
                    float moistureValue = generateFractalNoise(tileX, tileY, mMoistureNoise);
                    mMoistureValues[tileY][tileX] = moistureValue;
                }
            }
        }
    }
}

void WorldGen::render(sf::RenderWindow& window) {
    for (const auto& tile : mTiles) {
        window.draw(tile);
    }
}
