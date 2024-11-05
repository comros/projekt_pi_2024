#include "../headers/animation.hpp"

// Loading texture and starting to loop through frames
Animation::Animation(int x, int y, int width, int height, const std::string& texture_path)
{
    mTexture.loadFromFile(texture_path);
    for(int i = 0; i < mNumberOfFrames; i++)
    {
        mFrames[i] = { x + i * width, y, width, height };
    }
}

// Setting texture again, and applying correct frame
void Animation::ApplyToSprite(sf::Sprite& sprite) const
{
    sprite.setTexture(mTexture);
    sprite.setTextureRect(mFrames[mIFrame]);
}

// Making animation independent of fps
void Animation::Update(float deltaTime)
{
    mTime += deltaTime;
    while (mTime >= mFrameDisplayTime)
    {
        mTime -= mFrameDisplayTime;
        ChangeFrame();
    }
}

// Checking if the next frame is null to maintain the loop
void Animation::ChangeFrame()
{
    if (++mIFrame >= mNumberOfFrames)
    {
        mIFrame = 0;
    }
}
