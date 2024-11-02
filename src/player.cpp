#include "../headers/player.hpp"
#include <cmath>

// Setting sprite's starting values
Player::Player() : position(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), speed(100.0f), currentAnimation(AnimationType::IdleDown) {
    sprite.setPosition(position);
    sprite.setOrigin(SPRITE_SIZE_X / 2, SPRITE_SIZE_Y / 2);

    // Initialize animations
    animations[int(AnimationType::IdleDown)] = Animation(0, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationType::IdleLeftDown)] = Animation(0, SPRITE_SIZE_Y, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationType::IdleLeftUp)] = Animation(0, SPRITE_SIZE_Y*2, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationType::IdleUp)] = Animation(0, SPRITE_SIZE_Y*3, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationType::IdleRightUp)] = Animation(0, SPRITE_SIZE_Y*4, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationType::IdleRightDown)] = Animation(0, SPRITE_SIZE_Y*5, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);

    animations[int(AnimationType::WalkDown)] = Animation(0, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationType::WalkLeftDown)] = Animation(0, SPRITE_SIZE_Y, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationType::WalkLeftUp)] = Animation(0, SPRITE_SIZE_Y*2, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationType::WalkUp)] = Animation(0, SPRITE_SIZE_Y*3, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationType::WalkRightUp)] = Animation(0, SPRITE_SIZE_Y*4, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationType::WalkRightDown)] = Animation(0, SPRITE_SIZE_Y*5, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
}

void Player::setDirection(const sf::Vector2f& newDirection) {
    direction = newDirection;
}

void Player::animate(float deltaTime)
{
    // Starting idle animation based on the previous direction
    if (currentAnimation == AnimationType::WalkDown) currentAnimation = AnimationType::IdleDown;
    if (currentAnimation == AnimationType::WalkLeftDown) currentAnimation = AnimationType::IdleLeftDown;
    if (currentAnimation == AnimationType::WalkLeftUp) currentAnimation = AnimationType::IdleLeftUp;
    if (currentAnimation == AnimationType::WalkUp) currentAnimation = AnimationType::IdleUp;
    if (currentAnimation == AnimationType::WalkRightUp) currentAnimation = AnimationType::IdleRightUp;
    if (currentAnimation == AnimationType::WalkRightDown) currentAnimation = AnimationType::IdleRightDown;

    // Setting walking animation based on x(-1,0,1) y(-1,0,1) movement
    if (direction.x == 0 && direction.y == -1) currentAnimation = AnimationType::WalkDown;
    if (direction.x == -1 && direction.y == -1) currentAnimation = AnimationType::WalkLeftDown;
    if (direction.x == -1 && direction.y == 0) currentAnimation = AnimationType::WalkLeftDown;
    if (direction.x == -1 && direction.y == 1) currentAnimation = AnimationType::WalkLeftUp;
    if (direction.x == 0 && direction.y == 1) currentAnimation = AnimationType::WalkUp;
    if (direction.x == 1 && direction.y == 1) currentAnimation = AnimationType::WalkRightUp;
    if (direction.x == 1 && direction.y == 0) currentAnimation = AnimationType::WalkRightDown;
    if (direction.x == 1 && direction.y == -1) currentAnimation = AnimationType::WalkRightDown;

    // Updating animation
    animations[int(currentAnimation)].Update(deltaTime);
    animations[int(currentAnimation)].ApplyToSprite(sprite);
}

void Player::update(float deltaTime) {

    // Only normalize for velocity, do not alter `direction` directly for animation
    sf::Vector2f normalizedDirection = direction;
    float vectorLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (vectorLength != 0) {
        normalizedDirection.x /= vectorLength;
        normalizedDirection.y /= vectorLength;
    }

    // Setting the velocity and position accordingly
    velocity.x = speed * normalizedDirection.x;
    velocity.y = speed * -normalizedDirection.y; // Because SFML's graphic library positive y-axis points downwards
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    checkBounds();

    animate(deltaTime);

    // Updating position
    sprite.setPosition(position);
}

// Creating a bounding box so that the sprite won't go outside the screen and instead makes it bounce back
// Padding is needed due to SFML2 using the (0,0) of the shape as a reference point
// Adjusting the bounding box size by subtracting the shape's size makes it so the box won't leave the screen
void Player::checkBounds() {
    if (position.x < SPRITE_PADDING_LEFT) position.x = SPRITE_PADDING_LEFT;
    if (position.x > WINDOW_WIDTH - SPRITE_PADDING_RIGHT) position.x = WINDOW_WIDTH - SPRITE_PADDING_RIGHT;
    if (position.y < SPRITE_PADDING_UP) position.y = SPRITE_PADDING_UP;
    if (position.y > WINDOW_HEIGHT - SPRITE_PADDING_DOWN) position.y = WINDOW_HEIGHT - SPRITE_PADDING_DOWN;
}

float keepPositionX = 0, keepPositionY = 0;
void Player::draw(sf::RenderWindow& window, float zoomFactor) {

    // Camera work
    view = window.getDefaultView();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3) && isCameraLocked != false)
    {
        isCameraLocked = false;
        keepPositionX = position.x;
        keepPositionY = position.y;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4) && isCameraLocked != true) isCameraLocked = true;

    if (isCameraLocked) view.setCenter(position.x, position.y);
    else view.setCenter(keepPositionX, keepPositionY);

    view.zoom(zoomFactor);
    window.setView(view);
    window.draw(sprite);
}