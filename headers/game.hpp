#ifndef GAME_H
#define GAME_H

#include  <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "../headers/inputhandler.hpp"
#include "../headers/player.hpp"
#include "../headers/definitions.hpp"
#include "../headers/inventory.hpp"
#include "../headers/item.hpp"

class Game {
public:
    Game();
    ~Game();
    void run();


    Player getmPlayer() const { return mPlayer; }
private:
    void processEvents();
    void update(float deltaTime);
    void generateMap();
    void render(float deltaTime);
    void imgui(float deltaTime, Player& player); // ImGui

    Inventory mInventory;

    sf::Music backgroundMusic;


    sf::RenderWindow mWindow = { sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Projekt pi 2024", sf::Style::Fullscreen};
    sf::Clock mClock;              // Needed for deltaTime

    // Instances
    InputHandler mInputHandler;
    Player mPlayer;


    // Map generation

    sf::Texture mTTerrain;         // Terrain texture
    sf::VertexArray mMapVertices;  // Vertex array for the map

    int mTiles[12][40] = {
        {0,0,0,5,9,9,9,9,9,9,7,0,0,0,0,0,0,0,0,0,0,0,0,5,9,9,9,9,9,9,0,0,0,5,9,9,9,9,9,9},
        {9,9,9,6,1,1,1,1,1,1,8,9,9,7,0,0,0,0,0,0,9,9,9,6,1,1,1,1,1,1,9,9,9,6,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,8,9,9,9,9,9,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1},
        {1,1,1,2,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,4,4,4,4,4,1,1,1,1,1,1,1,1,1,4,3,3,3,1,1,4,4,4,4,4,1,1,1,1,1,4,4,4,4,4,1,1,1},
        {4,4,3,3,3,3,3,4,4,4,4,4,4,4,4,4,3,3,3,3,4,4,3,3,3,3,3,4,4,4,4,4,3,3,3,3,3,4,4,4},
        {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3}
        };

};



#endif //GAME_H
