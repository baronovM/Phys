#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

using Vector2d = sf::Vector2<double>;
using sf::Vector2f;

constexpr double INF_MASS = 0;

struct MPoint {
	Vector2d pos, vel, force;
	double mass, inv_mass;
	MPoint(double mass, Vector2d position);
	void moveEul(double deltaTime);
};

struct Spring {
	MPoint* a, * b;
	double k, l0;
	Spring(MPoint* a, MPoint* b, double k);
	void run();
};


class Object
{
public:
	std::vector<MPoint> points;
	std::vector<Spring> springs;
	double k;
	Object(double mass, double k, const std::vector<Vector2d>& points_coords, sf::RenderTarget* rendertarget);
	void draw(sf::RenderTarget& target) const;
	void moveEul(double deltaTime);

	static std::vector<Object*> objects;
	void solveCol();
	void runSprings();

	sf::RenderTarget* rendtarg;
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