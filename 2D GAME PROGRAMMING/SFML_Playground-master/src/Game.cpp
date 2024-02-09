//by:Alex Bowes 
#include "Game.h"
#include <iostream>
	

// Our target FPS
static double const FPS{ 60.0f };

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default)
	,m_tank(m_holder)
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
	/*	case sf::Keyboard::Up:
			m_tank.increaseSpeed();
			break;
		case sf::Keyboard::Down:
			m_tank.decreaseSpeed();
			break;
		case sf::Keyboard::Right:
			m_tank.increaseRotation();
			break;
		case sf::Keyboard::Left:
			m_tank.decreaseRotation();
			break;*/
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

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	m_tank.update(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		m_tank.increaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		m_tank.decreaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		m_tank.decreaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		m_tank.increaseSpeed();
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

#ifdef TEST_FPS
	m_window.draw(x_updateFPS);
	m_window.draw(x_drawFPS);
#endif
	m_window.display();
}





