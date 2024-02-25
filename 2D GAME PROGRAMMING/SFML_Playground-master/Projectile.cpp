#include "Projectile.h"

Projectile::Projectile(thor::ResourceHolder<sf::Texture, std::string>& t_holder, std::vector<sf::Sprite>& t_projectileSprites)
	: m_holder(t_holder),
	m_ProjectileSprites(t_projectileSprites)
{
	int currentLevel = 1;

	try {
		LevelLoader::load(currentLevel, m_level);
	}
	catch (std::exception& e) {
		std::cout << "Level Loading Failed" << std::endl;
		std::cout << e.what() << std::endl;
		throw e;
	}

	setupbullets();
}

void Projectile::update(double dt)
{
	for (auto& projectile : m_ProjectileSprites) {
	
	}
}

void Projectile::render(sf::RenderWindow& window)
{
	for (auto& projectile : m_ProjectileSprites) {
		window.draw(projectile);
	}

}

void Projectile::setPosition(sf::Vector2f t_position)
{


}

sf::Vector2f Projectile::getPosition()
{


	return sf::Vector2f();
}

void Projectile::setScale(sf::Vector2f t_scale)
{



}

void Projectile::setupbullets()
{
	sf::Texture& m_texture = m_holder["tankAtlas"];

	sf::IntRect bullet(253, 116, 24, 10);

	for (auto const& projectile : m_level.m_projectiles) {
		sf::Sprite sprite;
		sprite.setTexture(m_texture);
		sprite.setTextureRect(bullet);
		sprite.setOrigin(bullet.width / 2.0, bullet.height / 2.0);
		sprite.setPosition(projectile.m_position);
		sprite.setRotation(projectile.m_rotation);
		m_ProjectileSprites.push_back(sprite);

	}
}


