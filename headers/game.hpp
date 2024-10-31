#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "../headers/player.hpp"
#include "../headers/definitions.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow window;
    sf::Clock clock; // Needed for deltaTime
    Player player;
};



#endif //GAME_H
