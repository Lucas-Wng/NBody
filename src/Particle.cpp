//
// Created by Lucas Wang on 2025-01-05.
//

#include "Particle.h"

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float mass) : position{position}, velocity{velocity}, mass{mass} {}

void Particle::applyForce(const glm::vec3 &force) {
    acceleration += force / mass;
}

void Particle::update(float dt) {
    velocity += acceleration * dt;
    position += velocity * dt;
    acceleration = glm::vec3{0.0f};
}


