#include "Object.h"

std::vector<Object*> Object::objects;

MPoint::MPoint(double mass, Vector2d position)
	: mass(mass), pos(position), vel(0., 0.), force(0., 0.) {}


Object::Object(double mass, double k, const std::vector<Vector2d>& points_coords)
	: k(k)
{
	int count = points_coords.size();
	points.reserve(count);
	for (int i = 0; i < count; ++i) {
		points.push_back(MPoint(mass / count, points_coords[i]));
	}

	objects.push_back(this);
}

void Object::draw(sf::RenderTarget& target)
{
	sf::VertexArray conv(sf::LineStrip, points.size());
	for (int i = 0; i < points.size(); ++i) {
		conv[i].position = Vector2f(points[i].pos);
	}
	conv.append(conv[0]);
	target.draw(conv);
}

/*Object::Object(Vector2d coordinates, double mass, Vector2d speed, sf::Shape* shapePtr, sf::Sprite* spritePtr)
	: coordinates(coordinates), speed(speed), mass(mass), invMass(1. / mass), shape(shapePtr), sprite(spritePtr)
{
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
	sprite->move(Vector2f(motion));
	//text.setString(text.getString() + std::string()
}*/