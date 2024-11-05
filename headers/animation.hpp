#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

class Animation {
public:
    Animation() = default; // is needed to do the trick with the "count" state
    Animation(int, int, int, int, const std::string&);
    void ApplyToSprite(sf::Sprite&) const;
    void Update(float deltaTime);
private:
    static constexpr int mNumberOfFrames = 8;
    static constexpr float mFrameDisplayTime = 0.1f;
    sf::Texture mTexture;
    sf::IntRect mFrames[mNumberOfFrames];
    int mIFrame = 0;
    float mTime = 0.0f;

    void ChangeFrame();
};



#endif //ANIMATION_H
