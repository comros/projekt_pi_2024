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

    float brightness = 1.1f;

public:
    // Constructor
    Tile(sf::Texture& texture, const sf::Vector2f& position)
        : mType(TileType::Default) {
        mTextureAtlas.setSmooth(false);
        mSprite.setTexture(texture);
        mSprite.setPosition(position);

        mSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
        texture.setSmooth(false);
    }

    // Set the tile type and color the sprite
    void setType(TileType type) {
        mType = type;

        // Set color based on tile type
        switch (type) {
        case TileType::DeepWater:
            mSprite.setColor(adjustColorIntensity(sf::Color(33, 115, 184), brightness));  // Make grass color 20% brighter
            break;
        case TileType::ShallowWater:
            mSprite.setColor(adjustColorIntensity(sf::Color(83, 141, 189), brightness));  // Make grass color 20% brighter
            break;
        case TileType::NormalSand:
            mSprite.setColor(adjustColorIntensity(sf::Color(181, 160, 115), brightness));  // Make grass color 20% brighter
            break;
        case TileType::NormalGrass:
            mSprite.setColor(adjustColorIntensity(sf::Color(97, 120, 66), brightness));  // Make grass color 20% brighter
            break;
        case TileType::Mountain:
            mSprite.setColor(adjustColorIntensity(sf::Color(255,255,228), brightness));  // Make grass color 20% brighter
            break;
        case TileType::Default:
        default:
            mSprite.setColor(sf::Color::White); // Default tile color
            break;
        }
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
        const int typeIndex = static_cast<int>(mType); // Convert TileType to index (row offset)

        int col, row;

        if (bitmask == 15) {
            // Randomly select a row (0 to 3) in the fifth column (index 4)
            col = 4; // Fifth column
            row = rand() % 4; // Random row (0, 1, 2, 3)
        } else {
            // Calculate the row and column in the grid based on the bitmask
            row = bitmask / atlasWidth;
            col = bitmask % atlasWidth;
        }

        // Offset the row based on the tile type
        int textureY = (typeIndex * atlasWidth + row) * tileSize;
        int textureX = col * tileSize;

        // Set the texture rect to the appropriate tile
        mSprite.setTextureRect(sf::IntRect(textureX, textureY, tileSize, tileSize));
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
