#include "Solid.h"

Solid::Solid(Vector2d coordinates, double mass, double radius, const sf::Texture& texture, Vector2d speed) :
	Object(coordinates, mass, speed, new sf::CircleShape(radius), new sf::Sprite(texture)), radius(radius)
{
	shape->setOrigin(radius, radius);
	shape->setPosition(Vector2f(coordinates));
	sprite->setOrigin(sprite->getTextureRect().width / 2, sprite->getTextureRect().height / 2);
	sprite->setPosition(Vector2f(coordinates));
	solids.push_back(this);
}

void Solid::draw(sf::RenderTarget& target) const
{
	target.draw(*shape);
	target.draw(*sprite);
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

void Solid::move(double elapsedTime, const sf::Vector2u& size)
{
	if (borders) {
		if (coordinates.x < radius && newSpeed.x < 0) {
			newSpeed.x = -newSpeed.x;
		}
		if (coordinates.x > size.x - radius && newSpeed.x > 0) {
			newSpeed.x = -newSpeed.x;
		}
		if (coordinates.y < radius && newSpeed.y < 0) {
			newSpeed.y = -newSpeed.y;
		}
		if (coordinates.y > size.y - radius && newSpeed.y > 0) {
			newSpeed.y = -newSpeed.y;
		}
	}
	Object::move(elapsedTime);
}

