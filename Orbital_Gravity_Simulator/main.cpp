#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GravitySource.h"
#include "Particle.h"

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 1400

sf::Color map_value_to_color(float value) {
    if (value < 0.0f) value = 0;
    else if (value > 1.0f) value = 1;

    int r = 0, g = 0, b = 0;

    if (value < 0.5f) {
        b = 255 * (1.0f - 2 * value);
        g = 255 * 2 * value;
    }
    else {
        g = 255 * (2.0f - 2 * value);
        r = 255 * (2 * value - 1);
    }

    return sf::Color(r, g, b);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Orbital Gravity Simulator");
    window.setFramerateLimit(60);

    std::vector<GravitySource> sources = {
        GravitySource(500, 500, 7000),
        GravitySource(1200, 500, 700)
    };

    int num_particles = 1000;
    std::vector<Particle> particles;

    for (int i = 0; i < num_particles; ++i) {
        particles.emplace_back(600, 700, 4, 0.2 + (0.1 / num_particles) * i);
        float value = static_cast<float>(i) / num_particles;
        particles[i].set_color(map_value_to_color(value));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        window.clear();

        for (const auto& source : sources) {
            for (auto& particle : particles) {
                particle.update_physics(source);
            }
        }

        for (auto& source : sources) {
            source.render(window);
        }

        for (auto& particle : particles) {
            particle.render(window);
        }

        window.display();
    }

    return 0;
}
