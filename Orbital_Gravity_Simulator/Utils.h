#ifndef SIMULATOR_UTILS_H
#define SIMULATOR_UTILS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Particle.h"
#include "GravitySource.h"
#include "AppState.h"

void addParticlesAtPosition(std::vector<Particle>& particles, sf::Vector2f pos, int count, int i, ParticleType type, const GravitySource& source);
void updateParticles(std::vector<Particle>& particles, const std::vector<GravitySource>& sources, bool mutualGravity);
void renderScene(
    AppState state,
    const std::vector<sf::Text>& particleTypes,
    const std::vector<sf::Text>& sourceTypes,
    const ParticleType particleType,
    const GravitySourceType sourceType,
    sf::Text& instructions,
    Mode mode,
    sf::RenderWindow& window,
    bool pause,
    bool mutualGravity,
    std::vector<GravitySource>& sources,
    std::vector<Particle>& particles
);
void renderTypes(
    const std::vector<sf::Text>& particleTypes,
    const std::vector<sf::Text>& sourceTypes,
    const ParticleType particleType,
    const GravitySourceType sourceType,
    Mode mode,
    sf::RenderWindow& window);
GravitySource* findNearestSource(sf::Vector2f pos, std::vector<GravitySource>& sources);

#endif