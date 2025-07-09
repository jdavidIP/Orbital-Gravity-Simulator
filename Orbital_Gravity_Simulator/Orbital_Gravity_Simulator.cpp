#include <iostream>
#include <SFML/Graphics.hpp>

class GravitySource
{
	sf::Vector2f pos;
	float strength;
	sf::CircleShape s;

public:
	GravitySource(float pos_x, float pos_y, float strength)
	{
		pos.x = pos_x;
		pos.y = pos_y;
		this->strength = strength;

		s.setPosition(pos);
		s.setFillColor(sf::Color::White);
		s.setRadius(4);
	}
};

int main()
{
	int windowHeight = 1200;
	int windowWidth = 720;

	sf::RenderWindow window(sf::VideoMode(windowHeight, windowWidth), "Orbital Gravity Simulator");
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}


			window.clear();
			window.display();
		}
	}
}