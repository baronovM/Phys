#include "Solid.h"

Solid::Solid(Vector2d coordinates, double mass, double radius, Vector2d speed) :
	Object(coordinates, mass, speed, new sf::CircleShape(radius)), radius(radius)
{
	solids.push_back(this);
}

void Solid::draw(sf::RenderTarget& target) const
{
	target.draw(*shape);
}

void Solid::calcCollisions()
{
	Vector2d r, norm, perp; 
	double length, v1n, v1p, v2n, v2p;
	newSpeed = speed;
	for (auto i : solids) {
		if (i != this) {
			r = i->coordinates - coordinates;
			length = hypot(r.x, r.y);
			
			if (length <= radius + i->radius) {
				norm = r / length;

				perp.x = norm.y; perp.y = -norm.x;
				v1n = dotProduct(newSpeed, norm);
				v1p = dotProduct(newSpeed, perp);
				v2n = dotProduct(i->speed, norm);
				v2p = dotProduct(i->speed, perp);

				v1n = (2. * i->mass * v2n + v1n * (mass - i->mass)) / (mass + i->mass);

				newSpeed = v1p * perp + v1n * norm;
			}
		}
	}
}
