#include "Tank.h"
using namespace MathUtility;


Tank::Tank(thor::ResourceHolder<sf::Texture, std::string>& t_holder,
	std::vector<sf::Sprite>& t_wallSprites)
	: m_holder(t_holder),
	  m_wallSprites(t_wallSprites)
{
	initSprites();
}

void Tank::update(double dt)
{	
	if (m_speed > 0) {
		m_speed *= 0.99;
	}
	
	if (checkWallCollision()){
		m_state = TankState::COLLIDING;
	}else{
		m_state = TankState::NORMAL;
	}

	switch (m_state) 
	{
	case TankState::NORMAL:
		HandleKeyInput(); // putting the function here checks the input first then clamo the speed and the new x and y

		m_speed = std::clamp(m_speed, MAX_REVERSE_SPEED, MAX_FORWARD_SPEED);

		 NewPos_x = m_tankBase.getPosition().x + cos(m_rotation * DEG_TO_RAD) * m_speed * (dt / 1000);

		 NewPos_y = m_tankBase.getPosition().y + sin(m_rotation * DEG_TO_RAD) * m_speed * (dt / 1000);

		m_tankBase.setPosition(NewPos_x, NewPos_y);
		m_turret.setPosition(NewPos_x, NewPos_y);

		m_tankBase.setRotation(m_rotation);
		m_turret.setRotation(m_turretRotation);

		if (m_speed > 0) {
			m_speed *= 0.99;
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
	
}

void Tank::centreTurret()
{
	if (m_rotation >= 180.0f) {
		m_turretRotation = m_turretRotation + 360;
	}
	else if(m_turretRotation >= 180)
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

bool Tank::checkWallCollision()
{
	for (sf::Sprite const& sprite : m_wallSprites)

	{
		// Checks if either the tank base or turret has collided
		// with the current wall sprite.
		if (CollisionDetector::collision(m_turret, sprite))

		{
			// Get contact normal vector between the turret and the wall
			m_contactNormal = m_turret.getPosition() - sprite.getPosition();

			return true;
		}
		else if (CollisionDetector::collision(m_tankBase, sprite))
		{
			// Get contact normal vector between tank base and the wall
			m_contactNormal = m_tankBase.getPosition() - sprite.getPosition();
			return true;
		}

	}
	return false;
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