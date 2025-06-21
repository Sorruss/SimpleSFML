#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

class PlayerCharacter
{
private:
	sf::Texture texture;
	float movementSpeed;
	float attackCooldown;
	float attackCooldownMax;
	size_t health;
	size_t healthMax;

public:
	sf::Sprite sprite;

private:
	void initVariables();
	void initTexture();
	void initSprite();

public:
	PlayerCharacter();
	virtual ~PlayerCharacter();

	// Accessors.
	const sf::Vector2f getPosition() const;
	const sf::Vector2f getSize() const;
	const bool canAttack();
	const sf::FloatRect getGlobalBounds() const;
	const float getHealthRatio() const;

	// Modifiers.
	void setPosition(const float x, const float y);
	void setPosition(const sf::Vector2f position);

	void update();
	void render(sf::RenderTarget& target);
	void move(float dirX, float dirY);
	// Return true if the player is dead, otherwise return false.
	bool getHit(size_t damage);
};
