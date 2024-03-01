#include "Tank.h"
#include <iomanip>
using namespace MathUtility;


Tank::Tank(thor::ResourceHolder<sf::Texture, std::string>& t_holder,
	std::vector<sf::Sprite>& t_wallSprites,std::vector<sf::Sprite>& t_EnemySprites)
	: m_holder(t_holder),
	  m_wallSprites(t_wallSprites),
	  m_EnemySprites(t_EnemySprites)
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

	if (!m_arialFont.loadFromFile("resources/BebasNeue.otf"))
	{
		std::cout << "Error loading font file";
	}

	initSprites();
	setupbullets();
	setupText();
}

void Tank::update(double dt)
{
	HandleKeyInput();
	checkbulletbounds();

	if (centreNose == true) {
		centreTurret();
	}

	if (centreNose == false) {
		tankAimSystem();
	}

	if (m_aiming && !m_Fire) {
		tankShootingSystem();
	}

	if (m_aiming && m_Fire) {
		for (auto& projectile : m_ProjectileSprites) {
			projectile.move(m_BulletVelocity);
		}
	}

	

		if (checkWallCollision()) {
			m_state = TankState::COLLIDING;
		}
		else {
			m_state = TankState::NORMAL;
		}

		switch (m_state)
		{
		case TankState::NORMAL:
			HandleKeyInput(); // putting the function here checks the input first then clamo the speed and the new x and y

			m_speed = std::clamp(m_speed, MAX_REVERSE_SPEED, MAX_FORWARD_SPEED);

			NewPos_x = m_tankBase.getPosition().x + cos(m_rotation * DEG_TO_RAD) * m_speed * (dt / 1000);

			NewPos_y = m_tankBase.getPosition().y + sin(m_rotation * DEG_TO_RAD) * m_speed * (dt / 1000);

			if (!m_Move) {
				m_tankBase.setPosition(NewPos_x, NewPos_y);
				m_turret.setPosition(NewPos_x, NewPos_y);
				m_tankBase.setRotation(m_rotation);
				clicktodrive();
			}
			if (m_Move) {
				m_tankBase.move(m_TankVelocity);
				m_turret.move(m_TankVelocity);
				m_tankBase.setRotation(m_DriveStreering - 10);
			}

			if (!m_Fire) {
				for (auto& projectile : m_ProjectileSprites) {
					projectile.setPosition(NewPos_x, NewPos_y);

				}
			}

			if (m_speed > 0) {
				m_speed -= 0.9;
			}
			if (m_speed < 0) {
				m_speed += 0.9;
			}


			break;
		case TankState::COLLIDING:
			deflect(dt);
			break;
		}
	}

void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
	window.draw(m_Accrucity);
	window.draw(m_Hits);
	window.draw(m_Shot);
	window.draw(m_Misses);

	for (auto& projectile : m_ProjectileSprites) {
		window.draw(projectile);
	}
}

void Tank::setPosition(sf::Vector2f t_position)
{
	m_tankBase.setPosition(t_position);
	m_turret.setPosition(t_position);

}

sf::Vector2f Tank::getPosition()
{
	return m_tankBase.getPosition();
}


void Tank::setScale(sf::Vector2f t_scale)
{
	m_tankBase.setScale(t_scale);
	m_turret.setScale(t_scale);
}

void Tank::increaseSpeed()
{
	m_speed += 1;
	
}

void Tank::decreaseSpeed()
{
	m_speed -= 1;
}

void Tank::increaseRotation()
{
	m_rotation += 1;

	if (m_rotation > 360.0) {
		m_rotation = 0;
	}
}

void Tank::decreaseRotation()
{
	m_rotation -= 1; 

	if (m_rotation < 0.0) { 
		m_rotation = 360.0;
	}
}

void Tank::increaseTurretRotation()
{
	m_turretRotation += 1;

	if (m_turretRotation < 0.0) {
		m_turretRotation = 360.0;
	}

}

void Tank::decreaseTurretRotation()
{
	m_turretRotation -= 1;

	if (m_turretRotation < 0.0) {
		m_turretRotation = 360.0;
	}

}

void Tank::HandleKeyInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		increaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		decreaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		increaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		decreaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
		increaseTurretRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		decreaseTurretRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		centreNose = true;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		
		m_Fire = true;

		if (m_Click){
			m_Click = false;
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

		bool Click;
		Click = true;

		if (Click) {
			m_Move = true;
			Click = false;
		}
	}
	
}

void Tank::centreTurret()
{
	if (m_rotation >= 180.0f) {
		m_turretRotation = m_turretRotation + 360;
	}
	else if(m_turretRotation >= 180.0f)
	{
		m_turretRotation = m_turretRotation - 360;
	}

	if (m_turretRotation < m_rotation) {
		direction = 1;
	}
	else {
		direction = -1;
	}

	if (direction == 1) {
		m_turretRotation += 1;
	}

	if (direction == -1) {
		m_turretRotation -= 1;
	}

	if (m_turretRotation == m_rotation) {
		centreNose = false;
	}
}

void Tank::tankAimSystem()
{
	sf::Vector2i mouseLocation = sf::Mouse::getPosition();
	// Calculate the direction vector from turrent to mouse 
	sf::Vector2f direction = sf::Vector2f(mouseLocation.x, mouseLocation.y) - m_turret.getPosition();

	// Calculate the length of the direction vector
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	// Calculate the angle between the turrent and the mouse cursor using atan2
	float AngleRadians = std::atan2(direction.y, direction.x);
	float AngleDegrees = AngleRadians * 180 / 3.14; // Convert radians to degrees

	m_turret.setRotation(AngleDegrees); // Adjust sprite orientation

	if (!m_Fire) {
		for (auto& projectile : m_ProjectileSprites) {
			projectile.setRotation(AngleDegrees);
		}
	}

}

bool Tank::checkWallCollision()
{
	for (sf::Sprite const& wall : m_wallSprites) {
		//collisions for tank 
		if (CollisionDetector::collision(m_turret, wall))
		{
			if (CollisionDetector::pixelPerfectTest(m_turret, wall)) {
				// Get contact normal vector between the turret and the wall
				m_contactNormal = m_turret.getPosition() - wall.getPosition();
				m_Move = false;
				return true;
			}
			else if (CollisionDetector::pixelPerfectTest(m_tankBase, wall))
			{
				// Get contact normal vector between tank base and the wall
				m_contactNormal = m_tankBase.getPosition() - wall.getPosition();
				m_Move = false;
				return true;
			}
		}

	}

	for (sf::Sprite const& wall : m_wallSprites) {
		for (sf::Sprite const& projectile : m_ProjectileSprites) {

			//collisions for projectiles
			if (CollisionDetector::collision(projectile, wall)) {
				if (CollisionDetector::pixelPerfectTest(projectile, wall)) {
					bool hit;
					hit = true;
					if (hit) {
						m_Fire = false;

						hit = false;
					}
					m_shots = m_shots + 1;
					m_misses = m_misses + 1;
				}
			}
		}
	}

	for (sf::Sprite const&  enemy: m_EnemySprites) {
		for (sf::Sprite const& projectile : m_ProjectileSprites) {
			//collisions for projectiles
			if (CollisionDetector::collision(projectile, enemy)) {
				if (CollisionDetector::pixelPerfectTest(projectile, enemy)) {
					bool hit;
					hit = true;
					if (hit) {
						m_Fire = false;

						hit = false;
					}
					m_shots = m_shots + 1;
					m_hits++;
					m_hits = m_hits + 1;
					
				}
			}
		}
	}
		m_Hits.setString("Hits : " + std::to_string(m_hits));
		m_Shot.setString("Shots : " + std::to_string(m_shots));
		return false;
}

void Tank::checkbulletbounds()
{
	for (auto& projectile : m_ProjectileSprites) {
		if (projectile.getPosition().x < 0) {
			m_Fire = false;
			m_misses = m_misses + 1;
			m_shots = m_shots + 1;
		}
		if (projectile.getPosition().x > 1500) {
			m_Fire = false;
			m_misses = m_misses + 1;
			m_shots = m_shots + 1;
		}
		if (projectile.getPosition().y < 0) {
			m_Fire = false;
			m_misses = m_misses + 1;
			m_shots = m_shots + 1;
		}
		if (projectile.getPosition().y > 1000) {
			m_Fire = false;
			m_misses = m_misses + 1;
			m_shots = m_shots + 1;
		}
	}
	m_Shot.setString("Shots : " + std::to_string(m_shots));
	m_Misses.setString("Misses : " + std::to_string(m_misses));
}

void Tank::deflect(double dt){

	float impulseMagnitude = std::abs(m_speed);

	m_contactNormal = thor::unitVector(m_contactNormal);


	sf::Vector2f deflectVector =
	{
	impulseMagnitude * m_contactNormal.x * (static_cast<float>(dt) / 1000),
	impulseMagnitude * m_contactNormal.y * (static_cast<float>(dt) / 1000)
	};

	m_tankBase.move(deflectVector.x, deflectVector.y);
	m_turret.move(deflectVector.x, deflectVector.y);
}

void Tank::initSprites()
{
	m_holder.acquire("tankAtlas", thor::Resources::fromFile<sf::Texture>("resources/Images/SpriteIndex.png"));
	sf::Texture& texture = m_holder["tankAtlas"];
	sf::Texture& texture2 = m_holder["tankAtlas"];

	m_tankBase.setTexture(texture);
	m_tankBase.setTextureRect(sf::IntRect(0, 0, 245, 114));
	m_tankBase.setOrigin(100.5, 57);

	m_turret.setTexture(texture2);
	m_turret.setTextureRect(sf::IntRect(0, 114, 229, 96));
	m_turret.setOrigin(78, 48);


}

void Tank::setupbullets()
{
	sf::Texture& m_texture = m_holder["tankAtlas"];

	sf::IntRect bullet(253, 116, 24, 10);

	for (auto const& projectile : m_level.m_projectiles) {
		sf::Sprite sprite;
		sprite.setTexture(m_texture);
		sprite.setTextureRect(bullet);
		sprite.setOrigin(bullet.width / 2.0, bullet.height / 2.0);
		sprite.setPosition(projectile.m_position);
		sprite.setRotation(projectile.m_rotation);
		m_ProjectileSprites.push_back(sprite);

	}
}

void Tank::setupText()
{
	m_Hits.setFont(m_arialFont);
	m_Hits.setCharacterSize(25U);
	m_Hits.setPosition(655.0f, 25.0f);
	m_Hits.setFillColor(sf::Color::White);
	m_Hits.setString("Hits : " + std::to_string(m_hits));

	
	m_Shot.setFont(m_arialFont);
	m_Shot.setCharacterSize(25U);
	m_Shot.setPosition(355.0f, 25.0f);
	m_Shot.setFillColor(sf::Color::White);
	m_Shot.setString("Shots : " + std::to_string(m_shots));

	
	m_Misses.setFont(m_arialFont);
	m_Misses.setCharacterSize(25U);
	m_Misses.setPosition(955.0f, 25.0f);
	m_Misses.setFillColor(sf::Color::White);
	m_Misses.setString("Misses : " + std::to_string(m_misses));
}

void Tank::tankShootingSystem()
{
		for (auto& projectile : m_ProjectileSprites) {
			sf::Vector2i mouseLocation = sf::Mouse::getPosition();

			sf::Vector2f Direction = sf::Vector2f(mouseLocation.x, mouseLocation.y) - projectile.getPosition();

			float length = std::sqrt(Direction.x * Direction.x + Direction.y * Direction.y);

			if (length != 0) {
				Direction /= length;
			}

			float AngleRadians = std::atan2(Direction.y, Direction.x);
			float AngleDegrees = AngleRadians * 180 / 3.14; // Convert radians to degrees

			if (m_Move) {
				m_turret.setRotation(AngleDegrees);
			}

			m_BulletVelocity = Direction;
		}
}

void Tank::clicktodrive()
{
	sf::Vector2i mouseLocation = sf::Mouse::getPosition();

	sf::Vector2f Direction = sf::Vector2f(mouseLocation.x, mouseLocation.y) - m_tankBase.getPosition();

	float length = std::sqrt(Direction.x * Direction.x + Direction.y * Direction.y);

	if (length != 0) {
		Direction /= length;
	}

	float AngleRadians = std::atan2(Direction.y, Direction.x);
	float AngleDegrees = AngleRadians * 180 / 3.14; // Convert radians to degrees

	m_TankVelocity = Direction;
     
	m_DriveStreering = AngleDegrees;
	

}
