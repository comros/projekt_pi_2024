#include "../headers/player.hpp"
#include <cmath>
#include <iostream>

#include "../headers/audio.hpp"
#include <random>

// Setting sprite's starting values
Player::Player()
{
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

sf::FloatRect getInteractionRange(const std::shared_ptr<GameObject>& object) {
    // Use the object's getInteractionRange method to define a proximity box
    return object->getInteractionRange();
}

void Player::updatePosition(const float deltaTime, const std::vector<std::shared_ptr<GameObject>>& objects)
{
    if (mDirection.x != 0 || mDirection.y != 0)
        mSoundEffects.playSound("Walk"); // Start walking sound
    else
        mSoundEffects.stopSound("Walk"); // Stop walking sound

    // Create randomized value for pitch
    std::random_device random;
    std::uniform_real_distribution<float> dist(0.5f, 1.25f);


    mSoundEffects.setPitch("Walk", mSpeed / 90.0f * dist(random)); // Set sound pitch based on speed

    // Normalize direction for velocity calculation
    sf::Vector2f normalizedDirection = mDirection;
    float vectorLength = std::sqrt(mDirection.x * mDirection.x + mDirection.y * mDirection.y);
    if (vectorLength != 0)
    {
        normalizedDirection.x /= vectorLength;
        normalizedDirection.y /= vectorLength;
    }

    // Calculate potential new position based on movement direction
    mVelocity.x = mSpeed * normalizedDirection.x;
    mVelocity.y = mSpeed * -normalizedDirection.y; // SFML's positive y-axis is downward

    sf::Vector2f newPosition = mPosition + mVelocity * deltaTime;

    // Create a bounding box for the player's new position
    float squareSize = 8.0f;
    sf::FloatRect playerBounds(
        newPosition.x - squareSize / 2,
        newPosition.y + PLAYER_HEIGHT / 5 - squareSize - 2,
        squareSize,
        squareSize
    );

    bool collisionOccurred = false;
    sf::Vector2f overlap(0, 0);

    // Check for collisions with objects
    for (const auto& object : objects)
    {
        // If there's a collision, calculate the overlap and adjust position
        if (playerBounds.intersects(object->getCollisionBox()))
        {
            collisionOccurred = true;

            // Calculate the intersection area to determine overlap
            sf::FloatRect intersection = playerBounds;
            intersection.intersects(object->getCollisionBox(), intersection);

            // First handle horizontal overlap
            if (intersection.width < intersection.height) {
                // Horizontal collision: adjust player position to stop overlapping
                if (newPosition.x < object->getCollisionBox().left) {
                    // Moving to the right, adjust the x position
                    overlap.x = -(playerBounds.left + playerBounds.width - object->getCollisionBox().left);
                }
                else {
                    // Moving to the left, adjust the x position
                    overlap.x = object->getCollisionBox().left + object->getCollisionBox().width - playerBounds.left;
                }
            }

            // Then handle vertical overlap
            if (intersection.height < intersection.width) {
                // Vertical collision: adjust player position to stop overlapping
                if (newPosition.y < object->getCollisionBox().top) {
                    // Moving downward, adjust the y position
                    overlap.y = -(playerBounds.top + playerBounds.height - object->getCollisionBox().top);
                }
                else {
                    // Moving upward, adjust the y position
                    overlap.y = object->getCollisionBox().top + object->getCollisionBox().height - playerBounds.top;
                }
            }
        }
    }

    // Adjust position by the overlap amount if collision occurred
    if (collisionOccurred)
    {
        newPosition.x += overlap.x;
        newPosition.y += overlap.y;
    }

    // Update position if no collision occurred or after adjusting position for collisions
    mPosition = newPosition;

    // Ensure the player stays within world bounds
    keepInWorldBounds();

    // Apply animation based on movement
    animate(deltaTime);

    // Update sprite position
    mSprite.setPosition(mPosition);
}

// Creating a bounding box so that the sprite won't go outside the screen and instead makes it bounce back
// Padding is needed due to SFML2 using the (0,0) of the shape as a reference point
// Adjusting the bounding box size by subtracting the shape's size makes it so the box won't leave the screen
void Player::keepInWorldBounds()
{
    if (mPosition.x < PLAYER_PADDING_LEFT) mPosition.x = PLAYER_PADDING_LEFT;
    if (mPosition.x > 512*16 - PLAYER_PADDING_RIGHT) mPosition.x = 512*16 - PLAYER_PADDING_RIGHT;
    if (mPosition.y < PLAYER_PADDING_UP) mPosition.y = PLAYER_PADDING_UP;
    if (mPosition.y > 512*16 - PLAYER_PADDING_DOWN) mPosition.y = 512*16 - PLAYER_PADDING_DOWN;
}

void Player::updateCamera(sf::RenderWindow& window)
{
    mView = window.getDefaultView();

    // Get the window size and calculate the aspect ratio
    sf::Vector2u windowSize = window.getSize();
    float aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

    // Define world bounds
    float worldWidth = 512.0f * 16.0f;
    float worldHeight = 512.0f * 16.0f;

    // Adjust the view based on the aspect ratio
    if (aspectRatio > 1.0f) {
        // If the window is wider than the target (landscape mode), adjust the width
        mView.setSize(worldWidth, worldWidth / aspectRatio);  // Maintain height and adjust width
    } else {
        // If the window is taller than the target (portrait mode), adjust the height
        mView.setSize(worldHeight * aspectRatio, worldHeight);  // Maintain width and adjust height
    }

    // If camera is locked, follow the player
    if (mIsCameraLocked)
    {
        sf::Vector2f targetPosition = mPosition;
        fixedCameraPosition = targetPosition;

        mView.setCenter(fixedCameraPosition);
    }
    else
    {
        // Camera is unlocked, stay at the fixed position
        mView.setCenter(fixedCameraPosition);
    }

    // Apply zoom and update the window view
    mView.zoom(mZoomFactor);
    window.setView(mView);
}

void Player::renderCollisionBox(sf::RenderWindow& window) {
    // Define the square size for the bounding box
    float squareSize = 8.0f;  // Example square size

    // Calculate the position of the bounding box for the bottom-center square
    sf::FloatRect playerBounds(
        mPosition.x - squareSize / 2,  // Center the bounding box horizontally on the player
        mPosition.y + PLAYER_HEIGHT/5 - squareSize - 2,  // Position it at the bottom of the player sprite
        squareSize,  // Set the width of the bounding box
        squareSize  // Set the height of the bounding box
    );

    // Create a rectangle shape to visualize the bounding box
    sf::RectangleShape boundsRect(sf::Vector2f(squareSize, squareSize));
    boundsRect.setPosition(playerBounds.left, playerBounds.top);  // Set the position based on the calculated bounds
    boundsRect.setFillColor(sf::Color(255, 0, 0, 128));  // Semi-transparent red for visibility

    // Draw the bounding box on the screen
    window.draw(boundsRect);
}