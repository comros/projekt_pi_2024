#ifndef GAME_H
#define GAME_H

#include  <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "../headers/inputhandler.hpp"
#include "../headers/player.hpp"
#include "../headers/definitions.hpp"
#include "../headers/worldgen.hpp"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    WorldGen mWorldGen;
    void processEvents();
    void update(float deltaTime);
    void generateMap();
    void render(float deltaTime);
    void imgui(float deltaTime, Player& player); // ImGui


    sf::Music backgroundMusic;


    sf::RenderWindow mWindow = { sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Projekt pi 2024", sf::Style::Default};
    sf::Clock mClock;              // Needed for deltaTime

    // Instances
    InputHandler mInputHandler;
    Player mPlayer;

};



#endif //GAME_H
