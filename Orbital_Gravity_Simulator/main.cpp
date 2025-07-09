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

    // Text
    sf::Font open_sans;
    open_sans.loadFromFile("C:\\Users\\Ibapo\\OneDrive\\Escritorio\\Projects\\SMFL_Learning\\SFML_FONTS\\Open_Sans\\OpenSans-VariableFont_wdth,wght.ttf");

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
    bool gotInput = false;

    int num_particles;
    bool add_source = false;
    bool add_particle = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            if (!gotInput) {
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b') {
                        if (!userInput_num_particles.empty())
                            userInput_num_particles.pop_back();
                    }
                    else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                        userInput_num_particles += static_cast<char>(event.text.unicode);
                    }
                    else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                        if (!userInput_num_particles.empty()) {
                            num_particles = std::stoi(userInput_num_particles);
                            gotInput = true;

                            for (int i = 0; i < num_particles; ++i) {
                                particles.emplace_back(600, 700, 4, 0.2 + (0.1 / num_particles) * i);
                                float value = static_cast<float>(i) / num_particles;
                                particles[i].set_color(map_value_to_color(value));
                            }
                        }
                    }
                }
            }
            else {
                if (!start && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    start = true;
                }

                if (start) {

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    {
                        start = false;
                    }
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                add_particle = true;
                add_source = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                add_particle = false;
                add_source = true;
            }


            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                sf::Vector2f spawn_pos(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y));

                if (add_particle) {
                    float vel_x = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
                    float vel_y = static_cast<float>(std::rand() % 100 - 50) / 50.0f;

                    particles.emplace_back(spawn_pos.x, spawn_pos.y, vel_x, vel_y);
                    float value = static_cast<float>(particles.size()) / (particles.size() + 100);
                    particles.back().set_color(map_value_to_color(value));
                }
                else {
                    sources.emplace_back(spawn_pos.x, spawn_pos.y, 7000);
                }
            }
        }

        window.clear();

        std::string mode = add_particle ? "Particle Mode" : "Gravity Source Mode";

        if (!gotInput) {
            inputText_num_particles.setString(
                "Left-click: Add " + mode + "\n"
                "Press P: Switch to Particle Mode\n"
                "Press S: Switch to Gravity Source Mode\n"
                "Enter number of particles: " + userInput_num_particles + "\nPress Enter to confirm."
            );
            window.draw(inputText_num_particles);
        }
        else if (!start) {
            instructions.setString(
                "Press Enter to start the simulation."
            );
            window.draw(instructions);
        }
        else {
            instructions.setString(
                "Left-click: Add " + mode + "\n"
                "Press P: Switch to Particle Mode\n"
                "Press S: Switch to Gravity Source Mode\n"
                "Press Space: To Pause Simulation\n"
                "Press Esc: Quit"
            );
            window.draw(instructions);

            for (const auto& source : sources) {
                for (auto& particle : particles) {
                    particle.update_physics(source);
                }
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
