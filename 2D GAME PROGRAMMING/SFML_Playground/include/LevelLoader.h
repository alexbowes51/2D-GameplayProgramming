#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include "src\yaml-cpp\yaml.h"
#include <vector>
#include <SFML/System/Vector2.hpp>



struct PlayerData
{
   std::string m_name;
   int m_xp{0};
};

struct MapData
{
	std::string m_name;
	
};

struct EnemyData
{
	std::string m_type;
	float m_health;
	float m_x;
	float m_y;
};

struct ObstacleData
{
	std::string m_type;
	sf::Vector2f m_position;
	double m_rotation;
};



struct LevelData
{
   PlayerData m_player;
	EnemyData m_enemy;
   MapData m_map;
  std::vector<EnemyData> m_enemies;
  std::vector<ObstacleData> m_obstacles;
};

class LevelLoader
{
public:
	LevelLoader() = default;

   static void load(int t_levelNr, LevelData& t_level);
};


