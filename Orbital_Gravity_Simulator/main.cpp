#include <iostream>
#include <vector>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "GravitySource.h"
#include "Particle.h"
#include "Utils.h"

// SCALES
// 1 px = 1000 km
// 1 mass unit = 1 earth (5.97×10^24 kg)

constexpr int WINDOW_HEIGHT = 900;
constexpr int WINDOW_WIDTH = 1400;
constexpr float DEFAULT_GRAVITY_STRENGTH = 25000.0f; // Based on a small red dwarf's mass

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Gravity Simulator", sf::Style::Fullscreen);
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

    AppState state = AppState::AwaitingSources;
    Mode mode = Mode::AddParticle;

    bool pause = false;
    bool mutualGravity = false;

    ParticleType particleType = ParticleType::Terrestrial;
    GravitySourceType sourceType;

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
                        particles.clear();
                        sources.clear();
                        mutualGravity = false;
                        pause = false;
                    }
                    break;
                case sf::Keyboard::Num1:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::Planetoid;
                    break;
                case sf::Keyboard::Num2:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::Satellite;
                    break;
                case sf::Keyboard::Num3:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::Terrestrial;
                    break;
                case sf::Keyboard::Num4:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::GasGiant;
                    break;
                case sf::Keyboard::Num5:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::IceGiant;
                    break;
                case sf::Keyboard::Enter:
                    if (state == AppState::AwaitingSources && !sources.empty()) {
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
            else if (state == AppState::AwaitingSources && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f pos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
                sources.emplace_back(pos.x, pos.y, GravitySourceType::RedDwarf);
            }
            else if ((state == AppState::Running || state == AppState::Paused) && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f pos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
                GravitySource* source = findNearestSource(pos, sources);
                if (mode == Mode::AddParticle) {
                    addParticlesAtPosition(particles, pos, particles.size(), particles.size(), particleType, *source);
                }
                else {
                    sources.emplace_back(pos.x, pos.y, GravitySourceType::YellowDwarf);
                }
            }
        }

        // Only update physics when running and not paused
        if (state == AppState::Running && !pause) {
            updateParticles(particles, sources, mutualGravity);
        }

        window.clear();
        renderScene(state, userInput_num_particles, userInput_mass_min, userInput_mass_max, inputText, instructions, mode, window, pause, mutualGravity,sources, particles);
        window.display();
    }

    return 0;
}
