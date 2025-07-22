#ifndef SIMULATOR_GRAVITYSOURCE_H
#define SIMULATOR_GRAVITYSOURCE_H

#include <SFML/Graphics.hpp>

enum class GravitySourceType {
    RedDwarf,
    WhiteDwarf,
    YellowDwarf,
    NeutronStar
};

class GravitySource {
private:
    sf::Vector2f pos;
    float strength;
    sf::CircleShape s;
    GravitySourceType type;

public:
    GravitySource(float pos_x, float pos_y, GravitySourceType type);
    GravitySource(float pos_x, float pos_y, float strength);
    void render(sf::RenderWindow& window);

    
    sf::Vector2f get_pos() const;
    float get_strength() const;
    float get_radius() const;
    GravitySourceType get_type() const;

    void set_pos(float pos_x, float pos_y);
    void set_strength(float strength);
};

#endif