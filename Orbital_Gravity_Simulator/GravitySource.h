#pragma once

#include <SFML/Graphics.hpp>

class GravitySource {
    sf::Vector2f pos;
    float strength;
    sf::CircleShape s;

public:
    GravitySource(float pos_x, float pos_y, float strength);
    void render(sf::RenderWindow& window);
    sf::Vector2f get_pos() const;
    float get_strength() const;
};
