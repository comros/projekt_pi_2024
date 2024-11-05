#include "../headers/game.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

Game::Game()
{
    mWindow.setFramerateLimit(60);

    // ImGui init, I cast it to void to get rid of the unused result warning
    (void)ImGui::SFML::Init(mWindow);

    // Load terrain texture once
    mTTerrain.loadFromFile(TERRAIN_ATLAS);
    mTTerrain.setSmooth(false); // Prevents texture bleeding
    // Generate map vertex array once
    generateMap();
}

Game::~Game()
{
    // Shutdown ImGui in the rendering thread before exiting
    ImGui::SFML::Shutdown();
}



// It always should run: Input -> Physics -> Render
void Game::run() {
    while (mWindow.isOpen()) {
        // Calculating deltaTime for fps independency
        float deltaTime = mClock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

// GUI events has to be in the main thread for best portability
void Game::processEvents() {
    sf::Event event{};
    while (mWindow.pollEvent(event) && mWindow.hasFocus()) {
        ImGui::SFML::ProcessEvent(mWindow, event);
        mInputHandler.handleEvent(event, mWindow, mPlayer);
    }
}

void Game::update(float deltaTime)
{
    if(mWindow.hasFocus()) mPlayer.setDirection(InputHandler::getPlayerDirection());

    mPlayer.updatePosition(deltaTime);
    mPlayer.updateCamera(mWindow);
}

void Game::generateMap() {
    mMapVertices.setPrimitiveType(sf::Quads);
    mMapVertices.resize(40 * 12 * 4);  // 4 vertices per tile

    for (int x = 0; x < 40; ++x) {
        for (int y = 0; y < 12; ++y) {
            constexpr int tileSize = 16;
            const int tileType = mTiles[y][x];

            // I added casting to float to get rid of warnings in the IDE
            const auto xf = static_cast<float>(x);
            const auto yf = static_cast<float>(y);

            // Calculate texture position
            const int tx = (tileType % 4) * tileSize;
            const int ty = (tileType / 4) * tileSize;

            // Get pointer to the quad for this tile
            sf::Vertex* quad = &mMapVertices[(x + y * 40) * 4];

            // Define the quad's 4 corners
            quad[0].position = sf::Vector2f(xf * tileSize + WINDOW_WIDTH / 3, yf * tileSize + WINDOW_HEIGHT / 2 - 64);
            quad[1].position = sf::Vector2f((xf + 1) * tileSize + WINDOW_WIDTH / 3, yf * tileSize + WINDOW_HEIGHT / 2 - 64);
            quad[2].position = sf::Vector2f((xf + 1) * tileSize + WINDOW_WIDTH / 3, (yf + 1) * tileSize + WINDOW_HEIGHT / 2 - 64);
            quad[3].position = sf::Vector2f(xf * tileSize + WINDOW_WIDTH / 3, (yf + 1) * tileSize + WINDOW_HEIGHT / 2 - 64);

            constexpr int padding = 1;  // Adjust texture coordinates by 1 pixel inward

            const auto txf = static_cast<float>(tx);
            const auto tyf = static_cast<float>(ty);

            // Set texture coordinates
            quad[0].texCoords = sf::Vector2f(txf + padding, tyf + padding);
            quad[1].texCoords = sf::Vector2f(txf + tileSize - padding, tyf + padding);
            quad[2].texCoords = sf::Vector2f(txf + tileSize - padding, tyf + tileSize - padding);
            quad[3].texCoords = sf::Vector2f(txf + padding, tyf + tileSize - padding);
        }
    }
}

void Game::render() {

    // DeltaClock needed for ImGui
    const sf::Clock deltaClock;

    // Clear and render the window contents
    mWindow.clear(sf::Color(19,141,89,255));
    mWindow.draw(mMapVertices, &mTTerrain); // Draw map as a single vertex array
    mWindow.draw(mPlayer.getSprite());

    // ImGui rendering
    imgui(deltaClock, mPlayer);

    mWindow.display();
}

void Game::imgui(sf::Clock deltaClock, Player& player)
{
    sf::Text text;
    float playerSpeed = player.getSpeed();
    ImGui::SFML::Update(mWindow, deltaClock.restart());

    ImGui::Begin("Player");

    ImGui::Text("x: %g, y: %g", player.getPosition().x, player.getPosition().y);

    ImGui::SliderFloat("Speed",&playerSpeed, 0.0f, 300.0f);
    player.setSpeed(playerSpeed);

    ImGui::End();

    ImGui::SFML::Render(mWindow);
}
