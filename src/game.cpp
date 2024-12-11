#include "../headers/game.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>



Game::Game() : mInventory(4, 9)
{
    mWindow.setFramerateLimit(144);

    // ImGui init, I cast it to void to get rid of the unused result warning
    (void)ImGui::SFML::Init(mWindow);

    // Load background music once
    backgroundMusic.openFromFile(BACKGROUND_MUSIC);
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
    backgroundMusic.setVolume(10);
    Item pickaxe("Pickaxe", PICKAXE);
    Item sword("Sword", SWORD);
    Item iron_ore("Iron_ore", IRONORE, 10);
    mInventory.addItem(sword, 0, 2);
    mInventory.addItem(pickaxe, 0, 1);
    mInventory.addItem(iron_ore, 0, 0);
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
        const float deltaTime = mClock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render(deltaTime);
    }

}

// GUI events has to be in the main thread for best portability
void Game::processEvents() {
    sf::Event event{};
    while (mWindow.pollEvent(event) && mWindow.hasFocus()) {
        ImGui::SFML::ProcessEvent(mWindow, event);
        mInputHandler.handleEvent(event, mWindow, mPlayer, mInventory);
    }

    // Pause the music when window lost focus
    if (event.type == sf::Event::LostFocus) {
        backgroundMusic.pause();
    }
    // Plays the music when window gain focus
    if (event.type == sf::Event::GainedFocus) {
        backgroundMusic.play();
    }


}

void Game::update(const float deltaTime)
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

void Game::render(float deltaTime) {

    // Clear and render the window contents
    mWindow.clear(sf::Color(19,141,89,255));
    mWindow.draw(mMapVertices, &mTTerrain); // Draw map as a single vertex array
    mWindow.draw(mPlayer.getSprite());


    // ImGui rendering
    ImGui::SFML::Update(mWindow, sf::seconds(deltaTime));
    imgui(deltaTime, mPlayer);
    mInventory.drawInventory(mWindow);
    mInventory.drawHotbarOnScreen(mWindow);
    ImGui::SFML::Render(mWindow);


    mWindow.display();
}

void Game::imgui(const float deltaTime, Player& player)
{
    sf::Text text;
    float playerSpeed = player.getSpeed();
    static float effectsVolume = 100.0f;
    float musicVolume = backgroundMusic.getVolume()*10;
    const float fps = 1.0f / deltaTime;
    ImGui::Begin("Player");

    ImGui::Text("FPS: %.0f", fps);

    ImGui::Text("x: %g, y: %g", player.getPosition().x, player.getPosition().y);

    ImGui::SliderFloat("Speed",&playerSpeed, 0.0f, 300.0f);
    player.setSpeed(playerSpeed);

    if (ImGui::SliderFloat("Effects Volume", &effectsVolume, 0.0f, 200.0f)) {
        player.setEffectsVolume(effectsVolume); // Sets global volume in player
    }

    if (ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 200.0f)) {
        backgroundMusic.setVolume(musicVolume/10); // Sets global volume in player
    }
    ImGui::End();

}

