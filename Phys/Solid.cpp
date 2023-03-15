#include "Solid.h"

Solid::Solid(Vector2d coordinates, double mass, float radius, Vector2d speed) :
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
	Vector2d r, norm, perp, proj1, proj2;
	double length;
	newSpeed = speed;
	for (auto i : solids) {
		if (i != this) {
			r = i->coordinates - coordinates;
			length = hypot(r.x, r.y);
			if (length <= double(radius) + i->radius) {
				norm = r / length;

				perp.x = norm.y; perp.y = -norm.x;
				proj1 = dotProduct(newSpeed, norm) * norm;
				proj2 = dotProduct(i->speed, norm) * norm;

				newSpeed = dotProduct(newSpeed, perp) * perp + (proj1 * mass + proj2 * i->mass) / (mass + i->mass);
			}
		}
	}
}
