#include "LevelLoader.h"

/// The various operator >> overloads below are non-member functions used to extract
///  the game data from the YAML data structure.

/// <summary>
/// @brief Extracts the obstacle type, position and rotation values.
/// 
/// </summary>
/// <param name="t_obstacleNode">A YAML node</param>
/// <param name="t_obstacle">A simple struct to store the obstacle data</param>
////////////////////////////////////////////////////////////
/// 

void operator >> (const YAML::Node& t_projectileNode, ProjectileData& t_Projectile)
{
	t_Projectile.m_type = t_projectileNode["type"].as<std::string>();

	t_Projectile.m_position.x = t_projectileNode["position"]["x"].as<float>();
	t_Projectile.m_position.y = t_projectileNode["position"]["y"].as<float>();

	t_Projectile.m_scale.x = t_projectileNode["scale"]["x"].as<float>();
	t_Projectile.m_scale.y = t_projectileNode["scale"]["y"].as<float>();

	t_Projectile.m_rotation = t_projectileNode["rotation"].as<double>();

	std::cout << "bullet operater called " << std::endl;
}

void operator >> (const YAML::Node& t_obstacleNode, ObstacleData& t_obstacle)
{
	t_obstacle.m_type = t_obstacleNode["type"].as<std::string>();
	t_obstacle.m_position.x = t_obstacleNode["position"]["x"].as<float>();
	t_obstacle.m_position.y = t_obstacleNode["position"]["y"].as<float>();
	t_obstacle.m_rotation = t_obstacleNode["rotation"].as<double>();
}

/// <summary>
/// @brief Extracts the filename for the game background texture.
/// 
/// </summary>
/// <param name="t_backgroundNode">A YAML node</param>
/// <param name="t_background">A simple struct to store background related data</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& t_backgroundNode, BackgroundData& t_background)
{
	t_background.m_fileName = t_backgroundNode["file"].as<std::string>();
}

/// <summary>
/// @brief Extracts the initial screen position for the player tank.
/// 
/// </summary>
/// <param name="t_tankNode">A YAML node</param>
/// <param name="t_tank">A simple struct to store data related to the player tank</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& t_tankNode, TankData& t_tank)
{
	t_tank.m_position.x = t_tankNode["position"]["x"].as<float>();
	t_tank.m_position.y = t_tankNode["position"]["y"].as<float>();

	t_tank.m_scale.x = t_tankNode["scale"]["x"].as<float>();
	t_tank.m_scale.y = t_tankNode["scale"]["y"].as<float>();

	//std::cout << "tank void op called" << std::endl;
}

/// <summary>
/// @brief Top level function that extracts various game data from the YAML data stucture.
/// 
/// Invokes other functions to extract the background, tank and obstacle data.
//   Because there are multiple obstacles, obstacle data are stored in a vector.
/// </summary>
/// <param name="t_levelNode">A YAML node</param>
/// <param name="t_level">A simple struct to store level data for the game</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& t_levelNode, LevelData& t_level)
{
	t_levelNode["background"] >> t_level.m_background; // calls the void operator for background

	t_levelNode["tank"] >> t_level.m_tank;// calls the void operator for tank

	const YAML::Node& obstaclesNode = t_levelNode["obstacles"].as<YAML::Node>();
	for (unsigned i = 0; i < obstaclesNode.size(); ++i)
	{
		ObstacleData obstacle;
		obstaclesNode[i] >> obstacle;
		t_level.m_obstacles.push_back(obstacle);
	}

	const YAML::Node& projectileNode = t_levelNode["projectiles"].as<YAML::Node>();
	for (unsigned i = 0; i < projectileNode.size(); ++i)
	{
		ProjectileData projectile;
		projectileNode[i] >> projectile;
		t_level.m_projectiles.push_back(projectile);
	}
}

////////////////////////////////////////////////////////////
void LevelLoader::load(int t_levelNr, LevelData& t_level)
{
	std::string filename = "./resources/levelData/level" + std::to_string(t_levelNr) + ".yaml";

	try
	{
		YAML::Node baseNode = YAML::LoadFile(filename);
		if (baseNode.IsNull())
		{
			std::string message("File: " + filename + " not found");
			throw std::exception(message.c_str());
		}
		baseNode >> t_level;
	}
	catch (YAML::ParserException& e)
	{
		std::string message(e.what());
		message = "YAML Parser Error: " + message;
		throw std::exception(message.c_str());
	}
	catch (std::exception& e)
	{
		std::string message(e.what());
		message = "Unexpected Error: " + message;
		throw std::exception(message.c_str());
	}
}


