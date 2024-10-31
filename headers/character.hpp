#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp>

class Character {
public:
  Character(const sf::Vector2f&);
  void draw(sf::RenderTarget&) const;
  void setDirection(const sf::Vector2f&);
  void update(float);
private:
  static constexpr float speed = 100.0f;
  sf::Vector2f mPosition;
  sf::Vector2f mVelocity = {0.0f, 0.0f};
  sf::Texture mTexture;
  sf::Sprite mSprite;
};

#endif //CHARACTER_HPP
