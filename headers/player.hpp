#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "../headers/animation.hpp"
#include "../headers/definitions.hpp"

// Declaring animation set for player
enum class AnimationIndex {
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
    void draw(sf::RenderWindow& window);

private:
    void checkBounds();

    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f direction;
    sf::Vector2f velocity;
    float speed;
    Animation animations[int(AnimationIndex::Count)]; // Making an array of animations (default constructor is needed)
    AnimationIndex currentAnimation;
};



#endif //PLAYER_HPP
