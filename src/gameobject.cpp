#include "../headers/gameobject.hpp"
#include <iostream>

// Tree implementation
Tree::Tree(const sf::Vector2f& position, const sf::Texture& texture)
    : GameObject(position, texture, 100) {}

void Tree::interact() {
    mHealth -= 10;
    if (mHealth <= 0) {
        std::cout << "Tree destroyed!" << std::endl;
    }
}

// Rock implementation
Rock::Rock(const sf::Vector2f& position, const sf::Texture& texture)
    : GameObject(position, texture, 200) {}

void Rock::interact() {
    mHealth -= 20;
    if (mHealth <= 0) {
        std::cout << "Rock mined!" << std::endl;
    }
}

// Bush implementation
Bush::Bush(const sf::Vector2f& position, const sf::Texture& texture)
    : GameObject(position, texture, 200) {}

void Bush::interact() {
    mHealth -= 20;
    if (mHealth <= 0) {
        std::cout << "Rock mined!" << std::endl;
    }
}
