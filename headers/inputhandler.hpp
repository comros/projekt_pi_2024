#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <SFML/Graphics.hpp>
#include "player.hpp"

class InputHandler {
public:
    InputHandler();

    // Call this in Game::processEvents to handle events
    void handleEvent(const sf::Event& event, sf::RenderWindow& window, Player &player);

    // Call this in Game::update to set player movement direction based on keyboard input
    static sf::Vector2f getPlayerDirection();

private:

};

#endif