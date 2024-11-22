#include "../headers/WorldGen.hpp"

// Constructor
WorldGen::WorldGen(unsigned int width, unsigned int height, unsigned int terrainSeed, unsigned int moistureSeed)
:   mWidth(width),
    mHeight(height),
    mTerrainSeed(terrainSeed),
    mMoistureSeed(moistureSeed),
    mTerrainNoise(terrainSeed),  // Initialize Perlin noise generator for terrain
    mMoistureNoise(moistureSeed) // Initialize Perlin noise generator for moisture
{
    // Ensure terrain and moisture values are initialized properly (default to 0.0f)
    mTerrainValues.resize(height, std::vector<float>(width, 0.0f));
    mMoistureValues.resize(height, std::vector<float>(width, 0.0f));

    // Precompute falloff (e.g., distance from center) for map
    precomputeFalloff(mWidth, mHeight);

    // Precompute noise patterns for terrain and moisture generation
    precomputeNoise(mWidth, mHeight, 16);  // Adjust tile size (16) for noise granularity
    generateMap();  // Generate the map with terrain and moisture data
}

// Generates fractal noise (multi-octave) for terrain or moisture values
float WorldGen::generateFractalNoise(int x, int y, const PerlinNoise& noise) const {
    float value = 0.0f;
    float amplitude = 1.0f;
    float frequencyFactor = frequency; // Frequency controls the level of detail in the map

    // Iteratively add octaves of noise for more detail (5 octaves is typical)
    for (int i = 0; i < octaves; ++i) {
        value += noise.noise(x * frequencyFactor, y * frequencyFactor) * amplitude;
        frequencyFactor *= 2.0f; // Increase frequency for finer details
        amplitude *= 0.5f;       // Reduce amplitude for diminishing noise contribution
    }

    return value;
}

// Returns the color of a tile based on its terrain and moisture values
sf::Color WorldGen::getTileColor(float terrainValue, float moistureValue) const {
    // Define color thresholds for different terrain types based on noise values
    if (terrainValue < thresholdDeepWater) {
        return sf::Color(0, 62, 178); // Deep water
    } else if (terrainValue < thresholdShallowWater) {
        return sf::Color(9, 82, 198); // Shallow water
    } else if (terrainValue < thresholdSand) {
        // Sand type based on moisture value
        if (moistureValue < -0.2f)
            return sf::Color(134, 118, 69); // Dry sand
        else if (moistureValue < 0.2f)
            return sf::Color(164, 148, 99); // Normal sand
        else
            return sf::Color(194, 178, 129); // Wet sand
    } else if (terrainValue < thresholdGrass) {
        // Grass type based on moisture value
        if (moistureValue < -0.2f)
            return sf::Color(40, 77, 0); // Dry grass
        else if (moistureValue < 0.2f)
            return sf::Color(60, 97, 20); // Normal grass
        else
            return sf::Color(90, 127, 50); // Lush grass
    } else {
        // Mountain types based on moisture value
        if (moistureValue < -0.2f)
            return sf::Color(139, 141, 123); // Rocky mountain
        else if (moistureValue < 0.2f)
            return sf::Color(160, 162, 143); // Mountain
        else
            return sf::Color(235, 235, 235); // Snowy mountain
    }
}

// Generates the map by populating terrain and moisture values for each tile
void WorldGen::generateMap() {
    // Clear existing data
    mTerrainValues.clear();
    mMoistureValues.clear();
    mTiles.clear();

    // Reinitialize noise objects with new seeds
    mTerrainNoise = PerlinNoise(mTerrainSeed);
    mMoistureNoise = PerlinNoise(mMoistureSeed);

    // Resize value arrays to fit the map dimensions
    mTerrainValues.resize(mHeight, std::vector<float>(mWidth, 0.0f));
    mMoistureValues.resize(mHeight, std::vector<float>(mWidth, 0.0f));
    mFalloffValues.resize(mHeight, std::vector<float>(mWidth, 0.0f));

    // Recompute falloff and noise patterns based on new seeds
    precomputeFalloff(mWidth, mHeight);
    precomputeNoise(mWidth, mHeight, 16);

    // Generate tiles based on updated terrain and moisture values
    for (unsigned int y = 0; y < mHeight; ++y) {
        for (unsigned int x = 0; x < mWidth; ++x) {
            float terrainValue = mTerrainValues[y][x];
            float moistureValue = mMoistureValues[y][x];

            // Get the color based on terrain and moisture values
            sf::Color tileColor = getTileColor(terrainValue, moistureValue);

            // Create the tile as a rectangle
            sf::RectangleShape tile(sf::Vector2f(16.0f, 16.0f));
            tile.setPosition(x * 16.0f, y * 16.0f);  // Position based on grid coordinates
            tile.setFillColor(tileColor);  // Set the color of the tile

            // Add the tile to the list of all tiles (for later rendering)
            mTiles.push_back(tile);
        }
    }
}

// Precomputes a falloff effect based on the distance from the map's center
void WorldGen::precomputeFalloff(int mapWidth, int mapHeight) {
    mFalloffValues.clear();
    mFalloffValues.resize(mapHeight, std::vector<float>(mapWidth));

    // Iterate over every tile and compute a falloff value based on its distance to the center
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            // Normalize coordinates to range [-1, 1]
            float nx = 2.0f * (x / static_cast<float>(mapWidth)) - 1.0f;
            float ny = 2.0f * (y / static_cast<float>(mapHeight)) - 1.0f;

            // Calculate the distance from the center and apply the falloff strength
            float distance = std::sqrt(nx * nx + ny * ny);
            mFalloffValues[y][x] = std::pow(distance, falloffStrength);  // Falloff effect
        }
    }
}

// Precomputes the noise for terrain and moisture generation in chunks
void WorldGen::precomputeNoise(int mapWidth, int mapHeight, int tileSize) {
    int chunkSize = 64;  // Process in chunks to reduce memory pressure
    for (int y = 0; y < mapHeight; y += chunkSize) {
        for (int x = 0; x < mapWidth; x += chunkSize) {
            // Iterate through each tile in the chunk
            for (int dy = 0; dy < chunkSize && (y + dy) < mapHeight; ++dy) {
                for (int dx = 0; dx < chunkSize && (x + dx) < mapWidth; ++dx) {
                    int tileX = x + dx;
                    int tileY = y + dy;

                    // Generate terrain and moisture values using fractal noise
                    float terrainValue = generateFractalNoise(tileX, tileY, mTerrainNoise);
                    terrainValue = (terrainValue + 1.0f) / 2.0f;  // Normalize to 0-1 range
                    mTerrainValues[tileY][tileX] = terrainValue - mFalloffValues[tileY][tileX]; // Apply falloff to terrain

                    float moistureValue = generateFractalNoise(tileX, tileY, mMoistureNoise);
                    mMoistureValues[tileY][tileX] = moistureValue;  // Set moisture value
                }
            }
        }
    }
}

// Renders the tiles that are visible within the current view of the window
void WorldGen::render(sf::RenderWindow& window) {
    // Get the view's visible bounds (the area of the map that is currently visible)
    sf::View view = window.getView();
    sf::FloatRect viewBounds(
        view.getCenter() - view.getSize() / 2.0f,  // Calculate the top-left corner of the view
        view.getSize()  // Get the size of the view
    );

    // Calculate the visible grid bounds based on the current view
    int startX = std::max(0, static_cast<int>(viewBounds.left / 16));  // Tile size is 16x16
    int startY = std::max(0, static_cast<int>(viewBounds.top / 16));
    int endX = std::min(static_cast<int>(mWidth), static_cast<int>((viewBounds.left + viewBounds.width) / 16) + 1);
    int endY = std::min(static_cast<int>(mHeight), static_cast<int>((viewBounds.top + viewBounds.height) / 16) + 1);

    // Render only the visible tiles within the calculated bounds
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            float terrainValue = mTerrainValues[y][x];
            float moistureValue = mMoistureValues[y][x];

            // Get the color for the current tile
            sf::Color tileColor = getTileColor(terrainValue, moistureValue);

            // Create a rectangle for the tile
            sf::RectangleShape tile(sf::Vector2f(16.0f, 16.0f));
            tile.setPosition(x * 16.0f, y * 16.0f);  // Position based on grid coordinates
            tile.setFillColor(tileColor);  // Set the color of the tile

            // Draw the tile on the window
            window.draw(tile);
        }
    }
}

// Alternative simple rendering (less efficient but simpler) - renders all tiles at once
// void WorldGen::render(sf::RenderWindow& window) { for (const auto& tile : mTiles) window.draw(tile); }
