#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include "../headers/definitions.hpp"

class Animation {
public:
    Animation() = default; // is needed to do the trick with the "count" state
    Animation(int, int, int, int, const std::string&);
    void ApplyToSprite(sf::Sprite&) const;
    void Update(float deltaTime);
private:
    static constexpr int numberOfFrames = 8;
    static constexpr float frameDisplayTime = 0.1f;
    sf::Texture texture;
    sf::IntRect frames[numberOfFrames];
    int iFrame = 0;
    float time = 0.0f;

    void ChangeFrame();
};



#endif //ANIMATION_H
