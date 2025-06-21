#include "Game.h"

Game::Game() : scoreText(font), worldBackground(worldBackgroundTex), gameOverText(font)
{
	initTextures();
	initWindow();
	initWorld();
	initGUI();
	initPlayer();
	initEnemies();
}

Game::~Game()
{
	delete window;
	delete player;

	for (auto& texture : textures)
	{
		delete texture.second;
	}

	for (auto& projectile : projectiles)
	{
		delete projectile;
	}

	for (auto& enemy : enemies)
	{
		delete enemy;
	}
}

// Functions.
void Game::run()
{
	while (window->isOpen())
	{
		updatePollEvents();
		if (player->getHealthRatio() > 0.f)
		{
			update();
		}
		render();
	}
}

void Game::updatePollEvents()
{
	while (const std::optional<sf::Event> event = window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window->close();
		}
		else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
		{
			if (key->scancode == sf::Keyboard::Scancode::Escape)
			{
				window->close();
			}
		}
	}
}

void Game::updatePlayerCollision()
{
	// Bottom collision with the window.
	if (player->getPosition().y > window->getSize().y - player->getSize().y / 2)
	{
		player->setPosition({ player->getPosition().x, (float)(window->getSize().y - player->getSize().y / 2) });
	}

	// Top collision with the window.
	if (player->getPosition().y < -(player->getSize().y / 2))
	{
		player->setPosition({ player->getPosition().x, -(float)(player->getSize().y / 2) });
	}

	// Left collision with the window.
	if (player->getPosition().x < -(player->getSize().x / 2))
	{
		player->setPosition({ (float)(window->getSize().x - player->getSize().y / 2), player->getPosition().y });
	}

	// Right collision with the window.
	if (player->getPosition().x > window->getSize().x - player->getSize().y / 2)
	{
		player->setPosition({ -(player->getSize().y / 2), player->getPosition().y });
	}
}

void Game::updateInput()
{
	// Player movement input handling.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		player->move(-1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		player->move(1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		player->move(0.f, -1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		player->move(0.f, 1.f);
	}

	// Player shooting input handling.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && player->canAttack())
	{
		sf::Vector2f playerPos = player->getPosition();
		playerPos.x += player->getSize().x / 3.25f;
		projectiles.push_back(new Projectile(textures["PROJECTILE.BULLET"], playerPos, 0.f, -1.f));
	}
}

void Game::updateProjectiles()
{
	size_t counter = 0;
	for (auto& projectile : projectiles)
	{
		projectile->update();
		if (projectile->getGlobalBounds().position.y + projectile->getGlobalBounds().size.y < 0.f)
		{
			delete projectiles.at(counter);
			projectiles.erase(projectiles.begin() + counter);
			--counter;
		}
		++counter;
	}
}

void Game::updateEnemiesAndCollisionBullets()
{
	enemySpawnTimer += .5f;
	if (enemySpawnTimer >= enemySpawnTimerMax)
	{
		enemies.push_back(new Enemy({ 
			helper::getRandomInt(100, window->getSize().x - 200), -100 }));
		enemySpawnTimer = 0.f;
	}

	bool wasDeleted = false;
	size_t counter = 0;
	for (auto& enemy : enemies)
	{
		enemy->update();

		// Check if the enemy is hit by a projectile.
		for (int i = 0; i < projectiles.size(); ++i)
		{
			if (enemy->getGlobalBounds().findIntersection(projectiles[i]->getGlobalBounds()))
			{
				delete projectiles[i];
				projectiles.erase(projectiles.begin() + i);

				if (enemy->getHit())
				{
					score += enemy->getPoints();
					delete enemy;
					enemies.erase(enemies.begin() + counter);
					wasDeleted = true;
					--counter;
					break;
				}
			}
		}

		// Check if the enemy is out of bounds.
		if (!wasDeleted && enemy->getGlobalBounds().position.y > window->getSize().y)
		{
			delete enemies.at(counter);
			enemies.erase(enemies.begin() + counter);
			--counter;
		}

		// Check if the enemy collides with the player.
		if (!wasDeleted && enemy->getGlobalBounds().findIntersection(player->getGlobalBounds()))
		{
			if (enemy->getHit())
			{
				if (player->getHit(enemy->getDamage()))
				{
					break;
				}
				healthBarWidth = healthBarWidthMax * player->getHealthRatio();
				score += enemy->getPoints();
				delete enemies.at(counter);
				enemies.erase(enemies.begin() + counter);
				--counter;
			}
			else
			{
				if (player->getHit(enemy->getDamage()))
				{
					break;
				}
			}
		}

		wasDeleted = false;
		++counter;
	}
}

void Game::update()
{
	updateInput();
	player->update();
	updatePlayerCollision();
	updateEnemiesAndCollisionBullets();
	updateProjectiles();
	updateGUI();
}

void Game::updateGUI()
{
	// Update score text.
	std::stringstream ss;
	ss << "Score: " << score;
	scoreText.setString(ss.str());

	// Update health bar.
	if (healthBar.getSize().x > healthBarWidth)
	{
		healthBar.setSize({ healthBar.getSize().x - 1.f, healthBar.getSize().y });
	}
}

void Game::render()
{
	window->clear(sf::Color::Black);
	renderWorld();
	player->render(*window);
	
	for (auto& enemy : enemies)
	{
		enemy->render(*window);
	}
	
	for (auto& projectile : projectiles)
	{
		projectile->render(*window);
	}

	renderGUI();
	window->display();
}

void Game::renderGUI()
{
	window->draw(scoreText);
	window->draw(healthBarBack);
	window->draw(healthBar);

	if (player->getHealthRatio() <= 0.f)
	{
		window->draw(gameOverText);
	}
}

void Game::renderWorld()
{
	window->draw(worldBackground);
}

// Private functions.
void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode({ 800, 600 }), "First Game w/ SFML", 
		sf::Style::Close | sf::Style::Titlebar);
	window->setFramerateLimit(144);
	window->setVerticalSyncEnabled(false);
}

void Game::initPlayer()
{
	this->score = 0;
	this->player = new PlayerCharacter();
	float pX = (window->getSize().x / 2.f) - player->getSize().x / 2;
	float pY = (float)(window->getSize().y - player->getSize().y);
	player->sprite.setPosition({ pX, pY });
}

void Game::initEnemies()
{
	this->enemySpawnTimerMax = 100.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
}

void Game::initTextures()
{
	textures["PROJECTILE.BULLET"] = new sf::Texture();
	if (!textures["PROJECTILE.BULLET"]->loadFromFile("../Resources/Textures/projectile.png"))
	{
		std::cout << "Failed to load texture: PROJECTILE.BULLET\n";
	}
}

void Game::initGUI()
{
	this->healthBarWidthMax = 250.f;
	this->healthBarWidth = this->healthBarWidthMax;
	if (!font.openFromFile("../Resources/Fonts/NewRocker-Regular.ttf"))
	{
		std::cout << "Failed to load font: NewRocker-Regular.ttf\n";
	}

	scoreText.setFont(font);
	scoreText.setCharacterSize(28);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setString("Score: 0");
	scoreText.setPosition({ 10.f, 30.f });

	gameOverText.setFont(font);
	gameOverText.setCharacterSize(60);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setString("Game Over!");
	gameOverText.setPosition({
		(window->getSize().x / 2) - (gameOverText.getGlobalBounds().size.x / 2.f),
		(window->getSize().y / 2) - (gameOverText.getGlobalBounds().size.y / 2.f)
	});

	healthBar.setSize({ healthBarWidthMax, 15.f });
	healthBar.setFillColor(sf::Color::Red);
	healthBar.setPosition({ 10.f, 10.f });

	healthBarBack.setSize({ healthBarWidthMax, 15.f });
	healthBarBack.setFillColor(sf::Color::White);
	healthBarBack.setPosition({ 10.f, 10.f });
}

void Game::initWorld()
{
	if (!worldBackgroundTex.loadFromFile("../Resources/Textures/back1.png"))
	{
		std::cout << "Failed to load texture: back1.png\n";
	}
	worldBackground = sf::Sprite(worldBackgroundTex);
}
