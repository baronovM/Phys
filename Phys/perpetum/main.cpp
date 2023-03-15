#include "Object.h"

std::vector<Object*> Object::objects;

int main()
{
	sf::ContextSettings settings;
	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Atomic", sf::Style::Fullscreen, settings);

	settings.antialiasingLevel = 8;
	window.setFramerateLimit(180);

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text text("", font);
	text.setPosition({ 150, 50 });

	//new Object({ 500, 400 }, 1, 1);
	//new Object({ 100, 400 }, -1, 0.1, { 0, 250 });
	//new Object({ 900, 400 }, -1, 0.1, { 0, -250 });
	//new Object({ 480, 380 }, 1, 1, { 220, 220 });
	//new Object({ 520, 420 }, 1, 1, { -220, -220 });
	//new Object({ 540, 360 }, 0, 1, { -100, 100 });
	Vector2d firstClick, secondClick;

	sf::Clock clock;

	double timeSpeed = 0.5;
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
				else if (event.key.code == sf::Keyboard::C)
					Object::objects.clear();
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
					new Object(firstClick, -1, 1.0, (secondClick - firstClick) * 2.0);
				if (event.mouseButton.button == sf::Mouse::Middle)
					new Object(firstClick, 0, 1.0, (secondClick - firstClick) * 2.0);
				if (event.mouseButton.button == sf::Mouse::Right)
					new Object(firstClick, +1, 1.0, (secondClick - firstClick) * 2.0);
			}
		}
		window.clear();
		double elapsedTime = clock.getElapsedTime().asSeconds();
		for (auto i : Object::objects)
			i->calcElectro(elapsedTime * timeSpeed);
		for (auto i : Object::objects)
			i->calcStrong();
		for (auto i : Object::objects) {
			i->move(elapsedTime * timeSpeed);
			i->draw(window);
		}
		window.draw(text);
		clock.restart();
		window.display();
	}

	return 0;
}