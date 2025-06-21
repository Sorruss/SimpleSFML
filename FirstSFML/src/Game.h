#pragma once

#include "PlayerCharacter.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Helper.h"
#include <map>
#include <sstream>

class Game
{
private:
	// Window.
	sf::RenderWindow* window;

	// World.
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;

	// GUI.
	sf::Font font;
	sf::Text scoreText;
	sf::Text gameOverText;
	sf::RectangleShape healthBar;
	sf::RectangleShape healthBarBack;
	float healthBarWidthMax;
	float healthBarWidth;

	// Player.
	PlayerCharacter* player;
	size_t score;
	
	// Enemies.
	std::vector<Enemy*> enemies;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	// Resources.
	std::map<std::string, sf::Texture*> textures;
	std::vector<Projectile*> projectiles;

private:
	void initWindow();
	void initWorld();
	void initGUI();
	void initPlayer();
	void initEnemies();
	void initTextures();
	
public:
	Game();
	virtual ~Game();
	
	void run();
	void update();
	void updateGUI();
	
	void render();
	void renderGUI();
	void renderWorld();

	void updatePollEvents();
	void updatePlayerCollision();
	void updateEnemiesAndCollisionBullets();
	void updateInput();
	void updateProjectiles();
};
