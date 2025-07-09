#include "Particle.h"
#include <cmath>

Particle::Particle(float pos_x, float pos_y, float vel_x, float vel_y)
    : pos(pos_x, pos_y), velocity(vel_x, vel_y)
{
    s.setPosition(pos);
    s.setFillColor(sf::Color::White);
    s.setRadius(5);
}

void Particle::render(sf::RenderWindow& window) {
    s.setPosition(pos);
    window.draw(s);
}

void Particle::update_physics(const GravitySource& s) {
    constexpr float dt = 1.0f / 60.0f;
    constexpr float softening = 5.0f;
    constexpr float max_speed = 500.0f;      

    float dx = s.get_pos().x - pos.x;
    float dy = s.get_pos().y - pos.y;

    float distance_sq = dx * dx + dy * dy + softening * softening;
    float distance = std::sqrt(distance_sq);

    float nx = dx / distance;
    float ny = dy / distance;

    float acceleration_mag = s.get_strength() / distance_sq;

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



void Particle::set_color(sf::Color color) {
    s.setFillColor(color);
}

sf::Vector2f Particle::get_pos() const {
    return pos;
}

sf::Vector2f Particle::get_velocity() const {
    return velocity;
}
