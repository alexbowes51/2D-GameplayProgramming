#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include "yaml-cpp\yaml.h"

struct PlayerData
{
	std::string m_name;
	int m_xp;
};

struct EnemyData
{
	std::string m_type;
	float m_health;
	float m_x;
	float m_y;
};

struct LevelData
{
   PlayerData m_player;
   EnemyData m_enemy;
   std::vector<EnemyData> m_enemies;
};

class LevelLoader
{
public:
   static void load(int t_levelNr, LevelData& t_level);
};
