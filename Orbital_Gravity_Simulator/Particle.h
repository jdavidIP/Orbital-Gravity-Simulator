#ifndef SIMULATOR_PARTICLE_H
#define SIMULATOR_PARTICLE_H

#include <SFML/Graphics.hpp>
#include "GravitySource.h"

constexpr float G = 0.03f;
constexpr float SOFTENING = 1.0f;
constexpr float dt = 1.5f;  // 0.5f, normal speed - 1.5f faster speed - 3.0f fastest speed

enum class ParticleType {
    Planetoid, 
    Satellite,
    Terrestrial, 
    GasGiant,
    IceGiant
};

class Particle {
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::CircleShape s;
    float mass;
    ParticleType type;

public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y, ParticleType type);
    /// Particle(float pos_x, float pos_y, float vel_x, float vel_y, float mass, sf::Color color);

    void render(sf::RenderWindow& window);
    void update_physics(const std::vector<Particle>& others, const std::vector<GravitySource>& sources, bool mutualGravity);

    sf::Vector2f get_pos() const;
    sf::Vector2f get_velocity() const;
    float get_mass() const;
    ParticleType get_type() const;
    float get_radius() const;
};

#endif
