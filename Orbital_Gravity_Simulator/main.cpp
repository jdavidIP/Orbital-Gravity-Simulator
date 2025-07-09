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
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Orbital Gravity Simulator");
    window.setFramerateLimit(60);

    sf::Font open_sans;
    if (!open_sans.loadFromFile("C:\\Users\\Ibapo\\OneDrive\\Escritorio\\Projects\\SMFL_Learning\\SFML_FONTS\\Open_Sans\\OpenSans-VariableFont_wdth,wght.ttf")) {
        std::cerr << "Failed to load font!\n";
        return -1;
    }

    std::vector<GravitySource> sources;
    std::vector<Particle> particles;

    std::string userInput_num_particles;

    sf::Text inputText_num_particles;
    inputText_num_particles.setFont(open_sans);
    inputText_num_particles.setCharacterSize(24);
    inputText_num_particles.setFillColor(sf::Color::White);
    inputText_num_particles.setPosition(20, 20);

    sf::Text instructions;
    instructions.setFont(open_sans);
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition(20, 20);

    bool start = false;
    bool pause = false;
    bool gotInput = false;
    bool gotSpawnPos = false;

    int num_particles = 0;
    sf::Vector2f spawn_pos;

    bool add_source = false;
    bool add_particle = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            if (!gotInput) {
                // Handle user typing number of particles
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b') { // Backspace
                        if (!userInput_num_particles.empty())
                            userInput_num_particles.pop_back();
                    }
                    else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                        userInput_num_particles += static_cast<char>(event.text.unicode);
                    }
                    else if (event.text.unicode == '\r' || event.text.unicode == '\n') { // Enter key
                        if (!userInput_num_particles.empty()) {
                            num_particles = std::stoi(userInput_num_particles);
                            if (num_particles > 0) {
                                gotInput = true;
                            }
                        }
                    }
                }
            }
            else if (!gotSpawnPos) {
                // Wait for user to click spawn position for all particles
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    spawn_pos = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    for (int i = 0; i < num_particles; ++i) {
                        float vel_x = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
                        float vel_y = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
                        particles.emplace_back(spawn_pos.x, spawn_pos.y, vel_x, vel_y);
                        float value = static_cast<float>(i) / num_particles;
                        particles[i].set_color(map_value_to_color(value));
                    }
                    gotSpawnPos = true;
                }
            }
            else {
                // At least one gravity source has to be created before simulation starts
                if (sources.size() <= 0)
                {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2f pos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                        sources.emplace_back(pos.x, pos.y, 7000);
                    }

                }
                else if (!start && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    start = true;
                    pause = false;
                }

                if (start) {
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                        pause = !pause;
                    }

                    // Add new particles or sources during simulation
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2f pos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                        if (add_particle) {
                            float vel_x = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
                            float vel_y = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
                            particles.emplace_back(pos.x, pos.y, vel_x, vel_y);
                            float value = static_cast<float>(particles.size()) / (particles.size() + 100);
                            particles.back().set_color(map_value_to_color(value));
                        }
                        else {
                            sources.emplace_back(pos.x, pos.y, 7000);
                        }
                    }
                } 
                else
                {
                    continue;
                }
            }

            // Mode switching (outside above condition to allow anytime)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                add_particle = true;
                add_source = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                add_particle = false;
                add_source = true;
            }
        }

        // Rendering
        window.clear();

        if (!gotInput) {
            inputText_num_particles.setString(
                "Enter number of particles: " + userInput_num_particles + "\nPress Enter to confirm."
            );
            window.draw(inputText_num_particles);
        }
        else if (!gotSpawnPos) {
            instructions.setString(
                "Click to choose spawn position for all " + std::to_string(num_particles) + " particles."
            );
            window.draw(instructions);
        }
        else if (sources.size() <= 0)
        {
            instructions.setString(
                "Click to spawn a gravity source."
            );
            window.draw(instructions);
        }
        else if (!start) {
            instructions.setString(
                "Press Enter to start the simulation."
            );
            window.draw(instructions);
        }
        else if (pause) {
            instructions.setString(
                "Simulation is paused\n"
                "Left-click: Add " + std::string(add_particle ? "Particle" : "Gravity Source") + "\n" +
                "Press P: Switch to Particle Mode\n" +
                "Press S: Switch to Gravity Source Mode\n" +
                "Press Space: Resume Simulation\n" +
                "Press Esc: Quit"
            );
            window.draw(instructions);
        }
        else {
            instructions.setString(
                "Simulation is running\n"
                "Left-click: Add " + std::string(add_particle ? "Particle" : "Gravity Source") + "\n" +
                "Press P: Switch to Particle Mode\n" +
                "Press S: Switch to Gravity Source Mode\n" +
                "Press Space: Pause Simulation\n" +
                "Press Esc: Quit"
            );
            window.draw(instructions);

            // Update physics
            for (const auto& source : sources) {
                for (auto& particle : particles) {
                    particle.update_physics(source);
                }
            }
        }

        // Render sources and particles
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
