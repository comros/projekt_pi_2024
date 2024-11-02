#include "../headers/InputHandler.hpp"

InputHandler::InputHandler() = default;

// Handle individual SFML events (e.g., closing the window, zoom, camera lock/unlock)
void InputHandler::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }

    // Camera zoom
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        event.type == sf::Event::MouseWheelMoved &&
        zoomFactor - 0.05f * event.mouseWheel.delta > 0 &&
        zoomFactor - 0.05f * event.mouseWheel.delta < 1) {
        zoomFactor -= 0.05f * event.mouseWheel.delta;
        }

    // Camera lock/unlock
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3)) {
        cameraLocked = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4)) {
        cameraLocked = true;
    }
}

// Determine the player’s movement direction based on keyboard input
sf::Vector2f InputHandler::getPlayerDirection() const {
    sf::Vector2f direction(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y != -1 ? direction.y = 1 : direction.y = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y != 1 ? direction.y = -1 : direction.y = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x != 1 ? direction.x = -1 : direction.x = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x != -1 ? direction.x = 1 : direction.x = 0;
    }

    return direction;
}

// Get the current zoom factor
float InputHandler::getZoomFactor() const {
    return zoomFactor;
}

// Check if the camera is locked
bool InputHandler::isCameraLocked() const {
    return cameraLocked;
}