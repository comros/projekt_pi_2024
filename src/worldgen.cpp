#include "../headers/WorldGen.hpp"

// Constructor
WorldGen::WorldGen(unsigned int width, unsigned int height, unsigned int terrainSeed)
:   mWidth(width),
    mHeight(height),
    mTerrainSeed(terrainSeed),
    mTerrainNoise(terrainSeed)  // Initialize Perlin noise generator for terrain
{
    // Ensure terrain and moisture values are initialized properly (default to 0.0f)
    mTerrainValues.resize(height, std::vector<float>(width, 0.0f));
    mMoistureValues.resize(height, std::vector<float>(width, 0.0f));

    if (!mTextureAtlas.loadFromFile("../../assets/terrain/terrain_bitmasked3.png")) {
        // Handle error
    }
    mTextureAtlas.setSmooth(false);


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

// Generates the map by populating terrain and moisture values for each tile
void WorldGen::generateMap() {
    // Clear existing data
    mTerrainValues.clear();
    mMoistureValues.clear();
    mTiles.clear();
    mTextureAtlas.setSmooth(false);

    // Reinitialize noise objects with new seeds
    mTerrainNoise = PerlinNoise(mTerrainSeed);

    // Resize value arrays and tile grid to fit the map dimensions
    mTerrainValues.resize(mHeight, std::vector<float>(mWidth, 0.0f));
    mMoistureValues.resize(mHeight, std::vector<float>(mWidth, 0.0f));
    mTiles.resize(mHeight, std::vector<Tile>(mWidth, Tile(mTextureAtlas, sf::Vector2f(1.0f, 1.0f))));

    // Recompute falloff and noise patterns
    precomputeFalloff(mWidth, mHeight);
    precomputeNoise(mWidth, mHeight, 16);

    // Generate tiles
    for (unsigned int y = 0; y < mHeight; ++y) {
        for (unsigned int x = 0; x < mWidth; ++x) {
            float terrainValue = mTerrainValues[y][x];;

            // Determine the tile type based on terrain and moisture values
            Tile::TileType tileType;
            if (terrainValue < thresholdDeepWater) {
                tileType = Tile::TileType::DeepWater;
            } else if (terrainValue < thresholdShallowWater) {
                tileType = Tile::TileType::ShallowWater;
            } else if (terrainValue < thresholdSand) {
                    tileType = Tile::TileType::NormalSand;
            } else if (terrainValue < thresholdGrass) {
                    tileType = Tile::TileType::NormalGrass;
            } else {
                    tileType = Tile::TileType::Mountain;
            }

            // Use this texture in your Tile objects
            mTiles[y][x] = Tile(mTextureAtlas, sf::Vector2f(x * 16.0f, y * 16.0f));
            mTiles[y][x].setType(tileType);
        }
    }

    // Apply auto-tiling
    for (unsigned int y = 0; y < mHeight; ++y) {
        for (unsigned int x = 0; x < mWidth; ++x) {
            int bitmask = calculateBitmask(x, y, mTiles[y][x].getType());
            mTiles[y][x].setTextureByBitmask(bitmask);
        }
    }
}

int WorldGen::calculateBitmask(int x, int y, Tile::TileType type) const {
    int bitmask = 0;

    // Check neighbors (Top, Right, Bottom, Left)
    if (y > 0 && mTiles[y - 1][x].getType() == type) bitmask |= 1 << 0; // Top
    if (x < mWidth - 1 && mTiles[y][x + 1].getType() == type) bitmask |= 1 << 1; // Right
    if (y < mHeight - 1 && mTiles[y + 1][x].getType() == type) bitmask |= 1 << 2; // Bottom
    if (x > 0 && mTiles[y][x - 1].getType() == type) bitmask |= 1 << 3; // Left

    // Check for neighboring tiles of a higher priority
    static const std::vector<Tile::TileType> priorityOrder = {
        Tile::TileType::DeepWater,
        Tile::TileType::ShallowWater,
        Tile::TileType::NormalSand,
        Tile::TileType::NormalGrass,
        Tile::TileType::Mountain
    };

    auto currentPriority = std::find(priorityOrder.begin(), priorityOrder.end(), type);

    // Iterate over neighbors to find higher-priority tiles
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = x + dx;
            int ny = y + dy;

            if ((dx != 0 || dy != 0) && nx >= 0 && nx < (int)mWidth && ny >= 0 && ny < (int)mHeight) {
                auto neighborType = mTiles[ny][nx].getType();
                auto neighborPriority = std::find(priorityOrder.begin(), priorityOrder.end(), neighborType);

                // If a neighbor is of higher priority, force the bitmask to 15
                if (neighborPriority > currentPriority) {
                    return 15; // Force to "full connection"
                }
            }
        }
    }

    return bitmask;
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
                }
            }
        }
    }
}

// Renders only the visible tiles within the current view of the window
void WorldGen::render(sf::RenderWindow& window) {
    // Get the view's visible bounds
    sf::View view = window.getView();
    sf::FloatRect viewBounds(
        view.getCenter() - view.getSize() / 2.0f,
        view.getSize()
    );

    // Calculate the visible grid bounds
    int startX = std::max(0, static_cast<int>(viewBounds.left / 16));
    int startY = std::max(0, static_cast<int>(viewBounds.top / 16));
    int endX = std::min(static_cast<int>(mWidth), static_cast<int>((viewBounds.left + viewBounds.width) / 16) + 1);
    int endY = std::min(static_cast<int>(mHeight), static_cast<int>((viewBounds.top + viewBounds.height) / 16) + 1);

    // Render only the visible tiles
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            Tile& currentTile = mTiles[y][x];

            // Calculate the bitmask for the current tile
            int bitmask = 0;

            // Check top neighbor
            if (y > 0 && mTiles[y - 1][x].getType() == currentTile.getType()) {
                bitmask |= 1; // Top
            }

            // Check right neighbor
            if (x < static_cast<int>(mWidth) - 1 && mTiles[y][x + 1].getType() == currentTile.getType()) {
                bitmask |= 2; // Right
            }

            // Check bottom neighbor
            if (y < static_cast<int>(mHeight) - 1 && mTiles[y + 1][x].getType() == currentTile.getType()) {
                bitmask |= 4; // Bottom
            }

            // Check left neighbor
            if (x > 0 && mTiles[y][x - 1].getType() == currentTile.getType()) {
                bitmask |= 8; // Left
            }

            // Render an underlying tile if the bitmask indicates neighbors are different
            if (bitmask != 15) { // Not all neighbors are the same
                for (int i = 0; i < 4; ++i) {
                    int dx = (i == 1) - (i == 3); // Right (+1) or Left (-1)
                    int dy = (i == 2) - (i == 0); // Bottom (+1) or Top (-1)

                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < static_cast<int>(mWidth) && ny >= 0 && ny < static_cast<int>(mHeight)) {
                        Tile& neighborTile = mTiles[ny][nx];

                        if (neighborTile.getType() != currentTile.getType() &&
                            neighborTile.isLowerLayerThan(currentTile.getType())) {
                            sf::Sprite underlyingSprite = neighborTile.getSprite();
                            underlyingSprite.setPosition(currentTile.getSprite().getPosition());
                            window.draw(underlyingSprite);
                        }
                    }
                }
            }

            // Render the current tile
            window.draw(currentTile.getSprite());
        }
    }
}


// Alternative simple rendering (less efficient but simpler) - renders all tiles at once
// void WorldGen::render(sf::RenderWindow& window) { for (const auto& tile : mTiles) window.draw(tile); }
