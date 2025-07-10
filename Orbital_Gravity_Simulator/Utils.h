#ifndef SIMULATOR_UTILS_H
#define SIMULATOR_UTILS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Particle.h"
#include "GravitySource.h"
#include "AppState.h"

sf::Color map_value_to_color(float value);
void addParticlesAtPosition(std::vector<Particle>& particles, sf::Vector2f pos, int count, float min_mass, float max_mass, const GravitySource& source);
void updateParticles(std::vector<Particle>& particles, const std::vector<GravitySource>& sources, bool mutualGravity);
void renderScene(
    AppState state,
    const std::string& userInput_num_particles,
    const std::string& userInput_min_mass,
    const std::string& userInput_max_mass,
    sf::Text& inputText_num_particles,
    sf::Text& instructions,
    Mode mode,
    sf::RenderWindow& window,
    int num_particles,
    bool pause,
    bool mutualGravity,
    std::vector<GravitySource>& sources,
    std::vector<Particle>& particles
);
GravitySource* findNearestSource(sf::Vector2f pos, std::vector<GravitySource>& sources);

#endif