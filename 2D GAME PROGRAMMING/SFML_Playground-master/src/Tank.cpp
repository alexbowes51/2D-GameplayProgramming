#include "Tank.h"


Tank::Tank(thor::ResourceHolder<sf::Texture, std::string> & t_holder)
: m_holder(t_holder)
{
	initSprites();
}

void Tank::update(double dt)
{	


	m_speed = std::clamp(m_speed, MAX_REVERSE_SPEED, MAX_FORWARD_SPEED);

	float NewPos_x = m_tankBase.getPosition().x + cos(m_rotation) * m_speed * (dt / 1000);

	float NewPos_y = m_tankBase.getPosition().y + sin(m_rotation) * m_speed * (dt / 1000);

	m_tankBase.setPosition(NewPos_x, NewPos_y);
	m_turret.setPosition(NewPos_x, NewPos_y);

	m_tankBase.getRotation();
	m_tankBase.setRotation(m_rotation);

	m_turret.getRotation();
	m_turret.setRotation(m_rotation);



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

	if (m_rotation + 90.0f >= 360.0) {
		m_rotation = 0;
	}
}

void Tank::decreaseRotation()
{
	m_rotation -= 1; 

	if (m_rotation + 90.0f <= 0.0) { 
		m_rotation = 360.0;
	}
}


void Tank::initSprites()
{
	m_holder.acquire("tankAtlas", thor::Resources::fromFile<sf::Texture>("resources/Images/SpriteIndex.png"));
	sf::Texture& texture = m_holder["tankAtlas"];
	sf::Texture& texture2 = m_holder["tankAtlas"];

	m_tankBase.setTexture(texture);
	m_tankBase.setTextureRect(sf::IntRect(0, 0, 245, 114));
	m_tankBase.setOrigin(122.5, 57);
	m_tankBase.setRotation(90.0f);
	

	m_turret.setTexture(texture2);
	m_turret.setTextureRect(sf::IntRect(0, 114, 229, 96));
	m_turret.setOrigin(114.5, 48);
	m_turret.setRotation(90.0f);


}