#pragma once
#include "Object.h"
class Solid : public Object
{
public:
	static std::vector<Solid*> solids;
	Solid(Vector2d coordinates, double mass, double radius, Vector2d speed = { 0.0, 0.0 });
	virtual void draw(sf::RenderTarget& target) const override;
	void calcCollisions();
protected:
	double radius;
};

