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

void Particle::update_physics(const GravitySource& source) {
    float dx = source.get_pos().x - pos.x;
    float dy = source.get_pos().y - pos.y;

    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance == 0) return;

    float inv_dist = 1.f / distance;

    float nx = inv_dist * dx;
    float ny = inv_dist * dy;

    float inv_square = inv_dist * inv_dist;

    float ax = nx * source.get_strength() * inv_square;
    float ay = ny * source.get_strength() * inv_square;

    velocity.x += ax;
    velocity.y += ay;

    pos.x += velocity.x;
    pos.y += velocity.y;
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
