#include <SFML/Graphics.hpp> // USES

#define PADDING 100

int main() {

    // Creating a window and capping fps
    sf::RenderWindow window(sf::VideoMode(1920,1080), "Projekt pi 2024");
    window.setFramerateLimit(60);

    sf::RectangleShape rectangle;

    // Setting rectangle values
    sf::Vector2f rectanglePosition = sf::Vector2f(850,450);
    rectangle.setPosition(rectanglePosition);
    rectangle.setSize(sf::Vector2f(100, 100));

    // Declaring x,y velocity
    float xVelocity = 3;
    float yVelocity = 3;

    while (window.isOpen()) {
        sf::Event event;

        /// INPUT

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close(); // Checks if the x button on the window is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        /// PHYSICS

        // Creating a bounding box so that the rectangle won't go outside the screen and instead makes it bounce back
        // PADDING is needed due to SFML2 using the (0,0) of the shape as a reference point
        // Adjusting the bounding box size by subtracting the shape's size makes it so the box won't leave the screen
        if (rectanglePosition.x < 0 || rectanglePosition.x > 1920 - PADDING) xVelocity *= -1;
        if (rectanglePosition.y < 0 || rectanglePosition.y > 1080 - PADDING) yVelocity *= -1;

        // Setting the velocity and updating the position
        rectanglePosition.x += xVelocity;
        rectanglePosition.y += yVelocity;
        rectangle.setPosition(rectanglePosition);

        /// RENDERING

        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}