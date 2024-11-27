#include "../headers/game.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

Game::Game()
: mWorldGen(512, 512, std::random_device{}()),
objectManager(mWorldGen)
{
    mWindow.setFramerateLimit(144);

    // ImGui init, I cast it to void to get rid of the unused result warning
    (void)ImGui::SFML::Init(mWindow);

    // Load background music once
    backgroundMusic.openFromFile(BACKGROUND_MUSIC);
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
    backgroundMusic.setVolume(0);

    objectManager.loadTextures();
    objectManager.spawnObjects({512, 512}); // Spawn objects on valid tiles
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
        mInputHandler.handleEvent(event, mWindow, mPlayer);

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Right) {
                handleTileClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
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

    // Update in-game time
    mCurrentTime += (deltaTime / realDayDuration) * inGameDayDuration; // Advance in-game time
    if (mCurrentTime >= inGameDayDuration) {
        mCurrentTime -= inGameDayDuration; // Reset to the next day
    }

    // Calculate brightness based on time
    float brightness = calculateBrightness(mCurrentTime);

    // Update tile brightness in the world
    mWorldGen.updateTileBrightness(brightness);
}


void Game::render(float deltaTime) {

    // Clear and render the window contents
    mWindow.clear(sf::Color(36,126,202,255));

    mWorldGen.render(mWindow);

    objectManager.renderObjects(mWindow);

    mWindow.draw(mPlayer.getSprite());

    // ImGui rendering
    imgui(deltaTime, mPlayer);

    mWindow.display();
}

void Game::imgui(const float deltaTime, Player& player)
{
    sf::Text text;
    float playerSpeed = player.getSpeed();
    static float effectsVolume = 100.0f;
    float musicVolume = backgroundMusic.getVolume() * 10;
    const float fps = 1.0f / deltaTime;
    ImGui::SFML::Update(mWindow, sf::seconds(deltaTime));

    ImGui::Begin("Player");

    ImGui::Text("FPS: %.0f", fps);
    ImGui::Text("x: %g, y: %g", player.getPosition().x / 16, player.getPosition().y / 16);

    // Player speed slider
    ImGui::SliderFloat("Speed", &playerSpeed, 0.0f, 1000.0f);
    player.setSpeed(playerSpeed);

    if (ImGui::SliderFloat("Effects Volume", &effectsVolume, 0.0f, 200.0f)) {
        player.setEffectsVolume(effectsVolume); // Sets global volume in player
    }

    if (ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 200.0f)) {
        backgroundMusic.setVolume(musicVolume / 10); // Sets global volume in player
    }

    ImGui::End();

    ImGui::Begin("World Generation");


    // Sliders for controlling the seeds
    ImGui::Text("Seeds:");
    ImGui::InputInt("Terrain Seed", &mWorldGen.mTerrainSeed);

    // Display in-game time
    int inGameHour = static_cast<int>(mCurrentTime);
    int inGameMinutes = static_cast<int>((mCurrentTime - inGameHour) * 60);
    ImGui::Text("In-Game Time: %02d:%02d", inGameHour, inGameMinutes);

    // Sliders for world generation parameters
    ImGui::Text("Noise Parameters:");
    ImGui::SliderFloat("Falloff Strength", &mWorldGen.falloffStrength, 1.0f, 10.0f);
    ImGui::SliderInt("Octaves", &mWorldGen.octaves, 1, 20);
    ImGui::SliderFloat("Frequency", &mWorldGen.frequency, 0.001f, 0.05f);

    // Threshold sliders for terrain types
    ImGui::Text("Thresholds:");
    ImGui::SliderFloat("Deep Water Threshold", &mWorldGen.thresholdDeepWater, 0.0f, 1.0f);
    ImGui::SliderFloat("Shallow Water Threshold", &mWorldGen.thresholdShallowWater, 0.0f, 1.0f);
    ImGui::SliderFloat("Sand Threshold", &mWorldGen.thresholdSand, 0.0f, 1.0f);
    ImGui::SliderFloat("Grass Threshold", &mWorldGen.thresholdGrass, 0.0f, 1.0f);

    // Regenerate the map if parameters are updated
    if (ImGui::Button("Regenerate Map")) {
        mWorldGen.generateMap();  // Trigger map regeneration with new parameters
    }

    ImGui::End();

    // Render ImGui content
    ImGui::SFML::Render(mWindow);
}

void Game::handleTileClick(int mouseX, int mouseY) {
    // Convert the mouse position (screen space) to world coordinates
    sf::Vector2i mousePos(mouseX, mouseY);
    sf::Vector2f worldPos = mWindow.mapPixelToCoords(mousePos);

    // Calculate the tile's grid position from the world position
    int tileX = static_cast<int>(worldPos.x) / 16;
    int tileY = static_cast<int>(worldPos.y) / 16;

    // Ensure the tile is within the bounds of the world (you can adjust this depending on the world size)
    if (tileX >= 0 && tileX < 512 && tileY >= 0 && tileY < 512) {
        // Get the clicked tile at the calculated position
        Tile& clickedTile = mWorldGen.getTileAt(tileX, tileY);

        // Check if the clicked tile is a water tile (DeepWater or ShallowWater)
        if (clickedTile.getType() == Tile::TileType::DeepWater ||
            clickedTile.getType() == Tile::TileType::ShallowWater) {
            std::cout << "Cannot place grass on water tile at (" << tileX << ", " << tileY << ").\n";
            return; // Return early if we can't place grass on water
        }

        // Now, check the neighboring tiles to ensure they can accept the grass tile
        std::vector<sf::Vector2i> directions = {
            {1, 0},  // Right
            {-1, 0}, // Left
            {0, 1},  // Down
            {0, -1}, // Up
            {1, 1},  // Bottom-right diagonal
            {1, -1}, // Top-right diagonal
            {-1, 1}, // Bottom-left diagonal
            {-1, -1} // Top-left diagonal
        };

        // Iterate over the directions to check the neighboring tiles
        for (const auto& dir : directions) {
            int neighborX = tileX + dir.x;
            int neighborY = tileY + dir.y;

            // Ensure the neighboring tile is within bounds
            if (neighborX >= 0 && neighborX < 512 && neighborY >= 0 && neighborY < 512) {
                // Get the neighboring tile
                Tile& neighborTile = mWorldGen.getTileAt(neighborX, neighborY);

                // Check if the neighboring tile is water (DeepWater or ShallowWater)
                if (neighborTile.getType() == Tile::TileType::DeepWater ||
                    neighborTile.getType() == Tile::TileType::ShallowWater) {
                    std::cout << "Cannot place grass due to neighboring water tile at ("
                              << neighborX << ", " << neighborY << ").\n";
                    return; // Return early if we can't place grass due to a neighboring water tile
                }

                // For Sand, check if the bitmask is not 15 (and we want to block placement if it's not 15)
                if (neighborTile.getType() == Tile::TileType::Sand &&
                    mWorldGen.calculateBitmask(neighborX, neighborY, neighborTile.getType()) != 15) {
                    std::cout << "Cannot place grass due to neighboring sand tile at ("
                              << neighborX << ", " << neighborY << ") with bitmask not 15.\n";
                    return; // Return early if we can't place grass due to a sand tile with bitmask != 15
                }
            }
        }

        // If no restrictions, place the grass tile
        // Set the clicked tile type to NormalGrass (this will change any tile's type to grass)
        clickedTile.setType(Tile::TileType::Grass);

        // Print tile information for debugging
        clickedTile.printInfo();

        // Calculate and set the bitmask for the clicked tile (reflecting the new type)
        clickedTile.setTextureByBitmask(mWorldGen.calculateBitmask(tileX, tileY, Tile::TileType::Grass));

        // Update neighboring tiles' bitmask only, without changing their types
        for (const auto& dir : directions) {
            int neighborX = tileX + dir.x;
            int neighborY = tileY + dir.y;

            // Ensure the neighboring tile is within bounds
            if (neighborX >= 0 && neighborX < 512 && neighborY >= 0 && neighborY < 512) {
                // Get the neighboring tile (no type change)
                Tile& neighborTile = mWorldGen.getTileAt(neighborX, neighborY);

                // Calculate and set the bitmask for the neighboring tile (without changing the type)
                neighborTile.setTextureByBitmask(mWorldGen.calculateBitmask(neighborX, neighborY, neighborTile.getType()));
            }
        }

    } else {
        std::cout << "Click is outside the world bounds.\n";
    }
}
