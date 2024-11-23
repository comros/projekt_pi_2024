#include "../headers/player.hpp"
#include <cmath>
#include <iostream>

#include "../headers/InputHandler.hpp"
#include "../headers/audio.hpp"
#include <random>

// Setting sprite's starting values
Player::Player()
{

    mSprite.setPosition(mPosition);
    mSprite.setOrigin(PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);

    // Initialize animations
    mAnimations[static_cast<int>(PlayerAnimation::IdleDown)] = Animation(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_IDLE_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::IdleLeftDown)] = Animation(0, PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_IDLE_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::IdleLeftUp)] = Animation(0, PLAYER_HEIGHT*2, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_IDLE_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::IdleUp)] = Animation(0, PLAYER_HEIGHT*3, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_IDLE_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::IdleRightUp)] = Animation(0, PLAYER_HEIGHT*4, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_IDLE_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::IdleRightDown)] = Animation(0, PLAYER_HEIGHT*5, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_IDLE_ATLAS);

    mAnimations[static_cast<int>(PlayerAnimation::WalkDown)] = Animation(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WALK_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::WalkLeftDown)] = Animation(0, PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WALK_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::WalkLeftUp)] = Animation(0, PLAYER_HEIGHT*2, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WALK_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::WalkUp)] = Animation(0, PLAYER_HEIGHT*3, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WALK_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::WalkRightUp)] = Animation(0, PLAYER_HEIGHT*4, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WALK_ATLAS);
    mAnimations[static_cast<int>(PlayerAnimation::WalkRightDown)] = Animation(0, PLAYER_HEIGHT*5, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WALK_ATLAS);

    mSoundEffects.loadSound("Walk", WALKSOUND);
    mSoundEffects.setGlobalVolume(15.0f);
}

void Player::setDirection(const sf::Vector2f& newDirection)
{
    mDirection = newDirection;
}

void Player::animate(float deltaTime)
{

    // Starting idle animation based on the previous direction
    if (mCurrentAnimation == PlayerAnimation::WalkDown) mCurrentAnimation = PlayerAnimation::IdleDown;
    if (mCurrentAnimation == PlayerAnimation::WalkLeftDown) mCurrentAnimation = PlayerAnimation::IdleLeftDown;
    if (mCurrentAnimation == PlayerAnimation::WalkLeftUp) mCurrentAnimation = PlayerAnimation::IdleLeftUp;
    if (mCurrentAnimation == PlayerAnimation::WalkUp) mCurrentAnimation = PlayerAnimation::IdleUp;
    if (mCurrentAnimation == PlayerAnimation::WalkRightUp) mCurrentAnimation = PlayerAnimation::IdleRightUp;
    if (mCurrentAnimation == PlayerAnimation::WalkRightDown) mCurrentAnimation = PlayerAnimation::IdleRightDown;

    // Setting walking animation based on x(-1,0,1) y(-1,0,1) movement
    if (mDirection.x == 0 && mDirection.y == -1) mCurrentAnimation = PlayerAnimation::WalkDown;
    if (mDirection.x == -1 && mDirection.y == -1) mCurrentAnimation = PlayerAnimation::WalkLeftDown;
    if (mDirection.x == -1 && mDirection.y == 0) mCurrentAnimation = PlayerAnimation::WalkLeftDown;
    if (mDirection.x == -1 && mDirection.y == 1) mCurrentAnimation = PlayerAnimation::WalkLeftUp;
    if (mDirection.x == 0 && mDirection.y == 1) mCurrentAnimation = PlayerAnimation::WalkUp;
    if (mDirection.x == 1 && mDirection.y == 1) mCurrentAnimation = PlayerAnimation::WalkRightUp;
    if (mDirection.x == 1 && mDirection.y == 0) mCurrentAnimation = PlayerAnimation::WalkRightDown;
    if (mDirection.x == 1 && mDirection.y == -1) mCurrentAnimation = PlayerAnimation::WalkRightDown;

    // Updating animation
    mAnimations[static_cast<int>(mCurrentAnimation)].Update(deltaTime);
    mAnimations[static_cast<int>(mCurrentAnimation)].ApplyToSprite(mSprite);
}

void Player::updatePosition(const float deltaTime)
{

    if(mDirection.x != 0 || mDirection.y != 0) mSoundEffects.playSound("Walk"); //Starts playing walking sound
    else mSoundEffects.stopSound("Walk"); // //Stops playing walking sound

    // Creating randomized value for pitch
    std::random_device random;
    std::uniform_real_distribution<float> dist(0.5f, 1.25f);

    mSoundEffects.setPitch("Walk",mSpeed / 90.0f * dist(random) ); // Sets sound playing speed according to player speed

    // Only normalize for velocity, do not alter `direction` directly for animation
    sf::Vector2f normalizedDirection = mDirection;
    float vectorLength = std::sqrt(mDirection.x * mDirection.x + mDirection.y * mDirection.y);
    if (vectorLength != 0)
    {
        normalizedDirection.x /= vectorLength;
        normalizedDirection.y /= vectorLength;
    }

    // Setting the velocity and position accordingly
    mVelocity.x = mSpeed * normalizedDirection.x;
    mVelocity.y = mSpeed * -normalizedDirection.y; // Because SFML's graphic library positive y-axis points downwards
    mPosition.x += mVelocity.x * deltaTime;
    mPosition.y += mVelocity.y * deltaTime;

    // checkBounds();

    animate(deltaTime);

    // Updating position
    mSprite.setPosition(mPosition);
}

// Creating a bounding box so that the sprite won't go outside the screen and instead makes it bounce back
// Padding is needed due to SFML2 using the (0,0) of the shape as a reference point
// Adjusting the bounding box size by subtracting the shape's size makes it so the box won't leave the screen
void Player::checkBounds()
{
    if (mPosition.x < PLAYER_PADDING_LEFT) mPosition.x = PLAYER_PADDING_LEFT;
    if (mPosition.x > WINDOW_WIDTH - PLAYER_PADDING_RIGHT) mPosition.x = WINDOW_WIDTH - PLAYER_PADDING_RIGHT;
    if (mPosition.y < PLAYER_PADDING_UP) mPosition.y = PLAYER_PADDING_UP;
    if (mPosition.y > WINDOW_HEIGHT - PLAYER_PADDING_DOWN) mPosition.y = WINDOW_HEIGHT - PLAYER_PADDING_DOWN;
}

void Player::updateCamera(sf::RenderWindow& window)
{
    mView = window.getDefaultView();

    if (mIsCameraLocked)
    {
        fixedCameraPosition = mPosition;
        mView.setCenter(mPosition);
    }
    else mView.setCenter(fixedCameraPosition); // Leave's camera in place where player pressed unlock button

    mView.zoom(mZoomFactor);
    window.setView(mView);
}