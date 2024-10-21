#include "../headers/engine.hpp"

#include "SFML/System/Time.hpp"

const sf::Time Engine::TimePerFrame = sf::seconds(1.f/60.f);

Engine::Engine() {
    resolution = Vector2f(1920u,  1080u);
    window.create(VideoMode(resolution.x, resolution.y), "SFML Project", Style::Default);
    window.setFramerateLimit(FPS);
}

void Engine::run() {
    // Main loop - Runs until the window is closed
    while (window.isOpen()) {
        input();
        draw();
    }
}