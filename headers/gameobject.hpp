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
    void setSpriteColor(const sf::Color& color) {
        mSprite.setColor(color);  // Set the color (RGBA) of the sprite
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

    sf::FloatRect getUpperHalfInteractionRange() const {
        sf::FloatRect bounds = getInteractionRange();
        return sf::FloatRect(bounds.left , bounds.top , bounds.width, bounds.height / 2);
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
        mHealth -= 10;
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
        return sf::FloatRect(bounds.left - 16, bounds.top - 16, bounds.width + 32, bounds.height + 32); // Same for now
    }

};

// Rock subclass
class Rock : public GameObject {
public:
    Rock(const sf::Vector2f& position, const sf::Texture& texture)
        : GameObject(position, texture, 200) {}

    void interact() override {
        mHealth -= 20;
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
        return sf::FloatRect(bounds.left - 16, bounds.top - 16, bounds.width + 32, bounds.height + 32); // Same for now
    }
};

// Bush subclass
class Bush : public GameObject {
public:
    Bush(const sf::Vector2f& position, const sf::Texture& texture)
        : GameObject(position, texture, 150) {}

    void interact() override {
        mHealth -= 20;
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
        return sf::FloatRect(bounds.left - 16, bounds.top - 16, bounds.width + 32, bounds.height + 32); // Same for now
    }
};


#endif // GAMEOBJECT_HPP
