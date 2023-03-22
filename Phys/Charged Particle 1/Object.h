#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

const double rStrongInteraction = 24.0;
const double rMinEnergy = 21.0;
const double invKsqrt = 0.0005;

typedef sf::Vector2<double> Vector2d;
using sf::Vector2f;

class Object
{
public:
	static std::vector<Object*> objects;
	Object(Vector2d coordinates, short charge, double mass, Vector2d speed = { 0.0, 0.0 });
	void draw(sf::RenderTarget& target) const;
	void calcElectro(double elapsedTime);
	void calcStrong();
	void move(double elapsedTime);
private:
	sf::CircleShape m_shape;
	Vector2d m_coordinates;
	Vector2d m_speed;
	Vector2d m_newSpeed;
	Vector2d m_relocation;
	double m_mass, m_invMass;
	short m_charge;
};


template<class T>
T sqr(T x) {
	return x * x;
}

template<class T>
T cube(T x) {
	return x * x * x;
}