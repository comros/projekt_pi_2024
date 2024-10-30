#include <SFML/Graphics.hpp> // USES
#include <cmath>
#include <ctime>
#include <iostream>

#define SPRITE_SCALE 6
#define CHAR_ATLAS_PATH "../../assets/fantasy_txtpack/Player/Player.png"

// Needed to adjust for the player's sprite white space, remember sprite's origin is in the middle
#define SPRITE_SIZE 32
#define SPRITE_PADDING_UP (-11 * SPRITE_SCALE)
#define SPRITE_PADDING_RIGHT (6 * SPRITE_SCALE)
#define SPRITE_PADDING_DOWN (9 * SPRITE_SCALE)
#define SPRITE_PADDING_LEFT (-6 * SPRITE_SCALE)

int main() {

    // Creating a window and capping fps
    sf::RenderWindow window(sf::VideoMode(1920,1080), "Projekt pi 2024");
    window.setFramerateLimit(60);

    // Project build is in ./cmake-build-debug/bin, so in order to access the assets folder "../../" is needed
    sf::Texture texture;
    // Limiting to only a certain part of the atlas and adjusting for the left empty space
    // Sprites in the player atlas are 13 x 20
    // They are 19 pixels (+player size) apart on x and 12 pixels (+player size) apart on y
    sf::IntRect textureRect(0,0,SPRITE_SIZE,SPRITE_SIZE);
    sf::Sprite sprite;

    // Setting sprite's starting position
    auto spritePosition = sf::Vector2f(850,450); // auto -> sf::Vector2f
    sprite.setPosition(spritePosition);

    // Variables
    float speed = 8;
    auto velocity = sf::Vector2f(0,0);
    auto dir = sf::Vector2f(0, 0);
    int flip = 1;

    // Game loop
    while (window.isOpen()) {
        sf::Event event{};

        texture.loadFromFile(CHAR_ATLAS_PATH, textureRect);
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(SPRITE_SCALE * flip,SPRITE_SCALE));
        sprite.setOrigin(SPRITE_SIZE/2, SPRITE_SIZE/2); // NOTE: SPRITE_SIZE SHOULD BE DIVISIBLE BY 2

        /// INPUT

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close(); // Checks if the x button on the window is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

            dir.x = 0, dir.y = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                dir.y = -1;
                textureRect.left = 0;
                textureRect.top = 64;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                dir.y = 1;
                textureRect.left = 0;
                textureRect.top = 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                dir.x = -1;
                textureRect.left = 0;
                textureRect.top = 32;
                flip = -1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                dir.x = 1;
                textureRect.left = 0;
                textureRect.top = 32;
                flip = 1;
            }
        }

        /// PHYSICS

        // Normalizing the direction vector
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
        if (spritePosition.x < 0 - SPRITE_PADDING_LEFT || spritePosition.x > 1920 - SPRITE_PADDING_RIGHT) spritePosition.x = 850;
        if (spritePosition.y < 0 - SPRITE_PADDING_UP|| spritePosition.y > 1080 - SPRITE_PADDING_DOWN) spritePosition.y = 450;

        // Setting the velocity and updating the position
        spritePosition.x += velocity.x;
        spritePosition.y += velocity.y;
        sprite.setPosition(spritePosition);

        /// RENDERING

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}