#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

typedef sf::Vector2<double> Vector2d;
using sf::Vector2f;

class Object
{
public:
	static std::vector<Object*> objects;
	Object(Vector2d coordinates, double mass, Vector2d speed, sf::Shape* shapePtr);
	virtual void draw(sf::RenderTarget& target) const = 0;
	void acceptSpeed();
	void move(double elapsedTime);
protected:
	sf::Shape* m_shape;
	Vector2d m_coordinates;
	Vector2d m_speed;
	Vector2d m_newSpeed;
	double m_mass, m_invMass;
};


template<class T>
T sqr(T x) {
	return x * x;
}

template<class T>
T cube(T x) {
	return x * x * x;
}

template<class T>
T dotProduct(const sf::Vector2<T>& left, const sf::Vector2<T>& right) {
	return left.x * right.x + left.y * right.y;
}