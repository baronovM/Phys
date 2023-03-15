#include "Solid.h"

std::vector<Object*> Object::objects;
std::vector<Solid*> Solid::solids;

int main()
{
	sf::ContextSettings settings;
	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], sf::String("Физика", std::locale("RUS")), sf::Style::Default, settings);

	settings.antialiasingLevel = 8;
	window.setFramerateLimit(100);
	const int calcCnt = 3;

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text text("", font), text2("", font);
	text.setPosition({ 150, 25 });
	text2.setPosition({ 150, 50 });

	Vector2d firstClick, secondClick;
	//double firstClickTime, secondClickTime;
	sf::Clock clickClock;

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
					for (auto i : Object::objects)
						delete i;
					Object::objects.clear();
					Solid::solids.clear();
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				firstClick.x = event.mouseButton.x;
				firstClick.y = event.mouseButton.y;
				clickClock.restart();
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				double clickdur = clickClock.getElapsedTime().asSeconds();;
				if (event.mouseButton.button == sf::Mouse::Left) {
					secondClick.x = event.mouseButton.x;
					secondClick.y = event.mouseButton.y;
				} else {
					secondClick = firstClick;
				}
				new Solid(firstClick, 1. + clickdur, 8. * sqrt(1. + clickdur), (secondClick - firstClick));
				text2.setString(std::to_string(Solid::solids.size()));
			}
		}
		window.clear();
		double elapsedTime = clock.getElapsedTime().asSeconds();
		for (int k = 0; k < calcCnt; ++k) {
			for (auto i : Solid::solids)
				i->calcCollisions();
			for (auto i : Solid::solids)
				i->acceptSpeed();
		}
		for (auto i : Object::objects) {
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