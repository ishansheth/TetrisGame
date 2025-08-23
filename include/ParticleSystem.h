#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

constexpr float degreesToRadians(float degrees)
{
    return degrees * (3.14159265358979323846f / 180.f);
}

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
    struct Particle
    {
        sf::Vector2f velocity;
        sf::Vector2f emitterPos;
    };

    sf::VertexArray vertices;
    sf::Time lifetimeTimer;
    std::vector<Particle> particles;
    sf::Color particlesColor;
    void resetParticle(const unsigned int idx);

  public:
    ParticleSystem(unsigned int count, sf::Color color)
        : vertices(sf::PrimitiveType::Points, count), 
        lifetimeTimer(sf::seconds(3)), 
        particles(count), 
        particlesColor(color)
    {}

    void draw(sf::RenderTarget &target, sf::RenderStates state) const override;

    void setEmitter(const sf::Vector2f &startPos);

    void update();
};