#ifndef SIMULATOR_UTILS_H
#define SIMULATOR_UTILS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Particle.h"
#include "GravitySource.h"
#include "AppState.h"

sf::Color map_value_to_color(float value);
void addParticlesAtPosition(std::vector<Particle>& particles, sf::Vector2f pos, int count);
void updateParticles(std::vector<Particle>& particles, const std::vector<GravitySource>& sources);
void renderScene(
    AppState state,
    const std::string& userInput_num_particles,
    sf::Text& inputText_num_particles,
    sf::Text& instructions,
    Mode mode,
    sf::RenderWindow& window,
    int num_particles,
    bool pause,
    std::vector<GravitySource>& sources,
    std::vector<Particle>& particles
);

#endif