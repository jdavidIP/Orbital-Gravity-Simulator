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

void addParticlesAtPosition(
    std::vector<Particle>& particles,
    sf::Vector2f pos,
    int count,
    int i,
    float min_mass,
    float max_mass,
    const GravitySource& source
) {
    float mass = min_mass + static_cast<float>(std::rand()) / RAND_MAX * (max_mass - min_mass);

    float dx = pos.x - source.get_pos().x;
    float dy = pos.y - source.get_pos().y;
    float r_sq = dx * dx + dy * dy;

    // Handle near-center case
    if (r_sq < 1e-5f) {
        particles.emplace_back(pos.x, pos.y, 0, 0, mass);
    }
    else {
        float r = std::sqrt(r_sq);
        float v = std::sqrt(G * source.get_strength() / r);

        // Normalized tangent vector
        float tx = -dy / r;
        float ty = dx / r;

        // Base velocity + small random perturbation
        float perturbation = 0.05f * v * (std::rand() % 100 - 50) / 50.0f;
        float vel_x = v * tx + perturbation * tx;
        float vel_y = v * ty + perturbation * ty;

        particles.emplace_back(pos.x, pos.y, vel_x, vel_y, mass);
    }

    // Color mapping
    float value = i == count ? static_cast<float>(i) / (count + 100) : static_cast<float>(i) / count;
    particles.back().set_color(map_value_to_color(value));
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
            "Click to add gravity sources.\nPress Enter to confirm."
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

GravitySource* findNearestSource(sf::Vector2f pos, std::vector<GravitySource>& sources) {
    GravitySource* closest = nullptr;
    float minDist2 = std::numeric_limits<float>::max();
    for (auto& s : sources) {
        float dx = s.get_pos().x - pos.x;
        float dy = s.get_pos().y - pos.y;
        float dist2 = dx * dx + dy * dy;
        if (dist2 < minDist2) {
            minDist2 = dist2;
            closest = &s;
        }
    }
    return closest;
}
