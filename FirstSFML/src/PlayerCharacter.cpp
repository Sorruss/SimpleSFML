#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter() : sprite(texture)
{
	initVariables();
	initTexture();
	initSprite();
}

PlayerCharacter::~PlayerCharacter()
{

}

// Functions.
void PlayerCharacter::update()
{
	if (attackCooldown < attackCooldownMax)
	{
		attackCooldown += 1.f;
	}
}

void PlayerCharacter::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}

void PlayerCharacter::move(const float dirX, const float dirY)
{
	sprite.move({ dirX * movementSpeed, dirY * movementSpeed });
}

// Accessors.

const sf::Vector2f PlayerCharacter::getPosition() const
{
	return sprite.getPosition();
}

const sf::Vector2f PlayerCharacter::getSize() const
{
	return sprite.getGlobalBounds().size;
}

const bool PlayerCharacter::canAttack()
{
	if (attackCooldown >= attackCooldownMax)
	{
		this->attackCooldown = 0.f;
		return true;
	}
	return false;
}

const sf::FloatRect PlayerCharacter::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

const float PlayerCharacter::getHealthRatio() const
{
	return (float)health / healthMax;
}

// Private functions.
void PlayerCharacter::initVariables()
{
	this->movementSpeed = 3.f;
	this->attackCooldownMax = 30.f;
	this->attackCooldown = attackCooldownMax;
	this->healthMax = 200.f;
	this->health = healthMax;
}

void PlayerCharacter::initTexture()
{
	if (!texture.loadFromFile("../Resources/Textures/spacecraft.png"))
	{
		std::cout << "ERROR::PLAYERCHARACTER::INITTEXTURE::Could not load texture file.\n";
	}
}

void PlayerCharacter::initSprite()
{
	this->sprite = sf::Sprite(texture);
	sprite.scale({ 0.2f, 0.2f });
}

void PlayerCharacter::setPosition(const float x, const float y)
{
	sprite.setPosition({ x, y });
}

void PlayerCharacter::setPosition(const sf::Vector2f position)
{
	sprite.setPosition(position);
}

bool PlayerCharacter::getHit(size_t damage)
{
	health -= damage;
	if (health <= 0.f)
	{
		health = 0.f;
		return true;
	}

	return false;
}
