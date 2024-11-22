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
            mSprite.setColor(sf::Color(0, 62, 178)); // Deep water
            break;
        case TileType::ShallowWater:
            mSprite.setColor(sf::Color(9, 82, 198)); // Shallow water
            break;
        case TileType::DrySand:
            mSprite.setColor(sf::Color(134, 118, 69)); // Dry sand
            break;
        case TileType::NormalSand:
            mSprite.setColor(sf::Color(164, 148, 99)); // Normal sand
            break;
        case TileType::WetSand:
            mSprite.setColor(sf::Color(194, 178, 129)); // Wet sand
            break;
        case TileType::DryGrass:
            mSprite.setColor(sf::Color(40, 77, 0)); // Dry grass
            break;
        case TileType::NormalGrass:
            mSprite.setColor(sf::Color(60, 97, 20)); // Normal grass
            break;
        case TileType::LushGrass:
            mSprite.setColor(sf::Color(90, 127, 50)); // Lush grass
            break;
        case TileType::RockyMountain:
            mSprite.setColor(sf::Color(139, 141, 123)); // Rocky mountain
            break;
        case TileType::Mountain:
            mSprite.setColor(sf::Color(160, 162, 143)); // Mountain
            break;
        case TileType::SnowyMountain:
            mSprite.setColor(sf::Color(235, 235, 235)); // Snowy mountain
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
};

#endif // TILE_HPP
