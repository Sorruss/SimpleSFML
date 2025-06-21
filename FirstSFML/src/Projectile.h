#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Projectile
{
private:
	sf::Sprite sprite;
	sf::Vector2f direction;
	sf::Vector2f origin;
	float movementSpeed;

private:
	void initSprite();

public:
	Projectile(sf::Texture* _texture, sf::Vector2f _origin, float dirX, float dirY, float _movementSpeed=10.f);
	virtual ~Projectile();

	void update();
	void render(sf::RenderTarget& target);
	const sf::FloatRect getGlobalBounds() const;
};
