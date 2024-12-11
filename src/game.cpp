#include "../headers/game.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>




Game::Game() : mInventory(4, 9), mWorldGen(512, 512, std::random_device{}()),
objectManager(mWorldGen)

{
    mWindow.setFramerateLimit(144);
    mWindow.setKeyRepeatEnabled(false); // Disable key repeat for F11 fullscreen toggle

    // Set the initial aspect ratio for the view
    mWindow.setView(sf::View(sf::FloatRect(0.f, 0.f, 16.f * 32.f, 16.f * 32.f))); // 16x16 tile size view

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
  
    objectManager.loadTextures();
    objectManager.spawnObjects({512, 512}); // Spawn objects on valid tiles

    float playerClearanceRadius = 32.0f; // Minimum distance from objects
    sf::Vector2f playerSpawn = objectManager.findValidPlayerSpawn({512, 512}, playerClearanceRadius);
    mPlayer.setPosition(playerSpawn);

}

Game::~Game()
{
    ImGui::SFML::Shutdown();
}

// It always should run: Input -> Physics -> Render
void Game::run()
{
    while (mWindow.isOpen())
    {
        // Calculating deltaTime for fps independency
        const float deltaTime = mClock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render(deltaTime);
    }

}

void Game::toggleFullscreen() {
    static bool isFullscreen = false;

    if (isFullscreen) {
        // Switch to windowed mode
        mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Projekt pi 2024", sf::Style::Default);
    } else {
        // Switch to fullscreen mode
        mWindow.create(sf::VideoMode::getDesktopMode(), "Projekt pi 2024", sf::Style::Fullscreen);
    }

    // Toggle the fullscreen flag
    isFullscreen = !isFullscreen;
}

void Game::processEvents() {
    sf::Event event{};
    while (mWindow.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(mWindow, event);

        mInputHandler.handleEvent(event, mWindow, mPlayer, objectManager, mInventory);

        // Handle the fullscreen toggle with F11
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11) {
            toggleFullscreen();
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

    mPlayer.updatePosition(deltaTime, objectManager.getObjects());
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
    objectManager.adjustObjectBrightness(brightness);
    mPlayer.adjustPlayerBrightness(brightness);
}

bool Game::isVisibleInView(const sf::FloatRect& objectBounds, const sf::View& view) {
    sf::FloatRect viewBounds(
        view.getCenter() - view.getSize() / 2.0f,
        view.getSize()
    );
    return viewBounds.intersects(objectBounds);
}

void Game::render(float deltaTime) {
    // Clear and render the window contents
    mWindow.clear(sf::Color(0, 0, 0, 255));

    // Get the current camera view
    sf::View view = mWindow.getView();

    // Render world tiles
    mWorldGen.render(mWindow);

    // Render visible rocks and bushes
    objectManager.renderRocks(mWindow);
    objectManager.renderBushes(mWindow);

    // Render trees behind the player if visible
    for (const auto& object : objectManager.getObjects()) { // Use const reference to the shared_ptr
        Tree* tree = dynamic_cast<Tree*>(object.get());
        if (tree) {
            sf::FloatRect treeBounds = tree->getSprite().getGlobalBounds();
            if (isVisibleInView(treeBounds, view) && !tree->isInUpperHalfOfInteractionRange(mPlayer.getPosition())) {
                tree->adjustAlpha(1.f);
                mWindow.draw(tree->getSprite());
            }
        }
    }

    // Render the player sprite
    mWindow.draw(mPlayer.getSprite());

    // Render trees on top of the player if visible
    for (const auto& object : objectManager.getObjects()) { // Use const reference to the shared_ptr
        Tree* tree = dynamic_cast<Tree*>(object.get());
        if (tree) {
            sf::FloatRect treeBounds = tree->getSprite().getGlobalBounds();
            if (isVisibleInView(treeBounds, view) && tree->isInUpperHalfOfInteractionRange(mPlayer.getPosition())) {
                tree->adjustAlpha(0.5f);
                mWindow.draw(tree->getSprite());
            }
        }
    }

    // Optional: Render player's collision box
    // mPlayer.renderBounds(mWindow);


    // ImGui rendering
    ImGui::SFML::Update(mWindow, sf::seconds(deltaTime));
    imgui(deltaTime, mPlayer);
    mInventory.drawInventory(mWindow);
    mInventory.drawHotbarOnScreen(mWindow);
    ImGui::SFML::Render(mWindow);


    // Display the rendered frame
    mWindow.display();
}

void Game::imgui(const float deltaTime, Player& player)
{
    sf::Text text;
    float playerSpeed = player.getSpeed();
    static float effectsVolume = 100.0f;
    float musicVolume = backgroundMusic.getVolume() * 10;
    const float fps = 1.0f / deltaTime;
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

    if (ImGui::Button("Respawn")) {
        float playerClearanceRadius = 32.0f; // Minimum distance from objects
        sf::Vector2f playerSpawn = objectManager.findValidPlayerSpawn({512,512}, playerClearanceRadius);
        player.setPosition(playerSpawn);
    }
    ImGui::End();


    ImGui::Begin("World Generation");


    // Sliders for controlling the seeds
    ImGui::Text("Seeds:");
    ImGui::InputInt("Terrain Seed", &mWorldGen.mTerrainSeed);

    ImGui::Text("Objects:");
    ImGui::InputInt("Amount of objects", &objectManager.amountOfObjects);

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
        objectManager.clearObjects(); // Clear existing objects
        objectManager.spawnObjects({512, 512}); // Spawn new objects
        float playerClearanceRadius = 32.0f; // Minimum distance from objects
        sf::Vector2f playerSpawn = objectManager.findValidPlayerSpawn({512,512}, playerClearanceRadius);
        player.setPosition(playerSpawn);
    }


    if (ImGui::Button("Regenerate Objects")) {
        objectManager.clearObjects(); // Clear existing objects
        objectManager.spawnObjects({512, 512}); // Spawn new objects
    }

    ImGui::End();

}