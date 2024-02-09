#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>


/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
public:	
	Tank(thor::ResourceHolder <sf::Texture, std::string> & t_holder);
	void update(double dt);
	void render(sf::RenderWindow & window);
	void setPosition(sf::Vector2f t_position);
	sf::Vector2f getPosition();
	void setScale(sf::Vector2f t_scale);

	void increaseSpeed();
	void decreaseSpeed();

	void increaseRotation();
	void decreaseRotation();

	double m_speed{ 0.0 };
	double m_rotation{ 0.0 };

	double const MAX_REVERSE_SPEED{ -100 };
	double const MAX_FORWARD_SPEED{ 100 };
	
private:
	void initSprites();
	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	thor::ResourceHolder<sf::Texture, std::string> & m_holder;
		
};
