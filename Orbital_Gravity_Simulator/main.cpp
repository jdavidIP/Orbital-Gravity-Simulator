#include <iostream>
#include <vector>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "GravitySource.h"
#include "Particle.h"
#include "Utils.h"

constexpr int WINDOW_HEIGHT = 900;
constexpr int WINDOW_WIDTH = 1400;
constexpr float DEFAULT_GRAVITY_STRENGTH = 7000.0f;



int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Orbital Gravity Simulator");
    window.setFramerateLimit(60);

    sf::Font open_sans;
    if (!open_sans.loadFromFile("OpenSans-VariableFont_wdth,wght.ttf")) {
        std::cerr << "Failed to load font!\n";
        return -1;
    }

    std::vector<GravitySource> sources;
    std::vector<Particle> particles;

    std::string userInput_num_particles;
    std::string userInput_mass_min;
    std::string userInput_mass_max;

    sf::Text inputText;
    inputText.setFont(open_sans);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(20, 20);

    sf::Text instructions;
    instructions.setFont(open_sans);
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition(20, 20);

    AppState state = AppState::AwaitingMinMass;
    Mode mode = Mode::AddParticle;

    int num_particles = 0;
    float min_mass = 0.0f;
    float max_mass = 0.0f;
    sf::Vector2f spawn_pos;

    bool pause = false;
    bool mutualGravity = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Escape: window.close(); break;
                case sf::Keyboard::P: mode = Mode::AddParticle; break;
                case sf::Keyboard::S: mode = Mode::AddSource; break;
                case sf::Keyboard::G:
                    mutualGravity = !mutualGravity;
                    break;
                case sf::Keyboard::R:
                    if (state == AppState::Running || state == AppState::Paused)
                    {
                        state = AppState::AwaitingMinMass;
                        particles.clear();
                        sources.clear();
                    }
                    break;
                case sf::Keyboard::Enter:
                    if (state == AppState::AwaitingMinMass && !userInput_mass_min.empty())
                    {
                        min_mass = std::stof(userInput_mass_min);
                        if (min_mass >= MIN_MASS && min_mass <= MAX_MASS - 0.01f) state = AppState::AwaitingMaxMass;
                    }
                    else if (state == AppState::AwaitingMaxMass && !userInput_mass_max.empty())
                    {
                        max_mass = std::stof(userInput_mass_max);
                        if (max_mass <= MAX_MASS && max_mass > min_mass) state = AppState::AwaitingNumParticles;
                    }
                    else if (state == AppState::AwaitingNumParticles && !userInput_num_particles.empty()) {
                        num_particles = std::stoi(userInput_num_particles);
                        if (num_particles > 0) state = AppState::AwaitingSpawnPos;
                    }
                    else if (state == AppState::AwaitingSources && !sources.empty()) {
                        state = AppState::Running;
                        pause = false;
                    }
                    break;
                case sf::Keyboard::Space:
                    if (state == AppState::Running || state == AppState::Paused) {
                        pause = !pause;
                        state = pause ? AppState::Paused : AppState::Running;
                    }
                    break;
                }
            }
            else if (state == AppState::AwaitingNumParticles && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !userInput_num_particles.empty()) {
                    userInput_num_particles.pop_back();
                }
                else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    userInput_num_particles += static_cast<char>(event.text.unicode);
                }
            }
            else if (state == AppState::AwaitingMinMass && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b' && !userInput_mass_min.empty()) {
                    userInput_mass_min.pop_back();
                }
                else if ((event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == '.') {
                    userInput_mass_min += static_cast<char>(event.text.unicode);
                }
            }
            else if (state == AppState::AwaitingMaxMass && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b' && !userInput_mass_max.empty()) {
                    userInput_mass_max.pop_back();
                }
                else if ((event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == '.') {
                    userInput_mass_max += static_cast<char>(event.text.unicode);
                }
            }
            else if (state == AppState::AwaitingSpawnPos && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                spawn_pos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
                addParticlesAtPosition(particles, spawn_pos, num_particles, min_mass, max_mass);
                state = AppState::AwaitingSources;
            }
            else if (state == AppState::AwaitingSources && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f pos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
                sources.emplace_back(pos.x, pos.y, DEFAULT_GRAVITY_STRENGTH);
            }
            else if ((state == AppState::Running || state == AppState::Paused) && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f pos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
                if (mode == Mode::AddParticle) {
                    float vel_x = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
                    float vel_y = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
                    float mass = min_mass + static_cast<float>(std::rand()) / RAND_MAX * (max_mass - min_mass);
                    particles.emplace_back(pos.x, pos.y, vel_x, vel_y, mass);
                    float value = static_cast<float>(particles.size()) / (particles.size() + 100);
                    particles.back().set_color(map_value_to_color(value));
                }
                else {
                    sources.emplace_back(pos.x, pos.y, DEFAULT_GRAVITY_STRENGTH);
                }
            }
        }

        // Only update physics when running and not paused
        if (state == AppState::Running && !pause) {
            updateParticles(particles, sources, mutualGravity);
        }

        window.clear();
        renderScene(state, userInput_num_particles, userInput_mass_min, userInput_mass_max, inputText, instructions, mode, window, num_particles, pause, mutualGravity,sources, particles);
        window.display();
    }

    return 0;
}
