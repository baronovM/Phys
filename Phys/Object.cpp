#include "Object.h"

Object::Object(Vector2d coordinates, double mass, Vector2d speed, sf::Shape* shapePtr)
	: shape(shapePtr), coordinates(coordinates), speed(speed), mass(mass), invMass(1. / mass)
{
	shape->setPosition(Vector2f(coordinates));
	objects.push_back(this);
}

void Object::acceptSpeed() {
	speed = newSpeed;
}

void Object::move(double elapsedTime)
{
	speed = newSpeed;
	Vector2d motion = speed * elapsedTime;
	coordinates += motion;
	shape->move(Vector2f(motion));
	//text.setString(text.getString() + std::string()
}