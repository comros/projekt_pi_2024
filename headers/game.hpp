#ifndef GAME_H
#define GAME_H

#include  <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "../headers/inputhandler.hpp"
#include "../headers/pausemenu.hpp"
#include "../headers/player.hpp"
#include "../headers/definitions.hpp"
#include "../headers/inventory.hpp"
#include "../headers/item.hpp"

#include "../headers/worldgen.hpp"
#include "../headers/ObjectManager.hpp"


class Game {
public:
    Game();
    ~Game();
    void run();
    bool isVisibleInView(const sf::FloatRect& objectBounds, const sf::View& view);


    Player getmPlayer() const { return mPlayer; }
private:
    void processEvents();
    void update(float deltaTime);
    void render(float deltaTime);
    void toggleFullscreen();
    void imgui(float deltaTime, Player& player); // ImGui

    Inventory mInventory;

    PauseMenu mMenu;

    sf::Music backgroundMusic;


    sf::RenderWindow mWindow = { sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Projekt pi 2024", sf::Style::Default};

    sf::Clock mClock;              // Needed for deltaTime

    InputHandler mInputHandler;
    Player mPlayer;

    WorldGen mWorldGen;
    ObjectManager objectManager;

    // Day and night cycle
    float mCurrentTime = 12.0f; // Start at 12:00 in-game time (midday)
    const float realDayDuration = 60.0f * 24.f; // 1 real minute = 1 in-game day
    const float inGameDayDuration = 24.0f; // 24 in-game hours in a day

    float calculateBrightness(float inGameTime) const {
        // Normalize in-game time to [0, 1] (0 = midnight, 0.5 = noon, 1 = next midnight)
        float normalizedTime = inGameTime / inGameDayDuration;


        // Use a sine wave to simulate brightness: peaks at noon (normalizedTime = 0.5)
        float brightness = 0.1f + 0.5f * std::sin(2 * M_PI * (normalizedTime - 0.25f)) + 0.6f;// Shift by 6 hours for correct phase
        return std::clamp(brightness, 0.1f, 1.1f); // Clamp brightness to prevent extremes
    }
};



#endif //GAME_H
