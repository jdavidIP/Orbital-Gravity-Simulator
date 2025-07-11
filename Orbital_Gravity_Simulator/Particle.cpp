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
        s.setRadius(1.5);
        break;

    case ParticleType::Satellite:
        mass = 0.01f;
        s.setFillColor(sf::Color(192, 192, 192)); // pale grey
        s.setRadius(3);
        break;

    case ParticleType::Terrestrial:
        mass = 1.0f;
        s.setFillColor(sf::Color(11, 102, 35)); // Forest green
        s.setRadius(11);
        break;

    case ParticleType::GasGiant:
        mass = 318.0f;
        s.setFillColor(sf::Color(255, 174, 66));  // Yellowish orange
        s.setRadius(24);
        break;

    case ParticleType::IceGiant:
        mass = 17.0f;
        s.setFillColor(sf::Color(0, 255, 255));  // cyan
        s.setRadius(16);
        break;

    default:
        break;
    }
}

void Particle::render(sf::RenderWindow& window) {
    s.setPosition(pos);
    window.draw(s);
}

void Particle::update_physics(const GravitySource& source) {

    // --- Compute acceleration at current position --- 
    sf::Vector2f a0;
    {
        float dx = source.get_pos().x - pos.x;
        float dy = source.get_pos().y - pos.y;
        float dist2 = dx * dx + dy * dy + SOFTENING * SOFTENING;
        float dist = std::sqrt(dist2);
        float a_mag = G * source.get_strength() / dist2;
        a0 = { a_mag * dx / dist, a_mag * dy / dist };
    }

    // --- First half-step for velocity ---
    velocity.x += 0.5f * a0.x * dt;
    velocity.y += 0.5f * a0.y * dt;

    // --- Full step for position ---
    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;

    // --- Compute acceleration at new position ---
    sf::Vector2f a1;
    {
        float dx = source.get_pos().x - pos.x;
        float dy = source.get_pos().y - pos.y;
        float dist2 = dx * dx + dy * dy + SOFTENING * SOFTENING;
        float dist = std::sqrt(dist2);
        float a_mag = G * source.get_strength() / dist2;
        a1 = { a_mag * dx / dist, a_mag * dy / dist };
    }

    // --- Second half-step for velocity ---
    velocity.x += 0.5f * a1.x * dt;
    velocity.y += 0.5f * a1.y * dt;
}

void Particle::update_physics(
    const std::vector<Particle>& others,
    const std::vector<GravitySource>& sources,
    bool mutualGravity
) {

    // --- 1) Compute acceleration at current position ---
    sf::Vector2f accel0(0.f, 0.f);

    // from fixed sources
    for (auto const& src : sources) {
        float dx = src.get_pos().x - pos.x;
        float dy = src.get_pos().y - pos.y;
        float dist2 = dx * dx + dy * dy + SOFTENING * SOFTENING;
        float invDist = 1.0f / std::sqrt(dist2);
        float a_mag = G * src.get_strength() * invDist * invDist;
        accel0.x += a_mag * dx * invDist;
        accel0.y += a_mag * dy * invDist;
    }

    // mutual gravity
    if (mutualGravity) {
        for (auto const& other : others) {
            if (&other == this) continue;
            float dx = other.get_pos().x - pos.x;
            float dy = other.get_pos().y - pos.y;
            float dist2 = dx * dx + dy * dy + SOFTENING * SOFTENING;
            float invDist = 1.0f / std::sqrt(dist2);
            float a_mag = G * other.get_mass() * invDist * invDist;
            // Note: F=G*m_other*m_self/r^2, a=F/m_self => G*m_other/r^2
            accel0.x += a_mag * dx * invDist;
            accel0.y += a_mag * dy * invDist;
        }
    }

    // --- 2) Half-step velocity ---
    velocity += accel0 * (0.5f * dt);

    // --- 3) Full-step position ---
    pos += velocity * dt;

    // --- 4) Compute acceleration at new position ---
    sf::Vector2f accel1(0.f, 0.f);

    for (auto const& src : sources) {
        float dx = src.get_pos().x - pos.x;
        float dy = src.get_pos().y - pos.y;
        float dist2 = dx * dx + dy * dy + SOFTENING * SOFTENING;
        float invDist = 1.0f / std::sqrt(dist2);
        float a_mag = G * src.get_strength() * invDist * invDist;
        accel1.x += a_mag * dx * invDist;
        accel1.y += a_mag * dy * invDist;
    }

    if (mutualGravity) {
        for (auto const& other : others) {
            if (&other == this) continue;
            float dx = other.get_pos().x - pos.x;
            float dy = other.get_pos().y - pos.y;
            float dist2 = dx * dx + dy * dy + SOFTENING * SOFTENING;
            float invDist = 1.0f / std::sqrt(dist2);
            float a_mag = G * other.get_mass() * invDist * invDist / mass;
            accel1.x += a_mag * dx * invDist;
            accel1.y += a_mag * dy * invDist;
        }
    }

    // --- 5) Second half-step velocity ---
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