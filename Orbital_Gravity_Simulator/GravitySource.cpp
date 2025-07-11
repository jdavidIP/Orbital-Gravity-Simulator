#include "GravitySource.h"

GravitySource::GravitySource(float pos_x, float pos_y, GravitySourceType type)
    : pos(pos_x, pos_y), type(type)
{
    s.setPosition(pos);
    
    switch (type) {
    case GravitySourceType::RedDwarf:
        strength = 25000.0f;
        s.setFillColor(sf::Color::Red);
        s.setRadius(24);
        break;

    case GravitySourceType::WhiteDwarf:
        strength = 56000.0f;
        s.setFillColor(sf::Color::White);
        s.setRadius(1.2);
        break;

    case GravitySourceType::YellowDwarf:
        strength = 333000.0f;
        s.setFillColor(sf::Color(139, 128, 0));
        s.setRadius(60);
        break;

    case GravitySourceType::NeutronStar:
        strength = 450000.0f;
        s.setFillColor(sf::Color(216, 222, 236));
        s.setRadius(14);
        break;

    default:
        strength = 0.0f;
        break;
    }
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

GravitySourceType GravitySource::get_type() const {
    return type;
}