#include "Object.h"


int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], sf::String("Физика", std::locale("RUS")), sf::Style::Default, settings);

	window.setFramerateLimit(90);
	const int phys_per_frame = 4;

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text text("", font);
	text.setPosition(150, 25);

	sf::Clock clock;

	double timeSpeed = 1.0;
	bool flag_static = false;
	bool mouse_pressed = false;
	text.setString(std::to_string(timeSpeed));

	std::vector<Vector2d> points;
	

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
				if (mouse_pressed)
					continue;
				mouse_pressed = true;
				points.emplace_back(event.mouseButton.x, event.mouseButton.y);
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (!mouse_pressed)
					continue;
				mouse_pressed = false;
				new Object(flag_static ? INF_MASS : 1., 25., points, &window);
				points.clear();
			}
			else if (mouse_pressed && event.type == sf::Event::MouseMoved) {
				Vector2d mouse_move = Vector2d(event.mouseMove.x, event.mouseMove.y);
				if (points.empty() || len2(mouse_move - points[points.size() - 1]) >= sqr(20.)) {
					points.emplace_back(mouse_move);
				}
			}
			/*
			else if (event.type == sf::Event::MouseButtonPressed) {
				points.emplace_back(event.mouseButton.x, event.mouseButton.y);
				if (event.mouseButton.button == sf::Mouse::Right) {
					new Object(flag_static ? INF_MASS : 1., 150., points, &window);
					points.clear();
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
			}*/
		}
		window.clear();
		for (int i = 0; i < phys_per_frame; ++i) {
			for (auto i : Object::objects) {
				i->solveCol();
			}
			for (auto i : Object::objects) {
				i->run();
			}
			double elapsedTime = clock.getElapsedTime().asSeconds();
			for (auto i : Object::objects) {
				i->moveEul(elapsedTime * timeSpeed);
			}
		}
		for (auto i : Object::objects) {
			i->draw(window);
		}

		sf::VertexArray conv(sf::LineStrip, points.size());
		for (int i = 0; i < points.size(); ++i) {
			conv[i].position = Vector2f(points[i]);
		}
		window.draw(conv);


		window.draw(text);
		clock.restart();
		window.display();
	}

	return 0;
}