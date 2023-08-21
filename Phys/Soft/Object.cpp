#include "Object.h"

std::vector<Object*> Object::objects;


MPoint::MPoint(double mass, Vector2d position)
	: mass(mass), inv_mass(mass == INF_MASS ? 0. : 1. / mass), pos(position), vel(0., 0.), force(0., 0.) {}

void MPoint::moveEul(double deltaTime)
{
	force -= air_resist_lin * vel + air_resist_sqr * vel * sqrt(len2(vel));
	vel += force * inv_mass * deltaTime;
	pos += vel * deltaTime;
	force.x = 0;
	force.y = g * mass;
}


Spring::Spring(MPoint* a, MPoint* b, double k) :
	a(a), b(b), k(k), l0(sqrt(len2(b->pos - a->pos))) {}

void Spring::run()
{
	Vector2d p1 = a->pos, p2 = b->pos;
	double l = sqrt(len2(p2 - p1));
	double f = (l - l0) * k;
	Vector2d force = (p2 - p1) / l * f;
	a->force += force;
	b->force -= force;
}


Object::Object(double mass, double k, const std::vector<Vector2d>& points_coords, sf::RenderTarget* rendertarget)
	: k(k), rendtarg(rendertarget)
{
	int count = int(points_coords.size());
	points.reserve(count);
	springs.reserve(count);
	double m1 = mass / count, k1 = 2. * k / (count * count);
	for (int i = 0; i < count; ++i) {
		points.emplace_back(m1, points_coords[i]);
	}
	startArea = calcArea();
	for (int i = 0; i < count; ++i) {
		for (int j = i + 1; j < count; ++j)
			springs.emplace_back(&points[i], &points[j], k1);
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

void Object::run()
{
	for (int i = 0; i < springs.size(); ++i) {
		springs[i].run();
	}


	double area = calcArea(), deltaP = p0 * (startArea / area - 1), force;
	Vector2d edge, norm;
	for (int i = 0; i < points.size(); ++i) {
		edge = points[(i + 1) % points.size()].pos - points[i].pos;
		norm.x = -edge.y, norm.y = edge.x;
		norm /= sqrt(len2(norm));
		force = deltaP * sqrt(len2(edge)) / 2;
		points[i].force += force * norm;
		points[(i + 1) % points.size()].force += force * norm;
	}

}

double Object::calcArea()
{
	double s = 0;
	for (int i = 0; i < points.size() - 1; ++i) {
		s += crossProduct(points[i].pos, points[i + 1].pos);
	}
	s += crossProduct(points.back().pos, points.front().pos);
	return abs(s / 2);
}




double dist_to_line2(Vector2d point, Vector2d n, double c) {
	return sqr(n.x * point.x + n.y * point.y + c) / len2(n);
}

double dist_to_lineseg2(Vector2d point, Vector2d a, Vector2d b) {
	if (len2(point - b) + len2(b - a) >= len2(point - a)
		&& len2(point - a) + len2(b - a) >= len2(point - b)) {

		Vector2d norm((b - a).y, -(b - a).x);
		double c = -(norm.x * a.x + norm.y * a.y);
		return dist_to_line2(point, norm, c);
	}
	else {
		return std::min(len2(point - a), len2(point - b));
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
				cp.setFillColor(sf::Color::Magenta);
				cp.setOrigin(5.f, 5.f);
				cp.setPosition(interx, dot.y);
				rendtg->draw(cp);*/

			}
		}
	}
	return cnt & 1;
}

std::pair<Vector2d, Vector2d>* centr_impact(const MPoint& a, const MPoint& b, Vector2d n) {
	auto deltaVptr = new std::pair<Vector2d, Vector2d>;
	n /= sqrt(len2(n));
	double v1 = dotProduct(a.vel, n);
	double v2 = dotProduct(b.vel, n);

	double dv1, dv2;

	if (a.inv_mass == 0.) {
		dv1 = 0.;
		dv2 = (1. + kresb) * (v1 - v2);
	}
	else if (b.inv_mass == 0.) {
		dv1 = (1. + kresb) * (v2 - v1);
		dv2 = 0.;
	}
	else {
		double sminv = 1. / (a.mass + b.mass);
		dv1 = (1. + kresb) * b.mass * sminv * (v2 - v1);
		dv2 = (1. + kresb) * a.mass * sminv * (v1 - v2);
	}
	deltaVptr->first = n * dv1;
	deltaVptr->second = n * dv2;
	return deltaVptr;
}




void Object::solveCol()
{
	for (Object* other : objects) {
		if (other == this)
			continue;
		for (MPoint& mpt : points) {
			auto& otp = other->points;
			if (dotinpol(mpt.pos, otp, rendtarg)) {

				/*sf::CircleShape cp(4.f);
				cp.setFillColor(sf::Color::Red);
				cp.setOrigin(4.f, 4.f);
				cp.setPosition(Vector2f(mpt.pos));
				rendtarg->draw(cp);*/

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

				/*sf::RectangleShape nrline(Vector2f(sqrt(len2(b - a)), 3.));
				nrline.setFillColor(sf::Color::Cyan);
				nrline.setPosition(Vector2f(a));
				double PI = acos(-1.);
				double angle = atan2((b - a).y, (b - a).x) * 180. / PI;
				nrline.rotate(angle);
				rendtarg->draw(nrline);*/

				double c = -((b - a).x * mpt.pos.x + (b - a).y * mpt.pos.y);
				double ratio = sqrt(dist_to_line2(a, b - a, c) / len2(b - a));	//   !!!!!

				Vector2d nrpt = a + (b - a) * ratio;

				/*cp.setFillColor(sf::Color::Yellow);
				cp.setPosition(Vector2f(nrpt));
				rendtarg->draw(cp);*/

				MPoint& p1 = otp[nearest], & p2 = otp[(nearest + 1) % otp.size()];

				if (mpt.inv_mass == 0. && (p1.inv_mass == 0. || p2.inv_mass == 0.))
					continue;

				double r2;
				if (mpt.inv_mass == 0.)
					r2 = 0.;
				else if (p1.inv_mass == 0. || p2.inv_mass == 0.)
					r2 = 1.;
				else
					r2 = (p1.mass + p2.mass) / (mpt.mass + p1.mass + p2.mass);
				
				Vector2d shift = nrpt - mpt.pos;
				mpt.pos += shift * r2;

				Vector2d s0 = -shift * (1. - r2);
				double antidiv = 1. / (sqr(1. - ratio) + sqr(ratio));
				p1.pos += s0 * (1. - ratio) * antidiv;	//				!!!!!!
				p2.pos += s0 * ratio * antidiv;			//				!!!!!!

				MPoint edge(INF_MASS, (p1.pos + p2.pos) / 2.);
				edge.vel = p1.vel;						//  !!!!!!
				if (p1.inv_mass != 0. && p2.inv_mass != 0.) {
					double efm1 = p1.mass * ratio, efm2 = p2.mass * (1. - ratio),
						invsm = 1. / (efm1 + efm2);
					edge.mass = efm1 + efm2;
					edge.pos = (p1.pos * efm1 + p2.pos * efm2) * invsm;
					edge.vel = (p1.vel * efm1 + p2.vel * efm2) * invsm;
				}
				std::pair<Vector2d, Vector2d>* dvptr = centr_impact(mpt, edge, shift);

				mpt.vel += dvptr->first;
				p1.vel += dvptr->second * 2. * ratio;				//			!!!!!!!!!
				p2.vel += dvptr->second * 2. * (1. - ratio);		//			!!!!!!!!!
				delete dvptr;
			}
		}
	}
}