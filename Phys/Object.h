#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

using Vector2d = sf::Vector2<double>;
using sf::Vector2f;

const bool borders = true;

class Object
{
public:
	static std::vector<Object*> objects;
	Object(Vector2d coordinates, double mass, Vector2d speed, sf::Shape* shapePtr, sf::Sprite* spritePtr);
	virtual void draw(sf::RenderTarget& target) const = 0;
	void acceptSpeed();
	void move(double elapsedTime);
protected:
	std::unique_ptr<sf::Shape> shape;
	std::unique_ptr<sf::Sprite> sprite;
	Vector2d coordinates;
	Vector2d speed;
	Vector2d newSpeed;
	double mass, invMass;
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