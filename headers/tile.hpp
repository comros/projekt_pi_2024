#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Tile {
public:
    enum class TileType {
        DeepWater,
        ShallowWater,
        Sand,
        Grass,
        Rock,
        Default
    };

    bool isLowerLayerThan(TileType otherType) const {
        // Define hierarchy for layering (lowest to highest)
        static const std::vector<TileType> hierarchy = {
            TileType::DeepWater,
            TileType::ShallowWater,
            TileType::Sand,
            TileType::Grass,
            TileType::Rock
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

    float mBrightness = 1.1f;

public:
    // Constructor
    Tile(sf::Texture& texture, const sf::Vector2f& position)
        : mType(TileType::Default) {
        srand(time(NULL));
        mTextureAtlas.setSmooth(false);
        mSprite.setTexture(texture);
        mSprite.setPosition(position);

        mSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
        texture.setSmooth(false);
    }

    float getBrightness() const {return mBrightness;}
    void setBrightness(float newBrightness)
    {
        mBrightness = newBrightness; // Update the brightness value

        // Reapply the current type's color with the new brightness
        setType(mType); // Reapply color and brightness
    }

    TileType getType() const { return mType; }    // Get the current tile type
    void setType(TileType type) {                 // Set the tile type and color the sprite
        mType = type;

        // Set color based on tile type
        switch (type) {
        case TileType::DeepWater:
            mSprite.setColor(adjustColorIntensity(sf::Color(33, 115, 184), mBrightness));  // Make grass color 20% brighter
            break;
        case TileType::ShallowWater:
            mSprite.setColor(adjustColorIntensity(sf::Color(83, 141, 189), mBrightness));  // Make grass color 20% brighter
            break;
        case TileType::Sand:
            mSprite.setColor(adjustColorIntensity(sf::Color(181, 160, 115), mBrightness));  // Make grass color 20% brighter
            break;
        case TileType::Grass:
            mSprite.setColor(adjustColorIntensity(sf::Color(97, 120, 66), mBrightness));  // Make grass color 20% brighter
            break;
        case TileType::Rock:
            mSprite.setColor(adjustColorIntensity(sf::Color(255,255,228), mBrightness));  // Make grass color 20% brighter
            break;
        case TileType::Default:
        default:
            mSprite.setColor(sf::Color::White); // Default tile color
            break;
        }
    }

    // Set position of the tile
    sf::Vector2f getPosition() const { return mSprite.getPosition(); }
    void setPosition(const sf::Vector2f& position) { mSprite.setPosition(position); }

    // Get the sprite for rendering
    const sf::Sprite& getSprite() const {
        return mSprite;
    }

    // If needed, you can implement a method to print tile info
    void printInfo() const {
        std::cout << "Tile Position: (" << mSprite.getPosition().x / 16<< ", " << mSprite.getPosition().y / 16<< ")\n";
    }

    // Adjust texture rect based on bitmask
    void setTextureByBitmask(int bitmask) {
        int tileSize = 16;
        int atlasWidth = 4; // 4 tiles per row (excluding the column with random variants)
        const int typeIndex = static_cast<int>(mType); // Convert TileType to index (row offset)

        // Lookup table mapping bitmask values to tile IDs
        // Those correspond to tileAtlas id's, the array position is bitmask value
        static const int bitmaskToTileID[16] = {12,8,13,9,0,4,1,5,15,11,14,10,3,7,2,6};

        // Get the tile ID from the lookup table
        int tileID = bitmaskToTileID[bitmask];

        int col, row;

        // IMPORTANT, in the random column add 1 pixel length stripe (padding) to prevent texture bleeding
        if (bitmask == 15) {
            // Randomly select a row (0 to 3) in the fifth column (index 4)
            col = 5; // Sixth column
            row = rand() % 4; // Random row (0, 1, 2, 3)
        } else {
            // Calculate the row and column in the grid based on the bitmask
            row = tileID / atlasWidth;
            col = tileID % atlasWidth;
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
