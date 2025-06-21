#include "Enemy.h"

Enemy::Enemy(sf::Vector2i location)
{
	initVariables();
	shape.setPosition((sf::Vector2f)location);
}

Enemy::~Enemy()
{
	
}

// Functions.
void Enemy::update()
{
	move({ 0.f, 1.f });
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
}

void Enemy::move(const sf::Vector2f& direction)
{
	shape.move(direction * movementSpeed);
}

bool Enemy::getHit()
{
	if (typeBase > 3)
	{
		updateVariables(typeBase - 1);
		return false;
	}

	return true;
}

// Accessors.
const sf::FloatRect Enemy::getGlobalBounds() const
{
	return shape.getGlobalBounds();
}

const size_t& Enemy::getPoints() const
{
	return points;
}

const size_t& Enemy::getDamage() const
{
	return damage;
}

// Private functions.
void Enemy::initVariables()
{
	this->damageBase = 10;
	this->healthBase = 20;
	this->pointsBase = 5;
	this->radiusBase = (float)helper::getRandomInt(20, 65);
	this->movementSpeedBase = 5.0f;

	this->typeBase = helper::getRandomInt(3, 6);
	this->shape.setPointCount(typeBase);
	sf::Color color = typeBase == 3 ? sf::Color::Blue : typeBase == 4 ?
		sf::Color::Blue : typeBase == 5 ? sf::Color::Magenta : sf::Color::Red;
	this->shape.setFillColor(color);
	this->movementSpeed = movementSpeedBase / typeBase;

	this->type = typeBase - 2;
	this->shape.setRadius(radiusBase);
	this->damage = damageBase * type;
	this->health = healthBase * type;
	this->points = pointsBase * type;
}

void Enemy::updateVariables(int newType)
{
	typeBase = newType;
	this->shape.setPointCount(typeBase);
	type = typeBase - 2;
	this->movementSpeed = movementSpeedBase / type;
	this->damage = damageBase * type;
	this->health = healthBase * type;
}
