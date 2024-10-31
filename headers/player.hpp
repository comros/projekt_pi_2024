#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "../headers/animation.hpp"
#include "../headers/definitions.hpp"

// Declaring animation set for player
enum class AnimationIndex {
    IdleUp,
    IdleDown,
    IdleSide,
    WalkingUp,
    WalkingDown,
    WalkingSide,
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
    int flip; // NOTE: This is necessary, because the atlas doesn't contain left-oriented sprites
    Animation animations[int(AnimationIndex::Count)]; // Making an array of animations (default constructor is needed)
    AnimationIndex currentAnimation;
};



#endif //PLAYER_HPP
