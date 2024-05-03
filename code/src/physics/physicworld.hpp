#pragma once

// Standard libraries
#include <vector>

// GLM
#include <glm/glm.hpp>

// Personnal libraries
#include "src/entity/entity.hpp"


class PhysicWorld
{
private:
    std::vector<Entity> m_entities;
    glm::vec3 m_gravity = glm::vec3(0.0f, -9.81, 0.0f);

public:
    PhysicWorld();
    ~PhysicWorld();

};