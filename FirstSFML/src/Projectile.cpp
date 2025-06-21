#include "Projectile.h"

Projectile::Projectile(sf::Texture* _texture, sf::Vector2f _origin, float dirX, float dirY, float _movementSpeed)
	: sprite(*_texture), origin(_origin), direction({ dirX, dirY }), movementSpeed(_movementSpeed)
{
	initSprite();
	sprite.setPosition(origin);
}

Projectile::~Projectile()
{

}

// Functions.
void Projectile::update()
{
	sprite.move(direction * movementSpeed);
}

void Projectile::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}

void Projectile::initSprite()
{
	sprite.scale({ 0.05f, 0.05f });
}

const sf::FloatRect Projectile::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}