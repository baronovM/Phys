#include "Object.h"

Object::Object(Vector2d coordinates, short charge, double mass, Vector2d speed)
	: m_shape(10.f), m_coordinates(coordinates), m_charge(charge), m_speed(speed), m_mass(mass), m_invMass(1 / mass)
{
	m_shape.setPosition(Vector2f(coordinates));
	m_shape.setFillColor(sf::Color(125 + 125 * m_charge, 125, 125 - 125 * m_charge));
	objects.push_back(this);
}

void Object::draw(sf::RenderTarget& target) const
{
	//if (target.getViewport(target.getDefaultView()).intersects(sf::IntRect(m_shape.getGlobalBounds())))
		target.draw(m_shape);
}

// Electromagnetic interaction
void Object::calcElectro(double elapsedTime)
{
	if (m_charge == (short)0)
		return;
	Vector2d F(0, 0), r;
	double length, tempF;
	for (auto i : objects) {
		if (i != this) {
			r = i->m_coordinates - m_coordinates;
			length = hypot(r.x, r.y);
			tempF = i->m_charge / sqr(length * invKsqrt);
			F += r / length * tempF;
		}
	}
	F *= double(-m_charge);
	m_speed += F * m_invMass * elapsedTime;
}

// Strong interaction
void Object::calcStrong() {
	m_relocation = { 0, 0 };
	Vector2d r, sumOfImpulses = m_speed * m_mass;
	double length, sumOfMasses = m_mass;
	int counter = 0;
	for (auto i : objects) {
		if (i != this) {
			r = i->m_coordinates - m_coordinates;
			length = hypot(r.x, r.y);
			if (length <= rStrongInteraction) {
				++counter;
				m_relocation += r / length * ((length - rMinEnergy) / 2);
				sumOfImpulses += i->m_speed * i->m_mass;
				sumOfMasses += i->m_mass;
			}
		}
	}
	if (counter) {
		m_newSpeed = sumOfImpulses / sumOfMasses;
		m_relocation /= double(counter);
	}
	else
		m_newSpeed = m_speed;
}

void Object::move(double elapsedTime)
{
	m_speed = m_newSpeed;
	Vector2d motion = m_speed * elapsedTime + m_relocation;
	m_coordinates += motion;
	m_shape.move(Vector2f(motion));
	//text.setString(text.getString() + std::string()
}