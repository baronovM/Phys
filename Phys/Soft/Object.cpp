#include "Object.h"

std::vector<Object*> Object::objects;

MPoint::MPoint(double mass, Vector2d position)
	: mass(mass), inv_mass(1. / mass), pos(position), vel(0., 0.), force(0., 0.) {}

void MPoint::moveEul(double deltaTime)
{
	vel += force * inv_mass * deltaTime;
	pos += vel * deltaTime;
	force.x = 0;
	force.y = 0;
}


Object::Object(double mass, double k, const std::vector<Vector2d>& points_coords, sf::RenderTarget* rendertarget)
	: k(k), rendtarg(rendertarget)
{
	int count = int(points_coords.size());
	points.reserve(count);
	for (int i = 0; i < count; ++i) {
		points.push_back(MPoint(mass / count, points_coords[i]));
	}

	objects.push_back(this);
}

void Object::draw(sf::RenderTarget& target) const
{
	sf::VertexArray conv(sf::LineStrip, points.size());
	for (int i = 0; i < points.size(); ++i) {
		conv[i].position = Vector2f(points[i].pos);
	}
	conv.append(conv[0]);
	target.draw(conv);
}

void Object::moveEul(double deltaTime)
{
	for (auto& i : points) {
		i.moveEul(deltaTime);
	}
}


bool dotinpol(const Vector2d& dot, const std::vector<MPoint>& polyg, sf::RenderTarget* rendtg) {
	// left hor
	int cnt = 0;
	for (int i = 0; i < polyg.size(); ++i) {
		Vector2d p1 = polyg[i].pos, p2 = polyg[(i + 1) % polyg.size()].pos;
		if (p1.y > p2.y)
			std::swap(p1, p2);
		if (p1.y < dot.y && p2.y > dot.y) {
			double ratio = (dot.y - p1.y) / (p2.y - p1.y),
				interx = p1.x + (p2.x - p1.x) * ratio;
			if (interx < dot.x) {
				++cnt;
				
				/*sf::CircleShape cp(5.f);
				cp.setFillColor(sf::Color::Red);
				cp.setOrigin(5.f, 5.f);
				cp.setPosition(interx, dot.y);
				rendtg->draw(cp);*/

			}
		}
	}
	return cnt & 1;
}

double len2(const Vector2d& vec) {
	return vec.x * vec.x + vec.y * vec.y;
}

double dist_to_lineseg2(Vector2d point, Vector2d a, Vector2d b) {
	if (len2(point - b) + len2(b - a) >= len2(point - a)
		&& len2(point - a) + len2(b - a) >= len2(point - b)) {

		Vector2d norm((b - a).y, -(b - a).x);
		double c = -(norm.x * a.x + norm.y * a.y);
		return sqr(norm.x * point.x + norm.y * point.y + c) / len2(norm);
	} else {
		return std::min(len2(point - a), len2(point - b));
	}
}

void Object::solveCol()
{
	for (Object* other : objects) {
		if (other == this)
			continue;
		for (MPoint& mpt : points) {
			const auto& otp = other->points;
			if (dotinpol(mpt.pos, otp, rendtarg)) {

				sf::CircleShape cp(4.f);
				cp.setFillColor(sf::Color::Red);
				cp.setOrigin(4.f, 4.f);
				cp.setPosition(Vector2f(mpt.pos));
				rendtarg->draw(cp);

				double mindist = 1e20, dist;
				int nearest = 0;
				for (int i = 0; i < otp.size(); ++i) {
					dist = dist_to_lineseg2(mpt.pos, otp[i].pos, otp[(i + 1) % otp.size()].pos);
					if (dist < mindist) {
						mindist = dist;
						nearest = i;
					}
				}

				Vector2d a(otp[nearest].pos), b(otp[(nearest + 1) % otp.size()].pos);
				sf::RectangleShape nrline(Vector2f(sqrt(len2(b - a)), 3.));
				nrline.setFillColor(sf::Color::Cyan);
				nrline.setPosition(Vector2f(a));
				double PI = acos(-1.);
				double angle = atan2((b - a).y, (b - a).x) * 180. / PI;
				nrline.rotate(angle);
				rendtarg->draw(nrline);
			}
		}
	}
}