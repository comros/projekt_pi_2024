#include "../headers/game.hpp"
#include <iostream>

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
        inputHandler.handleEvent(event, window);
    }
}

void Game::update(float deltaTime) {

    // Update player direction based on input
    player.setDirection(inputHandler.getPlayerDirection());

    // Update player position, etc.
    player.update(deltaTime);

    // Set zoom factor from input
    zoomFactor = inputHandler.getZoomFactor();
}

void Game::generateMap()
{
    sf::Sprite tileSprite;
    sf::Texture tTerrain;
    tTerrain.loadFromFile(TERRAIN_ATLAS);
    tileSprite.setTexture(tTerrain);
    int mapWidth = 40;
    int mapHeight = 12;
    int tileSize = 16;
    for (int x = 0; x < mapWidth; ++x) {
        for (int y = 0; y < mapHeight; ++y) {
            int tileType = tiles[y][x];

            // Set texture rect based on tile type
            int tx = (tileType % 4) * tileSize; // Assume 4 tiles per row in the atlas
            int ty = (tileType / 4) * tileSize;

            tileSprite.setTextureRect(sf::IntRect(tx, ty, tileSize, tileSize));
            tileSprite.setPosition(static_cast<float>(x * tileSize)+WINDOW_WIDTH/3, static_cast<float>(y * tileSize)+WINDOW_HEIGHT/2-64); // Position based on column (x) and row (y)

            window.draw(tileSprite);
        }
    }
}


void Game::render() {
    window.clear(sf::Color(19,141,89,255));
    generateMap();
    player.draw(window, zoomFactor);
    window.display();
}
