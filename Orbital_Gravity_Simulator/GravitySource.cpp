#include "GravitySource.h"

GravitySource::GravitySource(float pos_x, float pos_y, GravitySourceType type)
    : pos(pos_x, pos_y), type(type)
{
    s.setPosition(pos);
    
    switch (type) {
    case GravitySourceType::RedDwarf:
        strength = 25000.0f;
        s.setFillColor(sf::Color::Red);
        s.setRadius(178.8f);
        break;

    case GravitySourceType::WhiteDwarf:
        strength = 56000.0f;
        s.setFillColor(sf::Color::White);
        s.setRadius(11.66f);
        break;

    case GravitySourceType::YellowDwarf:
        strength = 333000.0f;
        s.setFillColor(sf::Color(139, 128, 0));
        s.setRadius(1160.56f);
        break;

    case GravitySourceType::NeutronStar:
        strength = 450000.0f;
        s.setFillColor(sf::Color(175, 238, 238));
        s.setRadius(0.023f);
        break;

    default:
        strength = 0.0f;
        break;
    }
}

void GravitySource::render(sf::RenderWindow& window) {
    s.setOrigin(s.getRadius(), s.getRadius());
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