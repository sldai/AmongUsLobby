/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <learnopengl/shader.h>
#include "texture.h"
#include <iostream>

struct MovingCenter
{
    glm::vec3 Position;
    glm::vec3 Velocity;
};

// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    // stores the index of the last particle used (for quick access to next dead particle)
    unsigned int lastUsedParticle = 0;
    enum class ParticleEffect
    {
        center2outer, outer2center
    };
public:
    // constructor
    ParticleGenerator(Shader* shader, Texture2D texture, unsigned int amount);
    // update all particles
    void Update(float dt, MovingCenter object, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f), ParticleEffect p = ParticleEffect::center2outer);
    // render all particles
    void Draw();
    void kill_all_particles()
    {
        for (size_t i = 0; i < particles.size(); i++)
        {
            particles[i].Life = 0;
        }
    }


private:
    // state
    std::vector<Particle> particles;
    unsigned int amount;
    // render state
    Shader* shader;
    Texture2D texture;
    unsigned int VAO;
    // initializes buffer and vertex attributes
    void init();
    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle
    void respawnParticle(Particle &particle, MovingCenter object, glm::vec3 offset = glm::vec3(0.0f));

    void respawnParticleCenter2Outer(Particle& particle, MovingCenter object, glm::vec3 offset = glm::vec3(0.0f))
    {
        auto r = []() {return ((rand() % 20) - 10) / 10.0f; }; // [-1,1]
        float rColor = 0.5f + ((rand() % 100) / 100.0f);
        particle.Position = object.Position + glm::normalize(glm::vec3(r(), r(), r()))*0.05f + offset;
        particle.Color = glm::vec4(0, 0, rColor, 1.0f);
        particle.Life = 1.0f;
        particle.Velocity = glm::normalize(glm::vec3(r(), r(), r())) * 2.0f/ particle.Life;
    }

    void respawnParticleOuter2Center(Particle& particle, MovingCenter object, glm::vec3 offset = glm::vec3(0.0f))
    {
        auto r = []() {return ((rand() % 20) - 10) / 10.0f; }; // [-1,1]
        float rColor = 0.5f + ((rand() % 100) / 100.0f);
        particle.Position = object.Position + glm::normalize(glm::vec3(r(), r(), r())) * 2.0f + offset;
        particle.Color = glm::vec4(rColor, 0, 0, 1.0f);
        particle.Life = 1.0f;
        particle.Velocity = (object.Position - particle.Position)/particle.Life;
    }

};

ParticleGenerator::ParticleGenerator(Shader* shader, Texture2D texture, unsigned int amount)
    : shader(shader), texture(texture), amount(amount)
{
    this->init();
}

void ParticleGenerator::Update(float dt, MovingCenter object, unsigned int newParticles, glm::vec3 offset, ParticleEffect p)
{
    // add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        if (p==ParticleEffect::center2outer)
        {
            this->respawnParticleCenter2Outer(this->particles[unusedParticle], object, offset);
        }
        else if (p == ParticleEffect::outer2center)
        {
            this->respawnParticleOuter2Center(this->particles[unusedParticle], object, offset);
        }
        
    }
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position += p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

// render all particles
void ParticleGenerator::Draw()
{
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader->use();
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            //std::cout << "draw" << particle.Position.x << particle.Position.y << particle.Position.z;
            this->shader->setVec3("offset", particle.Position);
            this->shader->setVec4("color", particle.Color);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}


unsigned int ParticleGenerator::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, MovingCenter object, glm::vec3 offset)
{
    float random = ((rand() % 40) - 20) / 10.0f;
    auto r = []() {return ((rand() % 40) - 20) / 10.0f; };
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = object.Position + glm::vec3(r(),r(),r()) + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Velocity * 0.1f;
}

#endif