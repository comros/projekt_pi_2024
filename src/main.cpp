#include <SFML/Graphics.hpp> // USES
#include <cmath>
#include <ctime>
#include <iostream>

#define SPRITE_SCALE 6

// Needed to adjust for the player's sprite white space
#define SPRITE_PADDING_UP (5 * SPRITE_SCALE)
#define SPRITE_PADDING_RIGHT ((32 - 10) * SPRITE_SCALE)
#define SPRITE_PADDING_DOWN ((32 - 7) * SPRITE_SCALE)
#define SPRITE_PADDING_LEFT (9 * SPRITE_SCALE)

int main() {

    // Creating a window and capping fps
    sf::RenderWindow window(sf::VideoMode(1920,1080), "Projekt pi 2024");
    window.setFramerateLimit(60);

    // Project build is in ./cmake-build-debug/bin, so in order to access the assets folder "../../" is needed
    sf::Texture texture;
    // Limiting to only a certain part of the atlas and adjusting for the left empty space
    // Sprites in the player atlas are 13 x 20
    // They are 19 pixels (+player size) apart on x and 12 pixels (+player size) apart on y
    sf::IntRect textureRect(0,0,32,32);
    texture.loadFromFile("../../assets/fantasy_txtpack/Player/Player.png", textureRect);
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // Setting sprite's values
    auto spritePosition = sf::Vector2f(850,450); // auto -> sf::Vector2f
    sprite.setPosition(spritePosition);
    sprite.scale(sf::Vector2f(SPRITE_SCALE,SPRITE_SCALE));

    // Variables
    float speed = 12;
    auto velocity = sf::Vector2f(0,0);
    auto dir = sf::Vector2f(0, 0);

    while (window.isOpen()) {
        sf::Event event{};

        /// INPUT

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close(); // Checks if the x button on the window is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

            dir.x = 0, dir.y = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                dir.y = -1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                dir.y = 1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                dir.x = -1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                dir.x = 1;
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