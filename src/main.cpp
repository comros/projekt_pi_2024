#include <SFML/Graphics.hpp> // USES
#include <cmath> // USES
#include <iostream> // USES
#include "../headers/Animation.h" // USES

#define SPRITE_SCALE 6

// Needed to adjust for the player's sprite white space, remember sprite's origin is in the middle: (32/2, 32/2)
#define SPRITE_SIZE 32
#define SPRITE_PADDING_UP (11 * SPRITE_SCALE)
#define SPRITE_PADDING_RIGHT (6 * SPRITE_SCALE)
#define SPRITE_PADDING_DOWN (9 * SPRITE_SCALE)
#define SPRITE_PADDING_LEFT (6 * SPRITE_SCALE)

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

// Project build is in ./cmake-build-debug/bin, so in order to access the assets folder "../../" is needed
#define CHAR_ATLAS_PATH "../../assets/fantasy_txtpack/Player/Player.png"

// Declaring animation set for player
enum class AnimationIndex
{
    IdleUp,
    IdleDown,
    IdleSide,
    WalkingUp,
    WalkingDown,
    WalkingSide,
    Count
};

int main() {

    // Needed for deltaTime
    sf::Clock clock;

    // Creating a window and capping fps
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), "Projekt pi 2024");
    window.setFramerateLimit(60);

    sf::Sprite sprite;

    // Variables
    float speed = 350.0f;
    auto velocity = sf::Vector2f(0,0);
    auto dir = sf::Vector2f(0, 0);
    bool isMoving = false;
    int flip = 1; // NOTE: This is necessary, because the atlas doesn't contain left-oriented sprites

    /// ANIMATION

    Animation animations[int(AnimationIndex::Count)]; // Making an array of animations (default constructor is needed)
    animations[int(AnimationIndex::IdleUp)] = Animation(0,SPRITE_SIZE*2,SPRITE_SIZE,SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::IdleDown)] = Animation(0,0,SPRITE_SIZE,SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::IdleSide)] = Animation(0,SPRITE_SIZE,SPRITE_SIZE,SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::WalkingUp)] = Animation(0,SPRITE_SIZE*5,SPRITE_SIZE,SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::WalkingDown)] = Animation(0,SPRITE_SIZE*3,SPRITE_SIZE,SPRITE_SIZE, CHAR_ATLAS_PATH);
    animations[int(AnimationIndex::WalkingSide)] = Animation(0,SPRITE_SIZE*4,SPRITE_SIZE,SPRITE_SIZE, CHAR_ATLAS_PATH);

    // Setting sprite's starting values
    auto spritePosition = sf::Vector2f(850,450); // auto -> sf::Vector2f
    sprite.setPosition(spritePosition);
    sprite.setOrigin(SPRITE_SIZE/2, SPRITE_SIZE/2); // NOTE: SPRITE_SIZE SHOULD BE DIVISIBLE BY 2
    AnimationIndex currentAnimation = AnimationIndex::IdleDown;

    // Game loop
    while (window.isOpen()) {
        sf::Event event{};

        // Calculating deltaTime once per frame
        float deltaTime = clock.restart().asSeconds();

        /// INPUT

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close(); // Checks if the x button on the window is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

            isMoving = false;
            if(!isMoving)
            {
                if(dir.y == -1) currentAnimation = AnimationIndex::IdleUp;
                if(dir.y == 1) currentAnimation = AnimationIndex::IdleDown;
                if(dir.x < -0.7f || dir.x > 0.7f) currentAnimation = AnimationIndex::IdleSide; // 0.7f because we normalize dir vector
                dir = {0,0};
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                dir.y = -1;
                isMoving = true;
                currentAnimation = AnimationIndex::WalkingUp;
                flip = 1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                dir.y = 1;
                isMoving = true;
                currentAnimation = AnimationIndex::WalkingDown;
                flip = 1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                dir.x = -1;
                isMoving = true;
                currentAnimation = AnimationIndex::WalkingSide;
                flip = -1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                dir.x = 1;
                isMoving = true;
                currentAnimation = AnimationIndex::WalkingSide;
                flip = 1;
            }
        }

        /// PHYSICS


        // Normalizing the direction vector, so the player doesn't move faster going diagonally
        float vectorLength = sqrt(dir.x * dir.x + dir.y * dir.y);
        if (vectorLength != 0) {
            dir.x /= vectorLength;
            dir.y /= vectorLength;
        }

        velocity.x = speed * dir.x;
        velocity.y = speed * dir.y;

        // Creating a bounding box so that the sprite won't go outside the screen and instead makes it bounce back
        // Padding is needed due to SFML2 using the (0,0) of the shape as a reference point
        // Adjusting the bounding box size by subtracting the shape's size makes it so the box won't leave the screen

        // Check left boundary
        if (spritePosition.x < SPRITE_PADDING_LEFT) {
            spritePosition.x = SPRITE_PADDING_LEFT;
        }
        // Check right boundary
        if (spritePosition.x > WINDOW_WIDTH - SPRITE_PADDING_RIGHT) {
            spritePosition.x = WINDOW_WIDTH - SPRITE_PADDING_RIGHT;
        }

        // Check top boundary
        if (spritePosition.y < SPRITE_PADDING_UP) {
            spritePosition.y = SPRITE_PADDING_UP;
        }
        // Check bottom boundary
        if (spritePosition.y > WINDOW_HEIGHT - SPRITE_PADDING_DOWN) {
            spritePosition.y = WINDOW_HEIGHT - SPRITE_PADDING_DOWN;
        }

        // Setting the velocity
        spritePosition.x += velocity.x * deltaTime;
        spritePosition.y += velocity.y * deltaTime;

        // Making sprite bigger and taking into account if the player is going left
        sprite.setScale(sf::Vector2f(SPRITE_SCALE * flip,SPRITE_SCALE));
        animations[int(currentAnimation)].Update(deltaTime);
        animations[int(currentAnimation)].ApplyToSprite(sprite);

        // Updating position
        sprite.setPosition(spritePosition);

        /// RENDERING

        window.clear();
        window.draw(sprite);
        window.display();

        /// DEBUG

        // std::cout << "Position X: " << spritePosition.x << ", Position Y: " << spritePosition.y << std::endl;
        // std::cout << "Dir.X: " << dir.x << ", Dir.Y: " << dir.y << std::endl;
        std::cout << "vX: " << velocity.x << " vY: " << velocity.y << std::endl;
    }

    return 0;
}