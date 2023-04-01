#include "Object.h"


int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], sf::String("Физика", std::locale("RUS")), sf::Style::Default, settings);

	window.setFramerateLimit(150);

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text text("", font);
	text.setPosition(150, 25);

	sf::Clock clock;

	double timeSpeed = 1.0;
	bool flag_static = false;
	text.setString(std::to_string(timeSpeed));

	std::vector<Vector2d> clicks;
	

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
					for (auto i : Object::objects)
						delete i;
					Object::objects.clear();
				}
				else if (event.key.code == sf::Keyboard::S) {
					flag_static = true;
				}
			}
			else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::S) {
					flag_static = false;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				clicks.emplace_back(event.mouseButton.x, event.mouseButton.y);
				if (event.mouseButton.button == sf::Mouse::Right) {
					new Object(flag_static ? INF_MASS : 1., 25., clicks, &window);
					clicks.clear();
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
			}
		}
		window.clear();
		double elapsedTime = clock.getElapsedTime().asSeconds();
		for (auto i : Object::objects) {
			i->solveCol();
		}
		for (auto i : Object::objects) {
			i->runSprings();
		}
		for (auto i : Object::objects) {
			i->moveEul(elapsedTime * timeSpeed);
			i->draw(window);
		}
		window.draw(text);
		clock.restart();
		window.display();
	}

	return 0;
}