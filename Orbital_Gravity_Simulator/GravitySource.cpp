#include "GravitySource.h"

GravitySource::GravitySource(float pos_x, float pos_y, float strength)
    : pos(pos_x, pos_y), strength(strength)
{
    s.setPosition(pos);
    s.setFillColor(sf::Color::White);
    s.setRadius(10);
}

void GravitySource::render(sf::RenderWindow& window) {
    window.draw(s);
}

sf::Vector2f GravitySource::get_pos() const {
    return pos;
}

float GravitySource::get_strength() const {
    return strength;
}
