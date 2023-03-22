#pragma once
#include "Object.h"
class Solid : public Object
{
public:
	static std::vector<Solid*> solids;
	Solid(Vector2d coordinates, double mass, double radius, const sf::Texture& texture, Vector2d speed = { 0.0, 0.0 });
	virtual void draw(sf::RenderTarget& target) const override;
	void calcCollisions();
	void move(double elapsedTime, const sf::Vector2u& size);
protected:
	double radius;
};

