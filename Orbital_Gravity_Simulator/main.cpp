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

    sf::Text titleText;
    titleText.setFont(open_sans);
    titleText.setCharacterSize(100);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Orbital Gravity Simulator");

    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    titleText.setPosition(desktop.width / 2.f, desktop.height / 2.f - 150);

    sf::Text subtitleText;
    subtitleText.setFont(open_sans);
    subtitleText.setCharacterSize(40);
    subtitleText.setFillColor(sf::Color::White);
    subtitleText.setString("Enter to Start");

    sf::FloatRect subtitleBounds = subtitleText.getLocalBounds();
    subtitleText.setOrigin(subtitleBounds.width / 2.f, subtitleBounds.height / 2.f);
    subtitleText.setPosition(desktop.width / 2.f, desktop.height / 2.f + 20);

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

    // Create particle type labels
    std::vector<sf::Text> particleTypes;
    std::vector<std::string> particleNames = {
        "1: Planetoid", "2: Satellite", "3: Terrestrial",
        "4: Gas Giant", "5: Ice Giant"
    };
    std::vector<sf::Color> particleColors = {
        sf::Color(165, 42, 42),   // Planetoid - reddish brown
        sf::Color(192, 192, 192), // Satellite - pale grey
        sf::Color(11, 102, 35),   // Terrestrial - green
        sf::Color(255, 174, 66),  // Gas Giant - yellowish orange
        sf::Color(0, 255, 255)    // Ice Giant - cyan
    };

    for (size_t i = 0; i < particleNames.size(); ++i) {
        sf::Text text(particleNames[i], open_sans, 20);
        text.setFillColor(particleColors[i]);
        particleTypes.push_back(text);
    }

    // Create gravity source type labels
    std::vector<sf::Text> sourceTypes;
    std::vector<std::string> sourceNames = {
        "1: Red Dwarf", "2: White Dwarf", "3: Yellow Dwarf", "4: Neutron Star"
    };
    std::vector<sf::Color> sourceColors = {
        sf::Color::Red, sf::Color::White,
        sf::Color(139, 128, 0), sf::Color(175, 238, 238)
    };

    for (size_t i = 0; i < sourceNames.size(); ++i) {
        sf::Text text(sourceNames[i], open_sans, 20);
        text.setFillColor(sourceColors[i]);
        sourceTypes.push_back(text);
    }


    AppState state = AppState::StartMenu;
    Mode mode = Mode::AddSource;

    bool pause = false;
    bool mutualGravity = false;

    ParticleType particleType = ParticleType::Terrestrial;
    GravitySourceType sourceType = GravitySourceType::RedDwarf;

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
                        state = AppState::AwaitingSources;
                        mode = Mode::AddSource;
                        mutualGravity = false;
                        pause = false;
                    }
                    break;
                case sf::Keyboard::Num1:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::Planetoid;
                    else if (mode == Mode::AddSource)
                        sourceType = GravitySourceType::RedDwarf;
                    break;
                case sf::Keyboard::Num2:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::Satellite;
                    else if (mode == Mode::AddSource)
                        sourceType = GravitySourceType::WhiteDwarf;
                    break;
                case sf::Keyboard::Num3:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::Terrestrial;
                    else if (mode == Mode::AddSource)
                        sourceType = GravitySourceType::YellowDwarf;
                    break;
                case sf::Keyboard::Num4:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::GasGiant;
                    else if (mode == Mode::AddSource)
                        sourceType = GravitySourceType::NeutronStar;
                    break;
                case sf::Keyboard::Num5:
                    if (mode == Mode::AddParticle)
                        particleType = ParticleType::IceGiant;
                    break;
                case sf::Keyboard::Enter:
                    if (state == AppState::StartMenu)
                    {
                        state = AppState::AwaitingSources;
                    }
                    else if (state == AppState::AwaitingSources && !sources.empty()) {
                        state = AppState::Running;
                        mode = Mode::AddParticle;
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
                sources.emplace_back(pos.x, pos.y, sourceType);
            }
            else if ((state == AppState::Running || state == AppState::Paused) && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f pos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
                GravitySource* source = findNearestSource(pos, sources);
                if (mode == Mode::AddParticle) {
                    addParticlesAtPosition(particles, pos, particles.size(), particles.size(), particleType, *source);
                }
                else {
                    sources.emplace_back(pos.x, pos.y, sourceType);
                }
            }
        }

        // Only update physics when running and not paused
        if (state == AppState::Running && !pause) {
            updateParticles(particles, sources, mutualGravity);
        }

        window.clear();
        state == AppState::StartMenu ? renderStartMenu(titleText, subtitleText, window) : renderScene(state, particleTypes, sourceTypes, particleType, sourceType, instructions, mode, window, pause, mutualGravity,sources, particles);
        window.display();
    }

    return 0;
}
