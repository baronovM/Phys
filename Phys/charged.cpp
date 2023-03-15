#include "ChargedParticle.h"

std::vector<ChargedParticle*> ChargedParticle::particles;

int main()
{
	sf::ContextSettings settings;
	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Atomic", sf::Style::Fullscreen, settings);

	settings.antialiasingLevel = 8;
	window.setFramerateLimit(120);

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text text("", font), text2("", font);
	text.setPosition({ 150, 25 });
	text2.setPosition({ 150, 50 });

	new ChargedParticle({ 500, 400 }, +1, 1.0);
	new ChargedParticle({ 100, 400 }, -1, 0.1, { 0, 250 });
	new ChargedParticle({ 900, 400 }, -1, 0.1, { 0, -250 });
	//new ChargedParticle({ 500, 800 }, -1, 0.1, { 50, 0 });
	//new ChargedParticle({ 500, 0 }, -1, 0.1, { -50, 0 });
	//new ChargedParticle({ 480, 380 }, 1, 1, { 220, 220 });
	//new ChargedParticle({ 520, 420 }, 1, 1, { -220, -220 });
	//new ChargedParticle({ 540, 360 }, 0, 1, { -100, 100 });
	Vector2d firstClick, secondClick;

	sf::Clock clock;

	double timeSpeed = 1.0;
	text.setString(std::to_string(timeSpeed));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				else if (event.key.code == sf::Keyboard::Comma) {
					timeSpeed /= 2.0;
					text.setString(std::to_string(timeSpeed));
				}
				else if (event.key.code == sf::Keyboard::Period) {
					timeSpeed *= 2.0;
					text.setString(std::to_string(timeSpeed));
				}
				else if (event.key.code == sf::Keyboard::Space)
					std::cout << 1;
				else if (event.key.code == sf::Keyboard::C) {
					for (auto i : ChargedParticle::particles)
						delete i;
					ChargedParticle::particles.clear();
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				//if (event.mouseButton.button == sf::Mouse::Left) {
				firstClick.x = event.mouseButton.x;
				firstClick.y = event.mouseButton.y;
				//}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				secondClick.x = event.mouseButton.x;
				secondClick.y = event.mouseButton.y;
				if (event.mouseButton.button == sf::Mouse::Left)
					new ChargedParticle(firstClick, -1, 0.1, (secondClick - firstClick));
				if (event.mouseButton.button == sf::Mouse::Middle)
					new ChargedParticle(firstClick, +0, 1.0, (secondClick - firstClick));
				if (event.mouseButton.button == sf::Mouse::Right)
					new ChargedParticle(firstClick, +1, 1.0, (secondClick - firstClick));
				text2.setString(std::to_string(ChargedParticle::particles.size()));
			}
		}
		window.clear();
		double elapsedTime = clock.getElapsedTime().asSeconds();
		for (auto i : ChargedParticle::particles)
			i->calcElectro(elapsedTime * timeSpeed);
		for (int k = 0; k < 3; ++k) {
			for (auto i : ChargedParticle::particles)
				i->calcStrong();
			for (auto i : ChargedParticle::particles)
				i->acceptSpeed();
		}
		for (auto i : ChargedParticle::particles) {
			i->move(elapsedTime * timeSpeed);
			i->draw(window);
		}
		window.draw(text);
		window.draw(text2);
		clock.restart();
		window.display();
	}

	return 0;
}