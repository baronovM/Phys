#include "ChargedParticle.h"

ChargedParticle::ChargedParticle(Vector2d coordinates, short charge, double mass, Vector2d speed) :
	m_coordinates(coordinates), m_mass(mass), m_invMass(1 / mass), m_speed(speed), m_shape(10.f), m_charge(charge)
{
	m_shape.setPosition(Vector2f(coordinates));
	if (m_charge < 0)
		m_shape.setFillColor(sf::Color(0, 0, 255 + m_charge * 10));
	else if (m_charge > 0)
		m_shape.setFillColor(sf::Color(255 - m_charge * 10, 0, 0));
	else
		m_shape.setFillColor(sf::Color(125, 125, 125));
	particles.push_back(this);
}

void ChargedParticle::draw(sf::RenderTarget& target) const
{
	//if (target.getViewport(target.getDefaultView()).intersects(sf::IntRect(m_shape.getGlobalBounds())))
	target.draw(m_shape);
}

// Electromagnetic interaction
void ChargedParticle::calcElectro(double elapsedTime)
{
	if (m_charge == (short)0)
		return;
	Vector2d F(0, 0), r;
	double length, tempF;
	for (auto i : particles) {
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
void ChargedParticle::calcStrong() {
	m_relocation = { 0, 0 };
	Vector2d r, sumOfImpulses = m_speed * m_mass, norm, perp, proj1, proj2;
	double length, sumOfMasses = m_mass;
	m_newSpeed = m_speed;
	int counter = 0;
	for (auto i : particles) {
		if (i != this) {
			r = i->m_coordinates - m_coordinates;
			length = hypot(r.x, r.y);
			if (length <= rStrongInteraction) {
				++counter;
				norm = r / length;
				m_relocation += norm * ((length - rMinEnergy) / 2);

				perp.x = norm.y; perp.y = -norm.x;
				proj1 = dotProduct(m_newSpeed, norm) * norm;
				proj2 = dotProduct(i->m_speed, norm) * norm;

				m_newSpeed = dotProduct(m_newSpeed, perp) * perp + (proj1 * m_mass + proj2 * i->m_mass) / (m_mass + i->m_mass);
				//sumOfImpulses += m_speed * m_mass + i->m_speed * i->m_mass;
				//sumOfMasses += i->m_mass;
			}
		}
	}
	if (counter) {
		//m_newSpeed = sumOfImpulses / sumOfMasses;
		m_relocation /= double(counter);
	}
	//else
		//m_newSpeed = m_speed;
}

void ChargedParticle::acceptSpeed() {
	m_speed = m_newSpeed;
}

void ChargedParticle::move(double elapsedTime)
{
	m_speed = m_newSpeed;
	Vector2d motion = m_speed * elapsedTime + m_relocation;
	m_coordinates += motion;
	m_shape.move(Vector2f(motion));
	//text.setString(text.getString() + std::string()
}