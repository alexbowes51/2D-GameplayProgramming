#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"
#include "LevelLoader.h"
#include <ProjectilePool.h>

/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>

enum class TankState{NORMAL,COLLIDING};

enum class ProjectileStat{NORMAL,COLLIDING};

class Tank
{
public:
	Tank(thor::ResourceHolder <sf::Texture, std::string>& t_holder, std::vector<sf::Sprite>& t_wallSprites, std::vector<sf::Sprite>& t_EnemySprites);
	void update(double dt);
	void render(sf::RenderWindow& window);
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

	void tankAimSystem();
	void tankShootingSystem();
	void clicktodrive();

	bool checkWallCollision();
	void checkbulletbounds();
	void deflect(double dt);

	void requestFire();
	bool m_fireRequested = false;
	int m_shootTimer = 800;
	static int const s_TIME_BETWEEN_SHOTS{ 800 };


	double m_speed{ 0.0 };
	double m_rotation{ 0.0 };
	double m_turretRotation{ 0.0 };
	int direction = 1;
	bool centreNose = false;



	double const MAX_REVERSE_SPEED{ -100 };
	double const MAX_FORWARD_SPEED{ 100 };

	float NewPos_x{ 0 };
	float NewPos_y{ 0 };

	sf::Vector2f m_BulletVelocity{ 0.0f,0.0f };
	sf::Vector2f m_TankVelocity{ 0.0f,0.0f };
	float m_DriveStreering{0};

	LevelData m_level;
	ProjectilePool m_pool;

	bool m_Click = false;
	bool m_Fire = false;
	bool m_aiming = true;

	bool m_Move = false;

	int m_hits = 0;
	int m_misses = 0;
	int m_shots = 0;

	float m_accruacy = 0;

	sf::Text m_Accrucity;
	sf::Font m_arialFont;
	sf::Text m_Hits;
	sf::Text m_Shot;
	sf::Text m_Misses;

	bool m_FireArray[3];
	
private:
	void initSprites();
	void setupbullets();
	void setupText();

	
	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	sf::Sprite m_bullet;
	thor::ResourceHolder<sf::Texture, std::string> & m_holder;
	std::vector<sf::Sprite>& m_wallSprites;
	std::vector<sf::Sprite> m_ProjectileSprites;
	std::vector<sf::Sprite>& m_EnemySprites;

	float FrameRateMiliseconds = 16.67; //60 fps in miliseconds
	float time = 80.0;
	

	sf::Vector2f m_contactNormal;
	TankState m_state{ TankState::NORMAL };

		
};
