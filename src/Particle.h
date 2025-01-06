//
// Created by Lucas Wang on 2025-01-05.
//

#ifndef NBODY_PARTICLE_H
#define NBODY_PARTICLE_H

#include "glm/glm.hpp"

class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;

    Particle(glm::vec3 position, glm::vec3 velocity, float mass);
    void applyForce(const glm::vec3& force);
    void update(float dt);

};


#endif //NBODY_PARTICLE_H
