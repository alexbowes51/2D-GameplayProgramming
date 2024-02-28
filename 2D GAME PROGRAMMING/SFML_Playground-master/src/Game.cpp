//by:Alex Bowes 
#include "Game.h"
#include <iostream>
#include <iomanip> // header for std::Fixed + std::setprecision
	

// Our target FPS
static double const FPS{ 60.0f };

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default)
	,m_tank(m_holder, m_wallSprites)
{
	int currentLevel = 1;

	try {
		LevelLoader::load(currentLevel, m_level);
	}
	catch (std::exception& e) {
		std::cout << "Level Loading Failed" << std::endl;
		std::cout << e.what() << std::endl;
		throw e;
	}

	init();
	generateWalls();
	setupText();
	//setupenemys();
}

////////////////////////////////////////////////////////////
void Game::init()
{
	// Really only necessary is our target FPS is greate than 60.
	m_window.setVerticalSyncEnabled(true);

	if (!m_arialFont.loadFromFile("resources/BebasNeue.otf"))
	{
		std::cout << "Error loading font file";
	}

	m_holder.acquire("background", thor::Resources::fromFile<sf::Texture>("resources/Images/Background.jpg"));

	sf::Texture& bgtexture = m_holder["background"];
	m_bgSprite.setTexture(bgtexture);

	m_tank.setPosition(m_level.m_tank.m_position);
	m_tank.setScale(m_level.m_tank.m_scale);

	for (auto& projectile : m_ProjectileSprites) {
		for (auto& projectileData : m_level.m_projectiles) {
			projectile.setPosition(projectileData.m_position);
		}
	}

	/*for (auto& enemysData : m_level.m_enemies_tanks) {
		for (auto& enemy : m_EnemySprites) {
			enemy.setPosition(enemysData.m_position);
		}
	}*/
	

	//m_tankBase.setRotation(90.0f); // rotates it 90 degrees clockwise on its origin 
	//m_tankBase.rotate(-90.0f); // rotates it -90 degrees anti-clockwise on its origin 
	//m_tankBase.setOrigin(246.0f, 105.0f); //the sprite is being drawn from position 100,100 with an origin of 0,0 when you put the origin to the centre of the Sprite the centre of the sprite is then placed on position 100,100 causing it to be displayed off screen 

#ifdef TEST_FPS
	x_updateFPS.setFont(m_arialFont);
	x_updateFPS.setPosition(20, 300);
	x_updateFPS.setCharacterSize(24);
	x_updateFPS.setFillColor(sf::Color::White);
	x_drawFPS.setFont(m_arialFont);
	x_drawFPS.setPosition(20, 350);
	x_drawFPS.setCharacterSize(24);
	x_drawFPS.setFillColor(sf::Color::White);
#endif

	//m_holder.release("tankAtkas"); // it throws an exception in Resourceholder.inl
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	sf::Time timePerFrame = sf::seconds(1.0f / FPS); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			
				update(timePerFrame.asMilliseconds()); //60 fps
			
#ifdef TEST_FPS
			x_secondTime += timePerFrame;
			x_updateFrameCount++;
			if (x_secondTime.asSeconds() > 1)
			{
				std::string updatesPS = "UPS " + std::to_string(x_updateFrameCount - 1);
				x_updateFPS.setString(updatesPS);
				std::string drawsPS = "DPS " + std::to_string(x_drawFrameCount);
				x_drawFPS.setString(drawsPS);
				x_updateFrameCount = 0;
				x_drawFrameCount = 0;
				x_secondTime = sf::Time::Zero;
			}
#endif
		}
		render(); // as many as possible
#ifdef TEST_FPS
		x_drawFrameCount++;
#endif
	}
}

////////////////////////////////////////////////////////////
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

////////////////////////////////////////////////////////////
void Game::processGameEvents(sf::Event& event)
{
	// check if the event is a a mouse button release
	if (sf::Event::KeyPressed == event.type)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			m_window.close();
			break;
		default:
			break;
		}
	}
}

void Game::generateWalls()
{
	sf::Texture& m_texture = m_holder["tankAtlas"];

	sf::IntRect wallRect(230, 116, 22, 32);

	for (auto const& obstacle : m_level.m_obstacles) {
		sf::Sprite sprite;
		sprite.setTexture(m_texture);
		sprite.setTextureRect(wallRect);
		sprite.setOrigin(wallRect.width / 2.0, wallRect.height / 2.0);
		sprite.setPosition(obstacle.m_position);
		sprite.setRotation(obstacle.m_rotation);
		m_wallSprites.push_back(sprite);
	}
}

void Game::setupText()
{
	m_timer.setFont(m_arialFont);
	m_timer.setCharacterSize(50U);
	m_timer.setPosition(1150.0f, 25.0f);
	m_timer.setFillColor(sf::Color::White);
	m_timer.setString("Time : " + std::to_string(m_time));

	m_Hits.setFont(m_arialFont);
	m_Hits.setCharacterSize(50U);
	m_Hits.setPosition(1155.0f, 75.0f);
	m_Hits.setFillColor(sf::Color::White);
	m_Hits.setString("Hits : " + std::to_string(m_tank.m_hits));

	m_GameOver.setFont(m_arialFont);
	m_GameOver.setCharacterSize(100U);
	m_GameOver.setPosition(400.0f, 350.0f);
	m_GameOver.setFillColor(sf::Color::White);
	m_GameOver.setString("GameOver");;

	m_Accuracy.setFont(m_arialFont);
	m_Accuracy.setCharacterSize(75U);
	m_Accuracy.setPosition(400.0f, 450.0f);
	m_Accuracy.setFillColor(sf::Color::White);

}

void Game::updateTimer()
{
	float FrameRateMiliseconds = 16.67; //60 fps in miliseconds
	m_time -= FrameRateMiliseconds / 1000.0; // Convert miliseconds to seconds

	if (m_time <= 0) {
		m_time = 0;
		m_game_over = true;
	}

	std::stringstream m_TimeDigits;
	m_TimeDigits << "Time: " << std::fixed << std::setprecision(0) << m_time;
	m_timer.setString(m_TimeDigits.str());

	if (m_tank.m_Shots > 0 || m_tank.m_Shots > INFINITY) {
		 m_tank.m_accruacy = (static_cast<float>(m_tank.m_Shots) / m_tank.m_hits) * 100;

		std::stringstream m_AccuracyDigits;
		m_AccuracyDigits << "Accuracy = " << std::fixed << std::setprecision(0) << m_tank.m_accruacy << " %";
		m_Accuracy.setString(m_AccuracyDigits.str());
	}
	else {
		m_Accuracy.setString("Accuracy = 0.00%");
	}

	if (m_tank.m_accruacy > 100) {
		m_Accuracy.setString("Accuracy = 100.00%");
	}
}


//void Game::setupenemys()
//	{
//		sf::Texture& m_texture6 = m_holder["tankAtlas"];
//		
//		//sf::Sprite sprite2;
//		sf::IntRect enemybase(247, 0, 224, 116);
//		sf::IntRect enemyturret(279, 114, 213, 96);
//
//    for (auto const& enemyData : m_level.m_enemies_tanks) {
//        sf::Sprite sprite1;
//		sprite1.setTexture(m_texture6);
//		sprite1.setTextureRect(enemybase);
//		sprite1.setOrigin(100.5, 57);
//		sprite1.setPosition(enemyData.m_position);
//		sprite1.setRotation(enemyData.m_rotation);
//		m_EnemySprites.push_back(sprite1);
//	}
//}



////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	if (!m_game_over) {
		m_tank.update(dt);
		updateTimer();
		
	}
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	m_window.draw(m_bgSprite);
	m_tank.render(m_window);
	
	
	for (auto& walls : m_wallSprites) {
		m_window.draw(walls);
	}

	/*for (auto& enemys : m_EnemySprites) {
		m_window.draw(enemys);
	}*/

	m_window.draw(m_timer);
	m_window.draw(m_Hits);

	if (m_game_over) {
		m_window.draw(m_GameOver);
		m_window.draw(m_Accuracy);
	}
#ifdef TEST_FPS
	m_window.draw(x_updateFPS);
	m_window.draw(x_drawFPS);
#endif
	m_window.display();
}





