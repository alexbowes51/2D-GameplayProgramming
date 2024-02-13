#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"

/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>

enum class TankState{NORMAL,COLLIDING};

class Tank
{
public:	
	Tank(thor::ResourceHolder <sf::Texture, std::string> & t_holder,std::vector<sf::Sprite> & t_wallSprites);
	void update(double dt);
	void render(sf::RenderWindow & window);
	void setPosition(sf::Vector2f t_position);
	sf::Vector2f getPosition();
	void setScale(sf::Vector2f t_scale);

	void increaseSpeed();
	void decreaseSpeed();

	void increaseRotation();
	void decreaseRotation();
	void increaseTurretRotation();
	void decreaseTurretRotation();

	void HandleKeyInput();
	void centreTurret();

	bool checkWallCollision();
	void deflect(double dt);

	double m_speed{ 0.0 };
	double m_rotation{ 0.0 };
	double m_turretRotation{ 0.0 };
	int direction = 1;
	bool centreNose = false;

	double const MAX_REVERSE_SPEED{ -100 };
	double const MAX_FORWARD_SPEED{ 100 };

	float NewPos_x{ 0 };
	float NewPos_y{ 0 };

	
private:
	void initSprites();
	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	thor::ResourceHolder<sf::Texture, std::string> & m_holder;
	std::vector<sf::Sprite>& m_wallSprites;
	sf::Vector2f m_contactNormal;
	TankState m_state{ TankState::NORMAL };
		
};
