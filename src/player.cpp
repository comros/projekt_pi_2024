#include "../headers/player.hpp"
#include <cmath>

// Setting sprite's starting values
Player::Player() : position(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), speed(350.0f), currentAnimation(AnimationIndex::IdleDown) {
    sprite.setPosition(position);
    sprite.setScale(SPRITE_SCALE, SPRITE_SCALE);
    sprite.setOrigin(SPRITE_SIZE_X / 2, SPRITE_SIZE_Y / 2);

    // Initialize animations
    animations[int(AnimationIndex::IdleDown)] = Animation(0, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationIndex::IdleLeftDown)] = Animation(0, SPRITE_SIZE_Y, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationIndex::IdleLeftUp)] = Animation(0, SPRITE_SIZE_Y*2, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationIndex::IdleUp)] = Animation(0, SPRITE_SIZE_Y*3, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationIndex::IdleRightUp)] = Animation(0, SPRITE_SIZE_Y*4, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);
    animations[int(AnimationIndex::IdleRightDown)] = Animation(0, SPRITE_SIZE_Y*5, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_IDLE_ATLAS);

    animations[int(AnimationIndex::WalkDown)] = Animation(0, 0, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationIndex::WalkLeftDown)] = Animation(0, SPRITE_SIZE_Y, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationIndex::WalkLeftUp)] = Animation(0, SPRITE_SIZE_Y*2, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationIndex::WalkUp)] = Animation(0, SPRITE_SIZE_Y*3, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationIndex::WalkRightUp)] = Animation(0, SPRITE_SIZE_Y*4, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
    animations[int(AnimationIndex::WalkRightDown)] = Animation(0, SPRITE_SIZE_Y*5, SPRITE_SIZE_X, SPRITE_SIZE_Y, P_WALK_ATLAS);
}

void Player::handleInput() {

    // Starting idle animation based on the previous direction
    if (currentAnimation == AnimationIndex::WalkDown) currentAnimation = AnimationIndex::IdleDown;
    if (currentAnimation == AnimationIndex::WalkLeftDown) currentAnimation = AnimationIndex::IdleLeftDown;
    if (currentAnimation == AnimationIndex::WalkLeftUp) currentAnimation = AnimationIndex::IdleLeftUp;
    if (currentAnimation == AnimationIndex::WalkUp) currentAnimation = AnimationIndex::IdleUp;
    if (currentAnimation == AnimationIndex::WalkRightUp) currentAnimation = AnimationIndex::IdleRightUp;
    if (currentAnimation == AnimationIndex::WalkRightDown) currentAnimation = AnimationIndex::IdleRightDown;

    direction = {0, 0};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        direction.y != -1 ? direction.y = 1 : direction.y = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        direction.y != 1 ? direction.y = -1 : direction.y = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        direction.x = -1;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        direction.x = 1;


    // Setting walking animation based on x(-1,0,1) y(-1,0,1) movement
    if (direction.x == 0 && direction.y == -1) currentAnimation = AnimationIndex::WalkDown;
    if (direction.x == -1 && direction.y == -1) currentAnimation = AnimationIndex::WalkLeftDown;
    if (direction.x == -1 && direction.y == 0) currentAnimation = AnimationIndex::WalkLeftDown;
    if (direction.x == -1 && direction.y == 1) currentAnimation = AnimationIndex::WalkLeftUp;
    if (direction.x == 0 && direction.y == 1) currentAnimation = AnimationIndex::WalkUp;
    if (direction.x == 1 && direction.y == 1) currentAnimation = AnimationIndex::WalkRightUp;
    if (direction.x == 1 && direction.y == 0) currentAnimation = AnimationIndex::WalkRightDown;
    if (direction.x == 1 && direction.y == -1) currentAnimation = AnimationIndex::WalkRightDown;
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
    velocity.y = speed * -direction.y; // Because SFML's graphic library positive y-axis points downwards
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    checkBounds();

    // Updating animation
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