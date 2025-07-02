#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

constexpr float degreesToRadians(float degrees) {
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

    void resetParticle(unsigned int idx)
    {
        static std::random_device dev;
        static std::mt19937 rng(dev());

        const float speedx = std::uniform_real_distribution(-2.f,2.f)(rng);
        const float speedy = std::uniform_real_distribution(-2.f,2.f)(rng);

        particles[idx].velocity = sf::Vector2f(speedx,speedy);
        vertices[idx].position = particles[idx].emitterPos;
    }

    public:
    ParticleSystem(unsigned int count):
    vertices(sf::PrimitiveType::Points, count)
    ,lifetimeTimer(sf::seconds(3))
    ,particles(count)
    {}

    void draw(sf::RenderTarget& target, sf::RenderStates state) const override
    {
        target.draw(vertices,state); 
    }

    void setEmitter(sf::Vector2f startPos)
    {
        for(std::size_t i = 0; i < particles.size(); i++)
        {
            particles[i].emitterPos.x = startPos.x;
            particles[i].emitterPos.y = startPos.y+10;

            resetParticle(i);
        }
    }

    void update()
    {
        for(std::size_t i = 0; i < particles.size(); i++)
        {
            Particle& p = particles[i];
            
            vertices[i].position += p.velocity;
            vertices[i].color = sf::Color(255,0,0);
        }
    }

};