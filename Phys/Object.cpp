#include "Object.h"

Object::Object(Vector2d coordinates, double mass, Vector2d speed, sf::Shape* shapePtr)
	: m_shape(shapePtr), m_coordinates(coordinates), m_speed(speed), m_mass(mass), m_invMass(1 / mass)
{
	m_shape->setPosition(Vector2f(coordinates));
	objects.push_back(this);
}

void Object::acceptSpeed() {
	m_speed = m_newSpeed;
}

void Object::move(double elapsedTime)
{
	m_speed = m_newSpeed;
	Vector2d motion = m_speed * elapsedTime;
	m_coordinates += motion;
	m_shape->move(Vector2f(motion));
	//text.setString(text.getString() + std::string()
}