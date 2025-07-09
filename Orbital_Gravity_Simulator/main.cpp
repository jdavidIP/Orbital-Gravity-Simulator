#include <iostream>
#include <SFML/Graphics.hpp>

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1200

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
		s.setRadius(10);
	}

	void render(sf::RenderWindow& window)
	{
		window.draw(s);
	}

	sf::Vector2f get_pos()
	{
		return pos;
	}

	float get_strength()
	{
		return strength;
	}
};

class Particle
{
	sf::Vector2f pos;
	sf::Vector2f velocity;
	sf::CircleShape s;

public:
	Particle(float pos_x, float pos_y, float vel_x, float vel_y)
	{
		pos.x = pos_x;
		pos.y = pos_y;
		
		velocity.x = vel_x;
		velocity.y = vel_y;

		s.setPosition(pos);
		s.setFillColor(sf::Color::White);
		s.setRadius(8);
	}

	void render(sf::RenderWindow& window)
	{
		s.setPosition(pos);
		window.draw(s);
	}

	void update_physics(GravitySource& s)
	{
		float distance_x = s.get_pos().x - pos.x;
		float distance_y = s.get_pos().y - pos.y;

		float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

		float inverse_distance = 1.f / distance;

		float normalized_x = inverse_distance * distance_x;
		float normalized_y = inverse_distance * distance_y;

		float inverse_square_dropoff = inverse_distance * inverse_distance;

		float acceleration_x = normalized_x * s.get_strength() * inverse_square_dropoff;
		float acceleration_y = normalized_y * s.get_strength() * inverse_square_dropoff;

		velocity.x += acceleration_x;
		velocity.y += acceleration_y;

		pos.x += velocity.x;
		pos.y += velocity.y;
	}

	sf::Vector2f get_pos()
	{
		return pos;
	}

	sf::Vector2f get_velocity()
	{
		return velocity;
	}
};

int main()
{

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Orbital Gravity Simulator");
	window.setFramerateLimit(60);

	GravitySource source(800, 500, 7000);

	Particle particle(600, 700, 4, 0);

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

		}


		window.clear();

		particle.update_physics(source);
		
		source.render(window);
		particle.render(window);
		window.display();
	}
}