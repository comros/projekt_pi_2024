#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

class Tile {
public:
    enum class TileType {
        DeepWater,
        ShallowWater,
        NormalSand,
        NormalGrass,
        Mountain,
        Default
    };

    bool isLowerLayerThan(TileType otherType) const {
        // Define hierarchy for layering (lowest to highest)
        static const std::vector<TileType> hierarchy = {
            TileType::DeepWater,
            TileType::ShallowWater,
            TileType::NormalSand,
            TileType::NormalGrass,
            TileType::Mountain
        };

        auto currentPos = std::find(hierarchy.begin(), hierarchy.end(), mType);
        auto otherPos = std::find(hierarchy.begin(), hierarchy.end(), otherType);

        return currentPos < otherPos; // Compare based on hierarchy
    }

    // Get bounding rectangle for sprite
    sf::FloatRect getBounds() const {
        return mSprite.getGlobalBounds();
    }

private:
    sf::Sprite mSprite;
    TileType mType;
    sf::Texture mTextureAtlas;
    sf::Texture mDeepWaterVarietyAtlas;
    sf::Texture mShallowWaterVarietyAtlas;
    sf::Texture mSandVarietyAtlas;
    sf::Texture mGrassVarietyAtlas;
    sf::Texture mMountainVarietyAtlas;

    float brightness = 1.1f;

public:
    // Constructor
    Tile(sf::Texture& texture, const sf::Vector2f& position)
        : mType(TileType::Default) {
        mSprite.setTexture(texture);
        mSprite.setPosition(position);

        // Default tile (12th tile in 4x4 grid) at (0, 48) with 16x16 size
        // mSprite.setTextureRect(sf::IntRect(0, 48, 16, 16));
        mSprite.setTextureRect(sf::IntRect(32, 16, 16, 16));
        texture.setSmooth(false);
    }

    // Set the tile type and color the sprite
    void setType(TileType type) {
        mType = type;

        // Set color based on tile type
        switch (type) {
        case TileType::DeepWater:
            mSprite.setColor(adjustColorIntensity(sf::Color(0, 62, 178), brightness));  // Make grass color 20% brighter
            break;
        case TileType::ShallowWater:
            mSprite.setColor(adjustColorIntensity(sf::Color(9, 82, 198), brightness));  // Make grass color 20% brighter
            break;
        case TileType::NormalSand:
            mSprite.setColor(adjustColorIntensity(sf::Color(164, 148, 99), brightness));  // Make grass color 20% brighter
            break;
        case TileType::NormalGrass:
            mSprite.setColor(adjustColorIntensity(sf::Color(60, 97, 20), brightness));  // Make grass color 20% brighter
            break;
        case TileType::Mountain:
            mSprite.setColor(adjustColorIntensity(sf::Color(160, 162, 143), brightness));  // Make grass color 20% brighter
            break;
        case TileType::Default:
        default:
            mSprite.setColor(sf::Color::White); // Default tile color
            break;
        }
    }

    // Randomize the texture within the given grid (ncols x nrows)
    void randomizeTexture(int ncols, int nrows) {
        // Generate random indices for selecting tile from the atlas
        int randomCol = rand() % ncols;
        int randomRow = rand() % nrows;

        // Set the texture rect to randomly pick a tile from the atlas
        mSprite.setTextureRect(sf::IntRect(randomCol * 16, randomRow * 16, 16, 16));
    }

    // Get the current tile type
    TileType getType() const {
        return mType;
    }

    // Set position of the tile
    void setPosition(const sf::Vector2f& position) {
        mSprite.setPosition(position);
    }

    // Get the sprite for rendering
    const sf::Sprite& getSprite() const {
        return mSprite;
    }

    // Adjust texture rect based on bitmask
    void setTextureByBitmask(int bitmask) {
        int tileSize = 16;
        int atlasWidth = 4; // 4 tiles per row

        int row = bitmask / atlasWidth;
        int col = bitmask % atlasWidth;

        mSprite.setTextureRect(sf::IntRect(col * tileSize, row * tileSize, tileSize, tileSize));
    }

    sf::Color adjustColorIntensity(const sf::Color& originalColor, float multiplier) {
        return sf::Color(
            std::min(static_cast<int>(originalColor.r * multiplier), 255),
            std::min(static_cast<int>(originalColor.g * multiplier), 255),
            std::min(static_cast<int>(originalColor.b * multiplier), 255)
        );
    }
};

#endif // TILE_HPP
