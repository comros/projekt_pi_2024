#include "../headers/character.hpp"

#define CHAR_ATLAS_PATH "../../assets/fantasy_txtpack/Player/Player.png"

Character::Character(const sf::Vector2f& position) : mPosition(position) {
    Character::mTexture.loadFromFile(CHAR_ATLAS_PATH);
    Character::mSprite = sf::Sprite(Character::mTexture);
    Character::mSprite.setTextureRect({0,0,64,64});
}

void Character::draw(sf::RenderTarget& renderTarget) const
{
    renderTarget.draw(Character::mSprite);
}

void Character::setDirection(const sf::Vector2f& direction)
{
    Character::mVelocity = direction * speed;
}

void Character::update(float deltaTime)
{
    Character::mPosition += Character::mVelocity * deltaTime;
    Character::mSprite.setPosition(Character::mPosition);
}