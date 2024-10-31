#include "../headers/Animation.h"

// Loading texture and starting to loop through frames
Animation::Animation(int x, int y, int width, int height, const std::string& texture_path)
{
    texture.loadFromFile(texture_path);
    for(int i = 0; i < numberOfFrames; i++)
    {
        frames[i] = { x + i * width, y, width, height };
    }
}

// Setting texture again, and applying correct frame
void Animation::ApplyToSprite(sf::Sprite& sprite) const
{
    sprite.setTexture(texture);
    sprite.setTextureRect(frames[iFrame]);
}

// Making animation independent of fps
void Animation::Update(float deltaTime)
{
    time += deltaTime;
    while (time >= frameDisplayTime)
    {
        time -= frameDisplayTime;
        ChangeFrame();
    }
}

// Checking if the next frame is null to mantain the loop
void Animation::ChangeFrame()
{
    if (++iFrame >= numberOfFrames)
    {
        iFrame = 0;
    }
}
