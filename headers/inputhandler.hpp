#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <SFML/Graphics.hpp>
#include "player.hpp"

class InputHandler {
public:
    InputHandler();

    // Call this in Game::processEvents to handle events
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

    // Call this in Game::update to set player movement direction based on keyboard input
    sf::Vector2f getPlayerDirection() const;

    // For camera zoom control
    float getZoomFactor() const;

    // Check if the camera is locked/unlocked
    bool isCameraLocked() const;

private:
    sf::Vector2f playerDirection = {0,0};
    float zoomFactor = 0.225f;
    bool cameraLocked = true;
};

#endif