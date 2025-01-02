#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

class GameObject {
public:
    GameObject(const sf::Vector2f& position, const sf::Texture& texture, int health)
        : mPosition(position), mTexture(texture), mHealth(health) {
        mSprite.setTexture(mTexture);
        mSprite.setPosition(mPosition);
    }

    virtual ~GameObject() = default;
    virtual void interact() = 0; // Pure virtual function

    int getHealth() const { return mHealth; }
    const sf::Sprite& getSprite() const { return mSprite; }

    // Method to change sprite color using RGBA
    void adjustAlpha(float alphaMultiplier) {
        // Get the current color of the sprite
        sf::Color currentColor = mSprite.getColor();

        // Calculate the new alpha value based on the multiplier
        int newAlpha = static_cast<int>(currentColor.a * alphaMultiplier);
        newAlpha = std::clamp(newAlpha, 0, 255);  // Clamp between 0 and 255 to ensure it's valid

        // Set the new color with the same RGB values but updated alpha
        mSprite.setColor(sf::Color(currentColor.r, currentColor.g, currentColor.b, newAlpha));
    }

    void setBrightness(const sf::Color& color, float brightness) {
        mSprite.setColor(adjustColorIntensity(color, brightness));  // Set the color (RGBA) of the sprite
    }

    sf::Color adjustColorIntensity(const sf::Color& originalColor, float multiplier) {
        return sf::Color(
            std::min(static_cast<int>(originalColor.r * multiplier), 255),
            std::min(static_cast<int>(originalColor.g * multiplier), 255),
            std::min(static_cast<int>(originalColor.b * multiplier), 255)
        );
    }

    // Get position method
    const sf::Vector2f& getPosition() const { return mPosition; }

    // Default collision box (full sprite bounds)
    virtual sf::FloatRect getCollisionBox() const {
        return mSprite.getGlobalBounds();
    }

    // Virtual method to be overridden in derived classes
    virtual sf::FloatRect getInteractionRange() const {
        sf::FloatRect bounds = getCollisionBox();
        // Expand the bounds by 16 pixels in all directions
        return sf::FloatRect(bounds.left - 16, bounds.top - 16, bounds.width + 32, bounds.height + 32);
    }

    virtual sf::FloatRect getUpperHalfInteractionRange() const {
        sf::FloatRect bounds = getInteractionRange();
        return sf::FloatRect(bounds.left , bounds.top, bounds.width, bounds.height / 2);
    }

    bool isInUpperHalfOfInteractionRange(const sf::Vector2f& playerPosition) const {
        return getUpperHalfInteractionRange().contains(playerPosition);
    }

protected:
    sf::Vector2f mPosition;
    sf::Texture mTexture;
    sf::Sprite mSprite;
    int mHealth;
};

// Tree subclass
class Tree : public GameObject {
public:
    Tree(const sf::Vector2f& position, const sf::Texture& texture)
        : GameObject(position, texture, 100) {}

    void interact() override {
        mHealth -= 40;
        if (mHealth <= 0) {
            std::cout << "Tree destroyed!" << std::endl;
        }
    }

    // Override collision box for 16x16 pixels at the bottom center
    sf::FloatRect getCollisionBox() const override {
        sf::Vector2f spriteSize(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height);
        return sf::FloatRect(
            mPosition.x + spriteSize.x / 2 - 8, // Center horizontally
            mPosition.y + spriteSize.y - 16,   // Align bottom
            16,                                // Width
            16                                 // Height
        );
    }

    // Override the interaction range (can add different logic in the future)
    sf::FloatRect getInteractionRange() const override {
        sf::FloatRect bounds = getCollisionBox();
        return sf::FloatRect(bounds.left - 32, bounds.top - 32, bounds.width + 64, bounds.height + 64);
    }


    sf::FloatRect getUpperHalfInteractionRange() const override{
        sf::FloatRect bounds = getInteractionRange();
        return sf::FloatRect(bounds.left + 24, bounds.top + 8, bounds.width - 48, bounds.height / 2 - 16);
    }
};

// Rock subclass
class Rock : public GameObject {
public:
    Rock(const sf::Vector2f& position, const sf::Texture& texture)
        : GameObject(position, texture, 100) {}

    void interact() override {
        mHealth -= 30;
        if (mHealth <= 0) {
            std::cout << "Rock mined!" << std::endl;

        }
    }

    // Rock uses full sprite as collision box
    sf::FloatRect getCollisionBox() const override {
        return mSprite.getGlobalBounds();
    }

    // Override the interaction range (can add different logic in the future)
    sf::FloatRect getInteractionRange() const override {
        sf::FloatRect bounds = getCollisionBox();
        return sf::FloatRect(bounds.left - 16, bounds.top - 16, bounds.width + 32, bounds.height + 32);
    }
};

// Bush subclass
class Bush : public GameObject {
public:
    Bush(const sf::Vector2f& position, const sf::Texture& texture)
        : GameObject(position, texture, 100) {}

    void interact() override {
        mHealth -= 50;
        if (mHealth <= 0) {
            std::cout << "Bush destroyed!" << std::endl;
        }
    }

    // Bush uses full sprite as collision box
    sf::FloatRect getCollisionBox() const override {
        return mSprite.getGlobalBounds();
    }

    // Override the interaction range (can add different logic in the future)
    sf::FloatRect getInteractionRange() const override {
        sf::FloatRect bounds = getCollisionBox();
        return sf::FloatRect(bounds.left - 16, bounds.top - 16, bounds.width + 32, bounds.height + 32);
    }
};


#endif // GAMEOBJECT_HPP
