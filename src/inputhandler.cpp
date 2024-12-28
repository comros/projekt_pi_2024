#include "../headers/InputHandler.hpp"



InputHandler::InputHandler() = default;


void InputHandler::handleEvent(const sf::Event& event, sf::RenderWindow& window, Player& player, ObjectManager& objectManager, InventoryManager& InventoryManager) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }

    // Camera zoom
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        event.type == sf::Event::MouseWheelMoved &&
        player.getZoomFactor() - 0.02f * static_cast<float>(event.mouseWheel.delta) > .02f &&
        player.getZoomFactor() - 0.02f * static_cast<float>(event.mouseWheel.delta) < 2) {
        player.setZoomFactor(0.02f * static_cast<float>(event.mouseWheel.delta));
        }

    // Camera lock/unlock
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
        player.setCameraLocked(true);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3)) {
        player.setCameraLocked(false);

    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I) {
        InventoryManager.toggleInventory(); // Wciśnięcie "I" otwiera/zamyka ekwipunek
    }

    for (int i = 0; i < 9; ++i) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::Num1 + i))) {
            InventoryManager.updateHotbarSelection(i);// Zmień aktywny slot
            break;
        }
    }
        // Handle object interactions
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos); // Convert to world coordinates

        // Check if any object was clicked and interact with it
        objectManager.handleObjectClick(worldPos, player.getPosition());
        }

}
sf::Vector2f InputHandler::getPlayerDirection() {
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
