#pragma once

#include <SFML/Graphics.hpp>
#include "helper.h"
#include <iostream>

class Enemy
{
private:
	size_t type;
	size_t typeBase;
	sf::CircleShape shape;

	size_t damageBase;
	size_t healthBase;
	size_t pointsBase;
	size_t radiusBase;
	float movementSpeedBase;

	size_t damage;
	size_t points;
	size_t health;
	float movementSpeed;

private:
	void initVariables();
	void updateVariables(int newType);

public:
	Enemy(sf::Vector2i location);
	virtual ~Enemy();

	// Accessors.
	const sf::FloatRect getGlobalBounds() const;
	const size_t& getPoints() const;
	const size_t& getDamage() const;

	void update();
	void render(sf::RenderTarget& target);

	void move(const sf::Vector2f& direction);
	// Returns true if the enemy is dead, false otherwise.
	bool getHit();
};
