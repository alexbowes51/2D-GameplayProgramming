#include "AITank.h"

////////////////////////////////////////////////////////////
AITank::AITank(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites)
	: m_aiBehaviour(AiBehaviour::SEEK_PLAYER)
	, m_texture(texture)
	, m_wallSprites(wallSprites)
	, m_steering(0, 0)
{
	// Initialises the tank base and turret sprites.
	initSprites();
}



////////////////////////////////////////////////////////////
void AITank::update(Tank m_Tank, double dt)
{

	sf::Vector2f vectorToPlayer = seek(m_Tank.getPosition());//the object has type qualifiers that are not compatible with the member function had to remove the const + & 
	switch (m_aiBehaviour)
	{
		case AiBehaviour::SEEK_PLAYER:
			m_steering += thor::unitVector(vectorToPlayer);
			m_steering += collisionAvoidance();
			m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
			m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);				
			break;
	
		case AiBehaviour::STOP:
			m_velocity = sf::Vector2f(0, 0);
			break;
		default:
			break;
	}
	
	// Now we need to convert our velocity vector into a rotation angle between 0 and 359 degrees.
	// The m_velocity vector works like this: vector(1,0) is 0 degrees, while vector(0, 1) is 90 degrees.
	// So for example, 223 degrees would be a clockwise offset from 0 degrees (i.e. along x axis).
	// Note: we add 180 degrees below to convert the final angle into a range 0 to 359 instead of -PI to +PI
	auto dest = atan2(-1 * m_velocity.y, -1 * m_velocity.x) / std::numbers::pi * 180 + 180;

	auto currentRotation = m_rotation;

	// Find the shortest way to rotate towards the player (clockwise or anti-clockwise)
	if (std::round(currentRotation - dest) == 0.0)
	{
		m_steering.x = 0;
		m_steering.y = 0;
	}

	else if ((static_cast<int>(std::round(dest - currentRotation + 360))) % 360 < 180)
	{
		// rotate clockwise
		m_rotation = static_cast<int>((m_rotation) + 1) % 360;
	}
	else
	{
		// rotate anti-clockwise
		m_rotation = static_cast<int>((m_rotation) - 1) % 360;
	}


	if (thor::length(vectorToPlayer) < MAX_SEE_AHEAD)
	{
		m_aiBehaviour = AiBehaviour::STOP;
	}
	else
	{
		m_aiBehaviour = AiBehaviour::SEEK_PLAYER;
	}

	updateMovement(dt);

}

////////////////////////////////////////////////////////////
void AITank::render(sf::RenderWindow & window)
{
	// TODO: Don't draw if off-screen...
	window.draw(m_tankBase);
	window.draw(m_turret);	

	for (auto& circle : m_obstacles) {
		window.draw(circle);
	}
}

////////////////////////////////////////////////////////////
void AITank::init(sf::Vector2f t_position, sf::Vector2f t_scale)
{
	m_tankBase.setPosition(t_position);
	m_tankBase.setScale(t_scale.x, t_scale.y);
	m_turret.setPosition(t_position);
	m_turret.setScale(t_scale.x, t_scale.y);

	for (sf::Sprite const wallSprite : m_wallSprites)
	{
		sf::CircleShape circle(wallSprite.getTextureRect().width * 1.5f); 
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setFillColor(sf::Color(255, 255, 255, 125));
		circle.setPosition(wallSprite.getPosition());
		m_obstacles.push_back(circle);
	}
}

////////////////////////////////////////////////////////////
sf::Vector2f AITank::seek(sf::Vector2f t_playerPosition) const {
	


   // This return statement is simply a placeholder and must be changed...
	return sf::Vector2f(0, 1);	
}

////////////////////////////////////////////////////////////
sf::Vector2f AITank::collisionAvoidance()
{
	auto headingRadians = thor::toRadian(m_rotation);
	sf::Vector2f headingVector(std::cos(headingRadians) * MAX_SEE_AHEAD, std::sin(headingRadians) * MAX_SEE_AHEAD);
	m_ahead = m_tankBase.getPosition() + headingVector;
	m_halfAhead = m_tankBase.getPosition() + (headingVector * 0.5f);

	findMostThreateningObstacle();
	
	sf::Vector2f avoidance(0, 0);

	if (m_mostThreatening != nullptr)
	{		
		avoidance.x = m_ahead.x - m_mostThreatening->getPosition().x;
		avoidance.y = m_ahead.y - m_mostThreatening->getPosition().y;
		avoidance = thor::unitVector(avoidance);
		avoidance *= MAX_AVOID_FORCE;
	}
	else
	{		
		avoidance *= 0.0f;
	}
	
	return avoidance;
}


////////////////////////////////////////////////////////////
void  AITank::findMostThreateningObstacle()
{	
	m_mostThreatening = nullptr;
	// Complete....

}

////////////////////////////////////////////////////////////
void AITank::initSprites()
{	
	// BrownTank,247, 0, 224, 116
	// Gun_01_Brown,279, 114, 213, 96

	//sf::IntRect enemybase(247, 0, 224, 116);
    //sf::IntRect enemyturret(279, 114, 213, 96);

	sf::IntRect brownTankRect(247, 0, 224, 116);
	m_tankBase.setTexture(m_texture);
	m_tankBase.setTextureRect(brownTankRect);

	m_tankBase.setOrigin(88, brownTankRect.height / 2.0);
	m_tankBase.setPosition(sf::Vector2f(200, 200));

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(279, 114, 213, 96);
	m_turret.setTextureRect(turretRect);

	m_turret.setOrigin(45, turretRect.height / 2.0);
}


////////////////////////////////////////////////////////////
void AITank::updateMovement(double dt)
{
	double speed = thor::length(m_velocity);
	sf::Vector2f newPos(m_tankBase.getPosition().x + std::cos(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000),
	m_tankBase.getPosition().y + std::sin(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000));
	m_tankBase.setPosition(newPos.x, newPos.y);
	m_tankBase.setRotation(m_rotation);
	m_turret.setPosition(m_tankBase.getPosition());
	m_turret.setRotation(m_rotation);
}