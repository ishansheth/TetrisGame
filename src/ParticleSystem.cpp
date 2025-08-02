#include "ParticleSystem.h"
#include <random>

void ParticleSystem::resetParticle(const unsigned int idx)
{
    static std::random_device dev;
    static std::mt19937 rng(dev());

    const float speedx = std::uniform_real_distribution(-2.f, 2.f)(rng);
    const float speedy = std::uniform_real_distribution(-2.f, 2.f)(rng);

    particles[idx].velocity = sf::Vector2f(speedx, speedy);
    vertices[idx].position = particles[idx].emitterPos;
}

void ParticleSystem::draw(sf::RenderTarget &target, sf::RenderStates state) const
{
    target.draw(vertices, state);
}

void ParticleSystem::setEmitter(const sf::Vector2f &startPos)
{
    for (std::size_t i = 0; i < particles.size(); i++)
    {
        particles[i].emitterPos.x = startPos.x;
        particles[i].emitterPos.y = startPos.y + 10;

        resetParticle(i);
    }
}

void ParticleSystem::update()
{
    for (std::size_t i = 0; i < particles.size(); i++)
    {
        Particle &p = particles[i];

        vertices[i].position += p.velocity;
        vertices[i].color = sf::Color(255, 0, 0);
    }
}
