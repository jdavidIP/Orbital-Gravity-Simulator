#include "Utils.h"

sf::Clock simClock;

void addParticlesAtPosition(
    std::vector<Particle>& particles,
    sf::Vector2f pos,
    int count,
    int i,
    ParticleType type,
    const GravitySource& source
) {
    float dx = pos.x - source.get_pos().x;
    float dy = pos.y - source.get_pos().y;
    float r_sq = dx * dx + dy * dy;

    // Handle near-center case
    if (r_sq < 1e-5f) {
        particles.emplace_back(pos.x, pos.y, 0, 0, type);
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

        particles.emplace_back(pos.x, pos.y, vel_x, vel_y, type);
    }
}


void updateParticles(std::vector<Particle>& particles, const std::vector<GravitySource>& sources, bool mutualGravity) {
    for (auto& particle : particles) {
        particle.update_physics(particles, sources, mutualGravity);
    }
}

void renderScene(
    AppState state,
    const std::vector<sf::Text>& particleTypes,
    const std::vector<sf::Text>& sourceTypes,
    const ParticleType particleType,
    const GravitySourceType sourceType,
    sf::Text& instructions,
    Mode mode,
    sf::RenderWindow& window,
    bool pause,
    bool mutualGravity,
    std::vector<GravitySource>& sources,
    std::vector<Particle>& particles
) {
    renderTypes(particleTypes, sourceTypes, particleType, sourceType, mode, window);

    switch (state) {
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

void renderTypes(
    const std::vector<sf::Text>& particleTypes,
    const std::vector<sf::Text>& sourceTypes,
    const ParticleType particleType,
    const GravitySourceType sourceType,
    Mode mode,
    sf::RenderWindow& window) {

    // Draw type selection in upper-right
    float startX = window.getSize().x - 175; 
    float startY = 20;
    int lineHeight = 30;

    const auto& types = (mode == Mode::AddParticle) ? particleTypes : sourceTypes;
    int selectedIndex = (mode == Mode::AddParticle)
        ? static_cast<int>(particleType)
        : static_cast<int>(sourceType);

    // Time for pulse animation
    float time = simClock.getElapsedTime().asSeconds();
    float pulse = (std::sin(time * 2.0f) + 1.0f) * 0.5f; // 0 to 1
    float alpha = 128 + static_cast<int>(pulse * 127);   // 128-255
    float scale = 1.0f + 0.1f * pulse;                   // 1.0x-1.1x

    for (size_t i = 0; i < types.size(); ++i) {
        sf::Text t = types[i];
        t.setPosition(startX, startY + i * lineHeight);

        if (i == selectedIndex) {
            t.setFillColor(sf::Color(t.getFillColor().r, t.getFillColor().g, t.getFillColor().b, alpha));
            t.setScale(scale, scale);
            t.setOutlineThickness(2.0f);
            t.setOutlineColor(sf::Color::Yellow);
        }
        else {
            t.setFillColor(sf::Color(t.getFillColor().r, t.getFillColor().g, t.getFillColor().b, 180));
            t.setScale(1.0f, 1.0f);
            t.setOutlineThickness(0.0f);
        }

        window.draw(t);
    }
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
