#pragma once

#include <SFML/Graphics.hpp>
#include "GravitySource.h"

class Particle {
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::CircleShape s;

public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y);

    void render(sf::RenderWindow& window);
    void update_physics(const GravitySource& s);
    void set_color(sf::Color color);

    sf::Vector2f get_pos() const;
    sf::Vector2f get_velocity() const;
};
