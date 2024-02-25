#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"
#include "LevelLoader.h"
#include "Tank.h"



enum class ProjectileState { NORMAL, COLLIDING };

class Projectile {
public:
	Projectile(thor::ResourceHolder <sf::Texture, std::string>& t_holder, std::vector<sf::Sprite>& t_projectileSprites);
	void update(double dt);
	void render(sf::RenderWindow& window);
	void setPosition(sf::Vector2f t_position);
	sf::Vector2f getPosition();
	void setScale(sf::Vector2f t_scale);

	bool checkWallCollision();


	LevelData m_level;


	double m_speed{ 0.0 };

	float NewPos_x{ 0 };
	float NewPos_y{ 0 };


private:
	thor::ResourceHolder<sf::Texture, std::string>& m_holder;
	std::vector<sf::Sprite> m_ProjectileSprites;
	sf::Vector2f m_contactNormal;
	ProjectileState m_state{ ProjectileState::NORMAL };
	void setupbullets();
};