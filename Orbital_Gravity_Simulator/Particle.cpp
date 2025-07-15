#include "Particle.h"
#include <cmath>

Particle::Particle(float pos_x, float pos_y, float vel_x, float vel_y, ParticleType type)
    : pos(pos_x, pos_y), velocity(vel_x, vel_y), type(type)
{
    s.setPosition(pos);
    mass = 0.0f;
    
    switch (type)
    {
    case ParticleType::Planetoid:
        mass = 0.0035f;
        s.setFillColor(sf::Color(165, 42, 42)); // reddish brown
        s.setRadius(0.783f);
        break;

    case ParticleType::Satellite:
        mass = 0.01f;
        s.setFillColor(sf::Color(192, 192, 192)); // pale grey
        s.setRadius(2.8906f);
        break;

    case ParticleType::Terrestrial:
        mass = 1.0f;
        s.setFillColor(sf::Color(11, 102, 35)); // Forest green
        s.setRadius(10.63f);
        break;

    case ParticleType::GasGiant:
        mass = 318.0f;
        s.setFillColor(sf::Color(255, 174, 66));  // Yellowish orange
        s.setRadius(119.153f);
        break;

    case ParticleType::IceGiant:
        mass = 17.0f;
        s.setFillColor(sf::Color(0, 255, 255));  // cyan
        s.setRadius(41.66f);
        break;

    default:
        break;
    }
}

void Particle::render(sf::RenderWindow& window) {
    s.setPosition(pos);
    s.setOrigin(s.getRadius(), s.getRadius());
    window.draw(s);
}

void Particle::update_physics(
    const std::vector<Particle>& others,
    const std::vector<GravitySource>& sources,
    bool mutualGravity
) {
    sf::Vector2f accel0(0.f, 0.f);

    // Fixed sources
    for (const auto& src : sources) {
        float dx = src.get_pos().x - pos.x;
        float dy = src.get_pos().y - pos.y;
        float dist2 = dx * dx + dy * dy + SOFTENING * SOFTENING;
        float dist = std::sqrt(dist2);

        // Effective distance includes visual radii
        float effectiveDist = dist - (src.get_radius() + get_radius());
        if (effectiveDist < SOFTENING) effectiveDist = SOFTENING;

        float invDist = 1.0f / effectiveDist;
        float a_mag = G * src.get_strength() * invDist * invDist;
        accel0.x += a_mag * dx / dist;
        accel0.y += a_mag * dy / dist;
    }

    // Mutual gravity
    if (mutualGravity) {
        for (const auto& other : others) {
            if (&other == this) continue;
            float dx = other.get_pos().x - pos.x;
            float dy = other.get_pos().y - pos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            float effectiveDist = dist - (other.get_radius() + get_radius());
            if (effectiveDist < SOFTENING) effectiveDist = SOFTENING;

            float invDist = 1.0f / effectiveDist;
            float a_mag = G * other.get_mass() * invDist * invDist;
            accel0.x += a_mag * dx / dist;
            accel0.y += a_mag * dy / dist;
        }
    }

    // Half-step velocity
    velocity += accel0 * (0.5f * dt);

    // Full-step position
    pos += velocity * dt;

    // Recompute acceleration at new position
    sf::Vector2f accel1(0.f, 0.f);

    for (const auto& src : sources) {
        float dx = src.get_pos().x - pos.x;
        float dy = src.get_pos().y - pos.y;
        float dist2 = dx * dx + dy * dy + SOFTENING * SOFTENING;
        float dist = std::sqrt(dist2);

        float effectiveDist = dist - (src.get_radius() + get_radius());
        if (effectiveDist < SOFTENING) effectiveDist = SOFTENING;

        float invDist = 1.0f / effectiveDist;
        float a_mag = G * src.get_strength() * invDist * invDist;
        accel1.x += a_mag * dx / dist;
        accel1.y += a_mag * dy / dist;
    }

    if (mutualGravity) {
        for (const auto& other : others) {
            if (&other == this) continue;
            float dx = other.get_pos().x - pos.x;
            float dy = other.get_pos().y - pos.y;
            float dist2 = dx * dx + dy * dy + SOFTENING * SOFTENING;
            float dist = std::sqrt(dist2);

            float effectiveDist = dist - (other.get_radius() + get_radius());
            if (effectiveDist < SOFTENING) effectiveDist = SOFTENING;

            float invDist = 1.0f / effectiveDist;
            float a_mag = G * other.get_mass() * invDist * invDist;
            accel1.x += a_mag * dx / dist;
            accel1.y += a_mag * dy / dist;
        }
    }

    // Second half-step velocity
    velocity += accel1 * (0.5f * dt);
}


sf::Vector2f Particle::get_pos() const {
    return pos;
}

sf::Vector2f Particle::get_velocity() const {
    return velocity;
}

float Particle::get_mass() const {
    return mass;
}

ParticleType Particle::get_type() const {
    return type;
}

float Particle::get_radius() const {
    return s.getRadius();
}