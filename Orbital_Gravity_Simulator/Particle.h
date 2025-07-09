#ifndef SIMULATOR_PARTICLE_H
#define SIMULATOR_PARTICLE_H

#include <SFML/Graphics.hpp>
#include "GravitySource.h"

class Particle {
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::CircleShape s;
    float mass;

public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y, float mass);

    void render(sf::RenderWindow& window);
    void update_physics(const GravitySource& s);
    void update_physics(const std::vector<Particle>& others, const std::vector<GravitySource>& sources, bool mutualGravity);
    void set_color(sf::Color color);

    sf::Vector2f get_pos() const;
    sf::Vector2f get_velocity() const;
    float get_mass() const;
};

#endif
