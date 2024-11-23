#include "../headers/InputHandler.hpp"

InputHandler::InputHandler() = default;

// Handle individual SFML events (e.g., closing the window, zoom, camera lock/unlock)
void InputHandler::handleEvent(const sf::Event& event, sf::RenderWindow& window, Player &player) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }

    // Camera zoom
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        event.type == sf::Event::MouseWheelMoved &&
        player.getZoomFactor() - 0.05f * static_cast<float>(event.mouseWheel.delta) > 0 &&
        player.getZoomFactor() - 0.05f * static_cast<float>(event.mouseWheel.delta) < 5) {
        player.setZoomFactor(0.05f * static_cast<float>(event.mouseWheel.delta));
        }

    // Camera lock/unlock
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
        player.setCameraLocked(true);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3)) {
        player.setCameraLocked(false);
    }
}

// Determine the player’s movement direction based on keyboard input
sf::Vector2f InputHandler::getPlayerDirection()
{
    sf::Vector2f direction(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y != -1 ?   direction.y = 1 : direction.y = 0;
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