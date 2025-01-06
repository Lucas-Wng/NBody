//
// Created by Lucas Wang on 2025-01-05.
//

#ifndef NBODY_SIMULATION_H
#define NBODY_SIMULATION_H

#include <vector>
#include "Particle.h"

class Simulation {
    std::vector<Particle> particles;
    const float G = 6.67430e-11f; // Gravitational constant
public:
    void addParticle(const Particle& particle);
    void update(float dt);
    const std::vector<Particle>& getParticles() const;
    std::vector<glm::vec3> getParticlePositions() const;
};


#endif //NBODY_SIMULATION_H
