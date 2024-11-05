#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "../headers/animation.hpp"
#include "../headers/definitions.hpp"

// Declaring animation set for player
enum class PlayerAnimation {
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
    void updatePosition(float deltaTime);
    void updateCamera(sf::RenderWindow& window);

    // Getters and setters
    void setDirection(const sf::Vector2f& newDirection);

    sf::Vector2f getPosition() const { return mPosition; }
    void setPosition(const sf::Vector2f& newPosition) { mPosition = newPosition; }

    float getSpeed() const { return mSpeed; }
    void setSpeed(const float newSpeed) { mSpeed = newSpeed; }

    sf::Sprite getSprite() { return mSprite; }

    void setCameraLocked(const bool newLockState) { mIsCameraLocked = newLockState; }

    float getZoomFactor() const { return mZoomFactor; }
    void setZoomFactor(const float newZoomFactor) { mZoomFactor -= newZoomFactor; }

private:
    void checkBounds();
    void animate(float deltaTime);

    sf::Sprite mSprite;
    sf::Vector2f mPosition = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
    sf::Vector2f mDirection;
    sf::Vector2f mVelocity;
    sf::View mView;
    float mSpeed = 100.0f;

    float mZoomFactor = 0.225f;
    bool mIsCameraLocked = true;
    sf::Vector2f fixedCameraPosition = {0, 0};

    // Making an array of animations (default constructor is needed)
    Animation mAnimations[static_cast<int>(PlayerAnimation::Count)];
    PlayerAnimation mCurrentAnimation = {PlayerAnimation::IdleDown};
};



#endif //PLAYER_HPP
