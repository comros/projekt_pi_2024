#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

#include "gameobject.hpp"

#include "../headers/animation.hpp"
#include "../headers/definitions.hpp"
#include "../headers/audio.hpp"


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
    void updatePosition(float deltaTime, const std::vector<std::shared_ptr<GameObject>>& objects);
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

    void setEffectsVolume(const float volume) { mSoundEffects.setGlobalVolume(volume); };


    sf::View getCamera() const { return mView; }

    void renderCollisionBox(sf::RenderWindow& window);

    // Method to change sprite color using RGBA
    void adjustPlayerBrightness(float brightness) {
        mSprite.setColor(adjustColorIntensity(sf::Color(255,255,255,255), brightness));  // Set the color (RGBA) of the sprite
    }


private:
    void keepInWorldBounds();
    void animate(float deltaTime);

    Audio mSoundEffects;

    bool mInventoryOpen = false;


    sf::Sprite mSprite;
    sf::Vector2f mPosition; // Middle of the map (if map is 256x256 and tiles are 16x16)
    sf::Vector2f mDirection;
    sf::Vector2f mVelocity;
    sf::View mView;
    float mSpeed = 100.0f;
    float mZoomFactor = 0.09f;
    bool mIsCameraLocked = true;
    sf::Vector2f fixedCameraPosition = {0, 0};

    // Making an array of animations (default constructor is needed)
    Animation mAnimations[static_cast<int>(PlayerAnimation::Count)];
    PlayerAnimation mCurrentAnimation = {PlayerAnimation::IdleDown};

    sf::Color adjustColorIntensity(const sf::Color& originalColor, float multiplier) {
        return sf::Color(
            std::min(static_cast<int>(originalColor.r * multiplier), 255),
            std::min(static_cast<int>(originalColor.g * multiplier), 255),
            std::min(static_cast<int>(originalColor.b * multiplier), 255)
        );
    }

};



#endif //PLAYER_HPP
