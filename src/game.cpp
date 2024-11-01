#include "../headers/game.hpp"

// Creating a window and capping fps
Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Projekt pi 2024"), player() {
    window.setFramerateLimit(60);
}

// It always should run: Input -> Physics -> Render
void Game::run() {
    while (window.isOpen()) {
        // Calculating deltaTime for fps independency
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close(); // Checks if the x button on the window is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
    }
}

void Game::update(float deltaTime) {
    player.handleInput();
    player.update(deltaTime);
}

void Game::render() {
    window.clear();
    player.draw(window);
    window.display();
}
