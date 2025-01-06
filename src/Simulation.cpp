//
// Created by Lucas Wang on 2025-01-05.
//

#include "Simulation.h"
#include <iostream>

void Simulation::addParticle(const Particle &particle) {
    particles.push_back(particle);
}

void Simulation::update(float dt) {
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = 0; j < particles.size(); ++j) {
            if (i == j) continue;
            glm::vec3 direction = particles[j].position - particles[i].position;
            float distance = glm::length(direction);
            if (distance < 0.01f) distance = 0.01f;
            glm::vec3 force = G * particles[i].mass * particles[j].mass / (distance * distance) * glm::normalize(direction);
            particles[i].applyForce(force);
        }
    }

    for (auto& particle : particles) {
        particle.update(dt);
    }
}

const std::vector<Particle>& Simulation::getParticles() const {
    return particles;
}

std::vector<glm::vec3> Simulation::getParticlePositions() const {
    std::vector<glm::vec3> positions;
    for (const auto& particle : particles) {
        positions.push_back(particle.position);
    }
    for (size_t i = 0; i < positions.size(); ++i) {
        if (glm::any(glm::isnan(positions[i])) || glm::any(glm::isinf(positions[i]))) {
            std::cerr << "Invalid particle position detected: ("
                      << positions[i].x << ", " << positions[i].y << ", " << positions[i].z << ")" << std::endl;
        }
    }

    return positions;
}


