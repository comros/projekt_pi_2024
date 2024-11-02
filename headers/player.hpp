#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "../headers/animation.hpp"
#include "../headers/definitions.hpp"

// Declaring animation set for player
enum class AnimationType {
    IdleDown,
    IdleLeftDown,
    IdleLeftUp,
    IdleUp,
    IdleRightUp,
    IdleRightDown,
    WalkDown,
    WalkLeftDown,
    WalkLeftUp,
    WalkUp,
    WalkRightUp,
    WalkRightDown,
    Count
};

class Player {
public:
    Player();
    void handleInput();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window, float zoomFactor);
    void setDirection(const sf::Vector2f& newDirection);

private:
    void checkBounds();
    void animate(float deltaTime);

    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f direction;
    sf::Vector2f velocity;
    sf::View view;
    float speed;
    bool isCameraLocked = true;
    Animation animations[int(AnimationType::Count)]; // Making an array of animations (default constructor is needed)
    AnimationType currentAnimation;
};



#endif //PLAYER_HPP
