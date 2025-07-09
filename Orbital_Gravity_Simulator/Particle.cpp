#include "Particle.h"
#include <cmath>

Particle::Particle(float pos_x, float pos_y, float vel_x, float vel_y, float mass)
    : pos(pos_x, pos_y), velocity(vel_x, vel_y), mass(mass)
{
    s.setPosition(pos);
    s.setFillColor(sf::Color::White);
    s.setRadius(5);
}

void Particle::render(sf::RenderWindow& window) {
    s.setPosition(pos);
    window.draw(s);
}

void Particle::update_physics(const GravitySource& source) {
    constexpr float dt = 1.0f / 60.0f;
    constexpr float softening = 5.0f;
    constexpr float max_speed = 500.0f;

    float dx = source.get_pos().x - pos.x;
    float dy = source.get_pos().y - pos.y;
    float distance_sq = dx * dx + dy * dy + softening * softening;
    float distance = std::sqrt(distance_sq);

    float nx = dx / distance;
    float ny = dy / distance;

    // Acceleration: F = G * M_source * m / r², a = F / m = G * M_source / r²
    float acceleration_mag = source.get_strength() / distance_sq;

    velocity.x += nx * acceleration_mag * dt;
    velocity.y += ny * acceleration_mag * dt;

    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > max_speed) {
        velocity.x *= max_speed / speed;
        velocity.y *= max_speed / speed;
    }

    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;
}

void Particle::update_physics(const std::vector<Particle>& others, const std::vector<GravitySource>& sources, bool mutualGravity) {
    constexpr float G = 1000.0f;
    constexpr float dt = 1.0f / 60.0f;
    constexpr float softening = 5.0f;
    constexpr float max_speed = 500.0f;

    sf::Vector2f acceleration(0.f, 0.f);

    // Gravity from GravitySources
    for (const auto& source : sources) {
        float dx = source.get_pos().x - pos.x;
        float dy = source.get_pos().y - pos.y;
        float distance_sq = dx * dx + dy * dy + softening * softening;
        float distance = std::sqrt(distance_sq);

        float nx = dx / distance;
        float ny = dy / distance;

        float a_mag = source.get_strength() / distance_sq;
        acceleration.x += nx * a_mag;
        acceleration.y += ny * a_mag;
    }

    // Mutual gravity between particles
    if (mutualGravity) {
        for (const auto& other : others) {
            if (&other == this) continue; // Skip self

            float dx = other.get_pos().x - pos.x;
            float dy = other.get_pos().y - pos.y;
            float distance_sq = dx * dx + dy * dy + softening * softening;
            float distance = std::sqrt(distance_sq);

            float nx = dx / distance;
            float ny = dy / distance;

            // a = G * m_other / r²
            float a_mag = G * other.get_mass() / distance_sq;
            acceleration.x += nx * a_mag;
            acceleration.y += ny * a_mag;
        }
    }

    velocity.x += acceleration.x * dt;
    velocity.y += acceleration.y * dt;

    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > max_speed) {
        velocity.x *= max_speed / speed;
        velocity.y *= max_speed / speed;
    }

    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;
}

void Particle::set_color(sf::Color color) {
    s.setFillColor(color);
}

sf::Vector2f Particle::get_pos() const {
    return pos;
}

sf::Vector2f Particle::get_velocity() const {
    return velocity;
}

float Particle::get_mass() const {
    return mass;
}