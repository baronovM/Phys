#include "Solid.h"

Solid::Solid(Vector2d coordinates, double mass, float radius, Vector2d speed) :
	Object(coordinates, mass, speed, new sf::CircleShape(radius))
{
	m_radius = radius;
	solids.push_back(this);
}

void Solid::draw(sf::RenderTarget& target) const
{
	target.draw(*m_shape);
}

void Solid::calcCollisions()
{
	Vector2d r, norm, perp, proj1, proj2;
	double length;
	m_newSpeed = m_speed;
	for (auto i : solids) {
		if (i != this) {
			r = i->m_coordinates - m_coordinates;
			length = hypot(r.x, r.y);
			if (length <= double(m_radius) + i->m_radius) {
				norm = r / length;

				perp.x = norm.y; perp.y = -norm.x;
				proj1 = dotProduct(m_newSpeed, norm) * norm;
				proj2 = dotProduct(i->m_speed, norm) * norm;

				m_newSpeed = dotProduct(m_newSpeed, perp) * perp + (proj1 * m_mass + proj2 * i->m_mass) / (m_mass + i->m_mass);
			}
		}
	}
}
