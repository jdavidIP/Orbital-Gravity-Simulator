#include "Utils.h"

sf::Color map_value_to_color(float value) {
    if (value < 0.0f) value = 0;
    else if (value > 1.0f) value = 1;

    int r = 0, g = 0, b = 0;

    if (value < 0.5f) {
        b = static_cast<int>(255 * (1.0f - 2 * value));
        g = static_cast<int>(255 * 2 * value);
    }
    else {
        g = static_cast<int>(255 * (2.0f - 2 * value));
        r = static_cast<int>(255 * (2 * value - 1));
    }

    return sf::Color(r, g, b);
}

void addParticlesAtPosition(std::vector<Particle>& particles, sf::Vector2f pos, int count, float min_mass, float max_mass) {
    for (int i = 0; i < count; ++i) {
        float vel_x = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
        float vel_y = static_cast<float>(std::rand() % 100 - 50) / 50.0f;
        float mass = min_mass + static_cast<float>(std::rand()) / RAND_MAX * (max_mass - min_mass);
        particles.emplace_back(pos.x, pos.y, vel_x, vel_y, mass);
        float value = static_cast<float>(i) / count;
        particles.back().set_color(map_value_to_color(value));
    }
}

void updateParticles(std::vector<Particle>& particles, const std::vector<GravitySource>& sources, bool mutualGravity) {
    for (auto& particle : particles) {
        particle.update_physics(particles, sources, mutualGravity);
    }
}

void renderScene(
    AppState state,
    const std::string& userInput_num_particles,
    const std::string& userInput_min_mass,
    const std::string& userInput_max_mass,
    sf::Text& inputText,
    sf::Text& instructions,
    Mode mode,
    sf::RenderWindow& window,
    int num_particles,
    bool pause,
    bool mutualGravity,
    std::vector<GravitySource>& sources,
    std::vector<Particle>& particles
) {
    switch (state) {
    case AppState::AwaitingMinMass:
        inputText.setString(
            "Enter the minimum mass (> 0.1): " + userInput_min_mass + "\nPress Enter to confirm."
        );
        window.draw(inputText);
        break;

    case AppState::AwaitingMaxMass:
        inputText.setString(
            "Enter the minimum mass (> 0.1): " + userInput_min_mass + 
            "\nEnter the maximum mass (< 5.0): " + userInput_max_mass + "\nPress Enter to confirm."
        );
        window.draw(inputText);
        break;

    case AppState::AwaitingNumParticles:
        inputText.setString(
            "Enter the minimum mass (> 0.1): " + userInput_min_mass +
            "\nEnter the maximum mass (< 5.0): " + userInput_max_mass +
            "\nEnter number of particles: " + userInput_num_particles + "\nPress Enter to confirm."
        );
        window.draw(inputText);
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
            "Mutual Gravity: " + std::string(mutualGravity ? "ON" : "OFF") + "\n"
            "Left-click: Add " + std::string(mode == Mode::AddParticle ? "Particle" : "Gravity Source") + "\n"
            "Press P/S: Switch mode\n"
            "Press Space: Resume\n"
            "Press R: Restart\n"
            "Press Esc: Quit"
        );
        window.draw(instructions);
        break;

    case AppState::Running:
        instructions.setString(
            "Simulation running.\n"
            "Mutual Gravity: " + std::string(mutualGravity ? "ON" : "OFF") + "\n"
            "Left-click: Add " + std::string(mode == Mode::AddParticle ? "Particle" : "Gravity Source") + "\n"
            "Press P/S: Switch mode\n"
            "Press Space: Pause\n"
            "Press R: Restart\n"
            "Press Esc: Quit"
        );
        window.draw(instructions);
        break;
    }

    for (auto& source : sources) source.render(window);
    for (auto& particle : particles) particle.render(window);
}