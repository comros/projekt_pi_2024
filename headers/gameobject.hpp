#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <SFML/Graphics.hpp>
#include <memory>

// Base class
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

protected:
    sf::Vector2f mPosition;
    sf::Texture mTexture;
    sf::Sprite mSprite;
    int mHealth;
};

// Tree declaration
class Tree : public GameObject {
public:
    Tree(const sf::Vector2f& position, const sf::Texture& texture);
    void interact() override;
};

// Rock declaration
class Rock : public GameObject {
public:
    Rock(const sf::Vector2f& position, const sf::Texture& texture);
    void interact() override;
};

// Bush declaration
class Bush : public GameObject {
public:
    Bush(const sf::Vector2f& position, const sf::Texture& texture);
    void interact() override;
};

#endif // GAMEOBJECT_HPP
