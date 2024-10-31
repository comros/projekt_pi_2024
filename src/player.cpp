#include "../headers/player.hpp"
#include <cmath>

// Setting sprite's starting values
Player::Player() : position(850, 450), speed(350.0f), flip(1), currentAnimation(AnimationIndex::IdleDown) {
    sprite.setPosition(position);
    sprite.setOrigin(SPRITE_SIZE / 2, SPRITE_SIZE / 2);

    // Initialize animations
    animations[int(AnimationIndex::IdleUp)] = Animation(0, SPRITE_SIZE * 2, SPRITE_SIZE, SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::IdleDown)] = Animation(0, 0, SPRITE_SIZE, SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::IdleSide)] = Animation(0, SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::WalkingUp)] = Animation(0, SPRITE_SIZE * 5, SPRITE_SIZE, SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::WalkingDown)] = Animation(0, SPRITE_SIZE * 3, SPRITE_SIZE, SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::WalkingSide)] = Animation(0, SPRITE_SIZE * 4, SPRITE_SIZE, SPRITE_SIZE, CHAR_ATLAS_PATH);
}

void Player::handleInput() {

    if (direction.y == -1) currentAnimation = AnimationIndex::IdleUp;
    else if (direction.y == 1) currentAnimation = AnimationIndex::IdleDown;
    else if (std::abs(direction.x) > 0.7f) currentAnimation = AnimationIndex::IdleSide; // 0.7f because we normalize dir vector

    direction = {0, 0};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if (direction.y != 1)
        {
            direction.y = -1;
            currentAnimation = AnimationIndex::WalkingUp;
        }
        else
        {
            direction.y = 0;
            currentAnimation = AnimationIndex::IdleUp;
        }

        flip = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if(direction.y != -1)
        {
            direction.y = 1;
            currentAnimation = AnimationIndex::WalkingDown;
        }
        else
        {
            currentAnimation = AnimationIndex::IdleDown;
            direction.y = 0;
        }

        flip = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x = -1;
        currentAnimation = AnimationIndex::WalkingSide;
        flip = -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x = 1;
        currentAnimation = AnimationIndex::WalkingSide;
        flip = 1;
    }
}

void Player::update(float deltaTime) {
    // Normalizing the direction vector, so the player doesn't move faster going diagonally
    float vectorLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (vectorLength != 0) {
        direction.x /= vectorLength;
        direction.y /= vectorLength;
    }

    // Setting the velocity and position accordingly
    velocity.x = speed * direction.x;
    velocity.y = speed * direction.y;
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    checkBounds();

    // Making sprite bigger and taking into account if the player is going left
    sprite.setScale(SPRITE_SCALE * flip, SPRITE_SCALE);
    animations[int(currentAnimation)].Update(deltaTime);
    animations[int(currentAnimation)].ApplyToSprite(sprite);

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

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}