#include <iostream>
#include <vector>
#include <cstdlib> 
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
    int num_particles;
    std::cout << "Enter a number of particles: ";
    std::cin >> num_particles;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Orbital Gravity Simulator");
    window.setFramerateLimit(60);

    // Text
    sf::Font open_sans;
    open_sans.loadFromFile("C:\\Users\\Ibapo\\OneDrive\\Escritorio\\Projects\\SMFL_Learning\\SFML_FONTS\\Open_Sans\\OpenSans-VariableFont_wdth,wght.ttf");

    sf::Text my_text;
    my_text.setFont(open_sans);
    my_text.setString("Press Enter to see a cool thing :)");
    my_text.setPosition(600, 360);
    my_text.setFillColor(sf::Color::White);

    std::vector<GravitySource> sources = {
        GravitySource(500, 500, 7000),
        GravitySource(1200, 500, 700)
    };

    std::vector<Particle> particles;

    for (int i = 0; i < num_particles; ++i) {
        particles.emplace_back(600, 700, 4, 0.2 + (0.1 / num_particles) * i);
        float value = static_cast<float>(i) / num_particles;
        particles[i].set_color(map_value_to_color(value));
    }

    bool start = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                start = true;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                sf::Vector2f spawn_pos(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y));

                // Example initial velocity: small random push
                float vel_x = static_cast<float>(std::rand() % 100 - 50) / 50.0f;  // -1.0 to +1.0
                float vel_y = static_cast<float>(std::rand() % 100 - 50) / 50.0f;

                particles.emplace_back(spawn_pos.x, spawn_pos.y, vel_x, vel_y);
                // Optional: color based on total count
                float value = static_cast<float>(particles.size()) / (particles.size() + 100);
                particles.back().set_color(map_value_to_color(value));
            }
        }
       
        window.clear();

        if (start) {
            for (const auto& source : sources) {
                for (auto& particle : particles) {
                    particle.update_physics(source);
                }
            }
        }
        else
        {
            window.draw(my_text);
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
