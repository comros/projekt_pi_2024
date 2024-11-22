#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

class Tile {
public:
    enum class TileType {
        DeepWater,
        ShallowWater,
        DrySand,
        NormalSand,
        WetSand,
        DryGrass,
        NormalGrass,
        LushGrass,
        RockyMountain,
        Mountain,
        SnowyMountain,
        Default
    };

private:
    sf::Sprite mSprite;
    TileType mType;
    sf::Texture mTextureAtlas;

    float brightness = 1.35f;
    float moistBrightness = 1.2f;

public:
    // Constructor
    Tile(sf::Texture& texture, const sf::Vector2f& position)
        : mType(TileType::Default) {
        mSprite.setTexture(texture);
        mSprite.setPosition(position);

        // Default tile (12th tile in 4x4 grid) at (0, 48) with 16x16 size
        // mSprite.setTextureRect(sf::IntRect(0, 48, 16, 16));
        mSprite.setTextureRect(sf::IntRect(32, 16, 16, 16));
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
        case TileType::DrySand:
            mSprite.setColor(adjustColorIntensity(sf::Color(134, 118, 69), brightness));  // Make grass color 20% brighter

            break;
        case TileType::NormalSand:
            mSprite.setColor(adjustColorIntensity(sf::Color(164, 148, 99), brightness));  // Make grass color 20% brighter

            break;
        case TileType::WetSand:
            mSprite.setColor(adjustColorIntensity(sf::Color(194, 178, 129), brightness));  // Make grass color 20% brighter

            break;
        case TileType::DryGrass:
            mSprite.setColor(adjustColorIntensity(sf::Color(40, 77, 0), brightness));  // Make grass color 20% brighter

            break;
        case TileType::NormalGrass:
            mSprite.setColor(adjustColorIntensity(sf::Color(60, 97, 20), brightness));  // Make grass color 20% brighter

            break;
        case TileType::LushGrass:
            mSprite.setColor(adjustColorIntensity(sf::Color(90, 127, 50), brightness));  // Make grass color 20% brighter

            break;
        case TileType::RockyMountain:
            mSprite.setColor(adjustColorIntensity(sf::Color(139, 141, 123), brightness));  // Make grass color 20% brighter

            break;
        case TileType::Mountain:
            mSprite.setColor(adjustColorIntensity(sf::Color(160, 162, 143), brightness));  // Make grass color 20% brighter

            break;
        case TileType::SnowyMountain:
            mSprite.setColor(adjustColorIntensity(sf::Color(235, 235, 235), brightness));

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
