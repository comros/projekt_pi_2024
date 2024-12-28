#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <SFML/Graphics.hpp>
#include "player.hpp"

#include "inventory.hpp"
#include "InventoryManager.hpp"
#include "../headers/tile.hpp"
#include "ObjectManager.hpp"


class InputHandler {
public:
    InputHandler();

    // Call this in Game::processEvents to handle events

    void handleEvent(const sf::Event& event, sf::RenderWindow& window, Player& player, ObjectManager& objectManager, InventoryManager& InventoryManager);


    // Call this in Game::update to set player movement direction based on keyboard input
    static sf::Vector2f getPlayerDirection();

private:

};

#endif