#include "LevelLoader.h"

void operator >> (const YAML::Node& t_playerNode, PlayerData& t_player)
{
	t_player.m_name = t_playerNode["name"].as<std::string>();
	t_player.m_xp = t_playerNode["xp"].as<int>();
}

void operator >> (const YAML::Node& t_levelNode, LevelData& t_level)
{
   // Process the 'player' mapping 
   t_levelNode["player"] >> t_level.m_player;

   // Class Exercise: Process the 'enemy' mapping (type and health only for now)


   // Process the sequence of enemies
   /* const YAML::Node& enemiesNode = t_levelNode["enemies"].as<YAML::Node>();
   for (unsigned i = 0; i < enemiesNode.size(); ++i)
   {
	  EnemyData enemy;
	  enemiesNode[i] >> enemy;
	  t_level.m_enemies.push_back(enemy);
   }*/
}

void LevelLoader::load(int t_levelNr, LevelData& t_level)
{
   std::string filename = "./levelData/level" + std::to_string(t_levelNr) + ".yaml";

   try
   {
	   YAML::Node baseNode = YAML::LoadFile(filename);
	   if (baseNode.IsNull())
	   {
		   std::string message("File: " + filename + " not found");
		   throw std::exception(message.c_str());
	   }
	   // Call overloaded operator >>, passing baseNode as arg1, t_level as arg2
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
