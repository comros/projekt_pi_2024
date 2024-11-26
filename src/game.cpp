#include "../headers/game.hpp"
#include "imgui.h"
#include "imgui-SFML.h"


Game::Game()
: mWorldGen(512, 512, std::random_device{}())
{
    mWindow.setFramerateLimit(144);

    // ImGui init, I cast it to void to get rid of the unused result warning
    (void)ImGui::SFML::Init(mWindow);

    // Load background music once
    backgroundMusic.openFromFile(BACKGROUND_MUSIC);
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
    backgroundMusic.setVolume(0);
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


void Game::render(float deltaTime) {

    // Clear and render the window contents
    mWindow.clear(sf::Color(255,255,255,255));

    mWorldGen.render(mWindow);

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
    ImGui::Text("x: %g, y: %g", player.getPosition().x, player.getPosition().y);

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


