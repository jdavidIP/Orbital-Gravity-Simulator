#include <iostream>
#include <vector>
#include <cstdlib> 
#include <SFML/Graphics.hpp>
#include "GravitySource.h"
#include "Particle.h"

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 1400
#define DEFAULT_GRAVITY_STRENGTH 7000

enum class AppState {
    AwaitingNumParticles,
    AwaitingSpawnPos,
    AwaitingSources,
    Running,
    Paused
};

enum class Mode {
    AddParticle,
    AddSource
};

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

void addParticlesAtPosition(std::vector<Particle>& particles, sf::Vector2f pos, int count) {
    for (int i = 0; i < count; ++i) {
        float vel_x = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
        float vel_y = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
        particles.emplace_back(pos.x, pos.y, vel_x, vel_y);
        float value = static_cast<float>(i) / count;
        particles.back().set_color(map_value_to_color(value));
    }
}

void rendering(const AppState state, const std::string userInput_num_particles, sf::Text& inputText_num_particles, 
    sf::Text& instructions, const Mode mode, sf::RenderWindow& window, const int num_particles,
    const bool pause, std::vector<GravitySource>& sources, std::vector<Particle>& particles)
{
    switch (state) {
    case AppState::AwaitingNumParticles:
        inputText_num_particles.setString(
            "Enter number of particles: " + userInput_num_particles + "\nPress Enter to confirm."
        );
        window.draw(inputText_num_particles);
        break;

    case AppState::AwaitingSpawnPos:
        instructions.setString(
            "Click to choose spawn position for all " + std::to_string(num_particles) + " particles."
        );
        window.draw(instructions);
        break;

    case AppState::AwaitingSources:
        instructions.setString(
            "Click to add gravity sources.\nPress Enter to start simulation."
        );
        window.draw(instructions);
        break;

    case AppState::Paused:
        instructions.setString(
            "Simulation paused.\n"
            "Left-click: Add " + std::string(mode == Mode::AddParticle ? "Particle" : "Gravity Source") + "\n" +
            "Press P/S: Switch mode\n" +
            "Press Space: Resume\n" +
            "Press Esc: Quit"
        );
        window.draw(instructions);
        break;

    case AppState::Running:
        instructions.setString(
            "Simulation running.\n"
            "Left-click: Add " + std::string(mode == Mode::AddParticle ? "Particle" : "Gravity Source") + "\n" +
            "Press P/S: Switch mode\n" +
            "Press Space: Pause\n" +
            "Press Esc: Quit"
        );
        window.draw(instructions);

        if (!pause) {
            for (const auto& source : sources) {
                for (auto& particle : particles) {
                    particle.update_physics(source);
                }
            }
        }
        break;
    }

    // Render all sources and particles
    for (auto& source : sources) source.render(window);
    for (auto& particle : particles) particle.render(window);
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

    AppState state = AppState::AwaitingNumParticles;
    Mode mode = Mode::AddParticle;

    int num_particles = 0;
    sf::Vector2f spawn_pos;

    bool pause = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    window.close();
                    break;

                case sf::Keyboard::P:
                    mode = Mode::AddParticle;
                    break;

                case sf::Keyboard::S:
                    mode = Mode::AddSource;
                    break;

                case sf::Keyboard::Enter:
                    if (state == AppState::AwaitingNumParticles && !userInput_num_particles.empty()) {
                        num_particles = std::stoi(userInput_num_particles);
                        if (num_particles > 0) {
                            state = AppState::AwaitingSpawnPos;
                        }
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
            else if (state == AppState::AwaitingSpawnPos && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                spawn_pos = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                addParticlesAtPosition(particles, spawn_pos, num_particles);
                state = AppState::AwaitingSources;
            }
            else if (state == AppState::AwaitingSources && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2f pos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                sources.emplace_back(pos.x, pos.y, DEFAULT_GRAVITY_STRENGTH);
            }
            else if ((state == AppState::Running || state == AppState::Paused) && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2f pos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if (mode == Mode::AddParticle) {
                    float vel_x = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
                    float vel_y = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
                    particles.emplace_back(pos.x, pos.y, vel_x, vel_y);
                    float value = static_cast<float>(particles.size()) / (particles.size() + 100);
                    particles.back().set_color(map_value_to_color(value));
                }
                else {
                    sources.emplace_back(pos.x, pos.y, DEFAULT_GRAVITY_STRENGTH);
                }
            }
        }

        // Rendering
        window.clear();

        rendering(state, userInput_num_particles, inputText_num_particles, instructions, mode, window, num_particles, pause, sources, particles);

        window.display();
    }

    return 0;
}
