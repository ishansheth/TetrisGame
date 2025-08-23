#include "ParticleSystem.h"
#include <random>
#include "Util.h"
#include "GameConstants.h"

void ParticleSystem::resetParticle(const unsigned int idx)
{
    static std::random_device dev;
    static std::mt19937 rng(dev());

    const float speedx = std::uniform_real_distribution<float>(-2.f, 2.f)(rng);
    const float speedy = std::uniform_real_distribution<float>(-2.f, 2.f)(rng);

    particles[idx].velocity = sf::Vector2f(speedx, speedy);
    vertices[idx].position = particles[idx].emitterPos;
}

void ParticleSystem::draw(sf::RenderTarget &displayWindow, sf::RenderStates state) const
{
    displayWindow.draw(vertices, state);
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

        auto temp = vertices[i].position + p.velocity;
        if(temp.x >= 0 && temp.x <= DRAW_WINDOW_WIDTH && temp.y >= 0 && temp.y <= DRAW_WINDOW_HEIGHT)
        {
            vertices[i].position = temp;
        }
        vertices[i].color = particlesColor;
    }
}
